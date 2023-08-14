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


import os
import traceback
import time
import argparse
from argparse import ArgumentParser, ArgumentTypeError
# Author: A. Villano
#

def parseEventList(fname="../coin_analysis/data/r76-80V-naitrig-100eV-goodchi.txt"):

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
         print(series)
         events=io.getRawEvents(pdir,s,eventNumbers=ev[s])
         allevents.append(events)

       #pulsedata = pd.DataFrame(allevents)
       pulsedata = pd.DataFrame()
       for i,df in enumerate(allevents):
         if i==0:
           pulsedata=df
         else:
           pulsedata = pd.concat([pulsedata, df])
        
       return pulsedata 

#the stuff below is so this functionality can be used as a script
########################################################################
if __name__ == "__main__":

        #make a parser for the input
        parser = argparse.ArgumentParser(description='options')
        parser.add_argument('-s','--series', type=str, dest='series', default='07181007_1415', help='series to load')
        #parser.add_argument('-d','--filedir', type=str, dest='filedir', default='./', help='directory to look for files')
        #parser.add_argument('-x','--regex', type=str, dest='regstr', default=r'(.*?)', help='regex for picking up files')
        #parser.add_argument('-o','--outfile', type=str, dest='outfile', default='data.h5', help='output file for data')
        #parser.add_argument('-c','--cuts', type=str, dest='cuts', default='NR', help='kind of cuts to apply')
        #parser.set_defaults(filedir='./');

        args = parser.parse_args()
        series = args.series

        frittsdir = "/data/chocula/fritts/data/k100proc/midas"
        avdir = "/data/chocula/villaa/pyraw_staging"
        print(frittsdir+"raw/"+"byseries")
        elist = parseEventList()	

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


