#!/usr/bin/env python

import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
#warnings.resetwarnings()



#Imports
import numpy as np
import matplotlib.pyplot as plt
import rawio.IO as io #will import at all?
import glob, pickle, os, sys, uproot, sys
#import joblib
#from tqdm import tqdm_notebook
from scdmsPyTools.Traces.Filtering import *
from scdmsPyTools.Traces.Noise import *
from scdmsPyTools.TES.Templates import *
from scdmsPyTools.Traces.Stats import *
#from scdmsPyTools.BatTools.IO import *
from rawio import IO
from ROOT import TCanvas, TGraph, gStyle, TH1F
#from ROOT import TF1
import uproot
import pickle
import pandas as pd
import statistics as st


import os
import traceback
import time
import argparse
from argparse import ArgumentParser, ArgumentTypeError
# Author: A. Villano
#

def parseEventList(fname="../coin_analysis/data/r76-80V-nairand-100eV-badchi.txt"):

       evdata = pd.read_csv(fname, skiprows=0, engine='python',skipfooter=0, \
            names=['SeriesNumber','EventNumber'], \
            delim_whitespace=True)

       print (evdata.head(10))

       sn = np.asarray(evdata['SeriesNumber'])
       en = np.asarray(evdata['EventNumber'])

       print(np.unique(sn))

       useriesgood = np.unique(sn)

       #for s in useriesgood:
       #    cseries=(sn==s)
       #    print(en[cseries])
       slist = {}
       for s in useriesgood:
           cseries=(sn==s)
           print(en[cseries])
           sseries = '{}'.format(s)
           sseries = '0'+sseries
           sseries = sseries[:-4] + '_' + sseries[-4:]
           slist[sseries] = en[cseries]
           #slist.append(en[cseries])

       print(slist)
       return slist

def gatherPulses(pdir="/data/chocula/villaa/pyraw_staging/",ev=None):
     
       allevents=[]
       for s in ev:
         print(pdir)
         print(s)
         print(ev)
         #eventShift(pdir,series=s,ev=ev[s])
         dd=getDumpDict(ev=ev[s])
         print(dd)
         events=fetchModifiedDF(pdir,series=s,dumps_dict=dd)
         #events=io.getRawEvents(pdir,s,eventNumbers=ev[s],skipEmptyEvents=False)
         allevents.append(events)

       #pulsedata = pd.DataFrame(allevents)
       pulsedata = pd.DataFrame()
       for i,df in enumerate(allevents):
         if i==0:
           pulsedata=df
         else:
           pulsedata = pd.concat([pulsedata, df])
        
       return pulsedata 

def getDumpDict(ev=None):

       dumps=(ev-ev%10000)/10000
       dumps=dumps.astype(int)
       udumps=np.unique(dumps)
       print(udumps)

       dumpdict=dict()

       for d in udumps:
         theseev = np.sort(ev[dumps==d])
         dumpdict[d]=theseev

       print(dumpdict)

       return dumpdict
       
def fetchModifiedDF(pdir="/data/chocula/villaa/pyraw_staging/",series='07220830_2118',dumps_dict=None):
       # initialize list
       event_data_list=list()
       event_num_list=list()
       event_series_list=list()
       #df=pd.DataFrame()
  
       # instantiate  raw data reader 
       myreader = io.RawDataReader(filepath=pdir, series=series)
 
       # loop dumps
       for  dump, event_nums in dumps_dict.items():
          events =  myreader.read_events(dump_nums=[dump], output_format=2, skip_empty=True)
 
          # loop event numbers for the dump
          for event_num in  event_nums:
 
            # extract data based on index
            index = event_num%10000
            data = events[index]
            #data = events.iloc[index]
            #print(type(data))
            #print(type(df))
            #data = data.to_frame()
            #df = pd.concat([df,data])
            

            #print(data) 
            for key in data.keys():
              if key.startswith('Z'):
                for key2 in data[key].keys():
                  print((key,key2))
            detectors = [key for key in data.keys() if key.startswith('Z')]
            #print(detectors)   
            #for key in detectors:
            #  print(data[key])       
    
            # append to list
            event_num_list.append(event_num)
            event_series_list.append(data['event']['SeriesNumber'])
            event_data_list.append(pd.Series({key: data[key] for key in detectors}))   
 
 
       # convert to multi index dataframe
       index_arrays = [np.array(event_series_list), np.array(event_num_list)]
       df = pd.DataFrame(event_data_list, index=index_arrays)
 
       return df

def eventShift(pdir="/data/chocula/villaa/pyraw_staging/",series='07220830_2118',ev=None):

       dumps=(ev-ev%10000)/10000
       dumps=dumps.astype(int)
       udumps=np.unique(dumps)
       print(udumps)

       dumpdict=dict()

       for d in udumps:
         theseev = np.sort(ev[dumps==d])
         dumpdict[d]=theseev
         #make an event number vector that has all events up to the max
         fullev=[]
         #for i in np.arange(d*10000,np.max(theseev)+1):
         #for i in np.arange(d*10000,d*10000+9999):
         #  fullev.extend([i])

         print(fullev)
         #events=io.getRawEvents(pdir,series,eventNumbers=fullev)
         myreader = io.RawDataReader(filepath=pdir, series=series) 
         events =  myreader.read_events(dump_nums=[d], output_format=1, skip_empty=True)
         railed=[]
         for oneev in events:
           print(oneev)

       print(dumpdict)
         #for j in fullev:
         #  intseries=np.char.replace(series,'_','')
         #  intseries=int(intseries)
         #  #print(intseries)
         #  print(events['Z1']['PA'][intseries,j])
         #  if isRailed(events['Z1']['PA'][intseries,j]):
         #    railed.extend([j])

         #railed=np.asarray(railed)
         ##print(np.shape(railed)) 
         #shifts=[]
         #for e in theseev:
         #  shifts.extend([np.sum(railed<=e)])

         #print(shifts)


       return

def isRailed(v):
    if(np.isnan(v).any()):
        return True
    if(st.mode(v)==0):
        return True
    return False

#the stuff below is so this functionality can be used as a script
########################################################################
if __name__ == "__main__":

        #make a parser for the input
        parser = argparse.ArgumentParser(description='options')
        parser.add_argument('-l','--evlist', type=str, dest='evlist', default='../coin_analysis/data/r76-80V-naitrig-100eV-goodchi.txt', \
        help='series/event list')
        #parser.add_argument('-s','--series', type=str, dest='series', default='07181007_1415', help='series to load')
        #parser.add_argument('-d','--filedir', type=str, dest='filedir', default='./', help='directory to look for files')
        #parser.add_argument('-x','--regex', type=str, dest='regstr', default=r'(.*?)', help='regex for picking up files')
        #parser.add_argument('-o','--outfile', type=str, dest='outfile', default='data.h5', help='output file for data')
        #parser.add_argument('-c','--cuts', type=str, dest='cuts', default='NR', help='kind of cuts to apply')
        #parser.set_defaults(filedir='./');

        args = parser.parse_args()
        #series = args.series
        evlist = args.evlist

        frittsdir = "/data/chocula/fritts/data/k100proc/midas"
        avdir = "/data/chocula/villaa/pyraw_staging"
        #print(frittsdir+"raw/"+"byseries")
        elist = parseEventList(evlist)	

        try:
          #events=io.getRawEvents(frittsdir+"raw/"+"byseries/",series)

          print('trying...')
          pulsedata = gatherPulses(pdir=frittsdir+"raw/byseries/",ev=elist)
          print(np.shape(pulsedata))
          #events=io.getRawEvents(avdir+"/",series,eventNumbers=[1970453])
          #onepulse=events['Z1']['PA'][72209171225, 1970453]
          #print(np.shape(onepulse))
          fileObj = open('pulses.pkl', 'wb')
          pickle.dump(pulsedata,fileObj)
          fileObj.close()

          ##how to read
          #fileObj = open('data.obj', 'rb')
          #exampleObj = pickle.load(fileObj)
          #fileObj.close()
          #print(exampleObj)
        except KeyboardInterrupt:
          print('Shutdown requested .... exiting')
        except Exception:
          traceback.print_exc(file=sys.stderr)


