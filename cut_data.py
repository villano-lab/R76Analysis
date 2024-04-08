#take a file in
#allow user to specify cuts
#pspit a smaller file out

#Imports
import re
import argparse as ap
import R76Tools as R76

#Constants
comparators = set([">","<","==","!="])
splitters = comparators.union(["+","-","*","/","^","(",")"])

#Command line args
parser = ap.ArgumentParser(prog='cut_data',description='Apply cuts to a data file, outputting a new, smaller file',
                    epilog='=^w^=') #i didn't know what to put here so have a cat.
parser.add_argument('cutfile',help='text file with list of cuts to apply')  #Required positional argument for cuts input file name
parser.add_argument('series',type=int,nargs='+',help='series to cut data from')
parser.add_argument('-p','--path',default='./',help='path to look for series in')
parser.add_argument('-o','--out','--outfile','--output',default=None,help='name of file to output. (if None, will output at datafile_cut.txt') #Optional argument for output file name, defaults to in_cut.txt
parser.add_argument('-w','--overwrite',type=bool,default=False,help='overwrite on output if target already exists.')

#If output file name was not provided, then make it now
if out is None:
    out = out[0]+"_cut.txt"
    
#Open files for reading/writing
_,z = R76.makechain_list(series,path=path,filters=[R76.fittingfilters,"PTOFamps0","PTOFdelay"],aliases=R76.qualityaliases)
fcuts = open(cutfile,"r")
if overwrite: fout  = open(out,"w")
else: fout = open(out,"x")

rands = True #To possibly change later
cuts = [] #To fill now
#Parse the cuts to be done from the file
for line.strip() in open(cutfile,"r"):
    #Here's a bunch of stripping and error handling.
    if line == "" or line[0] == "#": continue #Skip comments and empty lines
    line = line.split("#")[0] #kill inline comments
    if line == "EventCategory == 0": rands = False #Don't do randoms if we specify nonrandoms in the file
    if len(comparators & set(line)) != 1+line.count("||"):
        raise ValueError("The line `"+line+"` provided a mismatched number of comparators vs compared to the number of `||` (or operator) found. Found "+len(comparators & set(line))+" comparators but "+line.count("||")+" or operators. There should be one more comparator than the number of or operators.")
    if line.count("(") != line.count(")"):
        raise ValueError("The line `"+line+"` contains "+line.count("(")+" open parens but "+line.count(")")+" close parens. Aborting.")
    #Now we want to figure out what's a name and what's not
    line = line.split("||")
    for i,x in enumerate(line):
        for entry in splitters:
            x = re.split("("+string+")",x) #Split along the splitters
            #Now we'd want to verify that everything's either x["..."], a splitter, or a number -- how do we do that? 

#Close files before exit
fcuts.close(); fout.close()