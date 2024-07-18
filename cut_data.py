#take a file in
#allow user to specify cuts
#pspit a smaller file out

#Imports
import re
import argparse as ap
import R76Tools as R76
from itertools import chain

#Constants
#comparators = set([">","<","==","!="])
#splitters = comparators.union(["+","-","*","/","^","(",")","e+"])
pattern = r'x\[\"(.*?)\"\]'
filters = ["Row","SeriesNumber","EventNumber","pt_keVee","coinwin","PTOFdelay","PAWKmax","PAWKr50","cchisq"]
aliases = []
cuts = []

#Command line args
parser = ap.ArgumentParser(prog='cut_data',description='Apply cuts to a data file, outputting a new, smaller file',
                    epilog='=^w^=') #i didn't know what to put here so have a cat.
parser.add_argument('cutfile',help='text file with list of cuts to apply')  #Required positional argument for cuts input file name
parser.add_argument('series',type=int,nargs='+',help='series to cut data from')
parser.add_argument('-p','--path',default='./',help='path to look for series in')
parser.add_argument('-o','--out','--outfile','--output',default=None,help='name of file to output. (if None, will output at datafile_cut.txt') #Optional argument for output file name, defaults to in_cut.txt
parser.add_argument('-w','--overwrite',type=bool,default=False,help='overwrite on output if target already exists.')
args = parser.parse_args()

#If output file name was not provided, then make it now
if args.out is None:
    args.out = str(args.series[0])+"_cut.txt"

#Parse the cuts to be done from the file
#cutfile = open(cutfile,"r")
for line in open(args.cutfile,"r"):
    nline = line.strip() #line is immutable so we create a new variable
    #Here's a bunch of stripping and error handling.
    if nline in ["",None] or nline[0] == "#": continue #Skip comments and empty lines
    nline = nline.split("#")[0] #kill inline comments
    if len(comparators & set(line)) != 1:
        raise ValueError("The line `"+line+"` provided the wrong number of comparators. Found "+len(comparators & set(nline))+" comparators; there should be exactly one. Aborting.")
    if nline.count("(") != nline.count(")"):
        raise ValueError("The line `"+nline+"` contains "+nline.count("(")+" open parens but "+nline.count(")")+" close parens. Aborting.")
    #Now we want to figure out what's a name and what's not
    to_sort = re.findall(pattern,nline)
    for phrase in to_sort:
        try: #Check if it's an alias
            getattr(R76,value)
                aliases.append(value)
            except AttributeError: #Assume it's a filter
                filters.append(value) 
    cuts.append(nline)

#Now expand dependencies and kill duplicates
aliases = set(aliases)
for alias in aliases: 
    for filter_val in R76.aliasdeps[alias]: filters.append(filter_val)
filters = set(filters)
trees = []
for filter_val in filters: trees.append(R76.filterdeps[filter_val])
trees = set(trees)

#Open files for reading/writing
_,z = R76.makechain_list(series,path=path,filters=filters,aliases=aliases,trees=trees)
if overwrite: fout  = open(out,"w")
else: fout = open(out,"x")

#rands = True #Rather than this we'll just count them and if there're 0 then we won't worry about it.
cuts = [] #Now we're finally ready to sort out the cuts themselves.

#Close files before exit
fcuts.close(); fout.close()