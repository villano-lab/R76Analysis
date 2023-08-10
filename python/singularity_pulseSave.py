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


import os
import traceback
import time
import argparse
from argparse import ArgumentParser, ArgumentTypeError
# Author: A. Villano
#

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
        print(frittsdir+"raw/"+"byseries")
	

        try:
          events=io.getRawEvents(frittsdir+"raw/"+"byseries/",series)
          print(np.shape(events))
        except KeyboardInterrupt:
          print('Shutdown requested .... exiting')
        except Exception:
          traceback.print_exc(file=sys.stderr)


