
#!/usr/bin/env python

import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)
#warnings.resetwarnings()



#Imports
import numpy as np
import matplotlib.pyplot as plt
import glob, pickle, os, sys, uproot, sys
#import joblib
import uproot
import pickle
import pandas as pd
import statistics as st


import os
import traceback
import time
import argparse
from argparse import ArgumentParser, ArgumentTypeError


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

def getPTPulse(pulses,series=72208301829,ev=550152,chan=[('Z1','PA'),('Z1','PB'),('Z1','PD'),('Z3','PA'),('Z3','PB'),('Z3','PD')]):
       ptpulse=[]
       ccount=0
       for c in chan:
           #print(c[0])
           pulse = pulses[c[0]][c[1]][series,ev]
           if(isRailed(pulse)):
               continue
           mean = np.mean(pulse[100:])
           #print(mean)
           pulse = pulse-mean
           #print(pulse)
           #print(np.size(ptpulse))
           if(ccount==0):
               ptpulse=pulse
           else:
               ptpulse+=pulse
           ccount+=1

       #print(ptpulse)
       return ptpulse

def isRailed(v):
       if(np.isnan(v).any()):
           return True
       if(st.mode(v)==0):
           return True
       return False
