#Imports
import uproot, getpass, fnmatch, math, warnings
import pandas as pd
import numpy as np
try:
    from pysftp import Connection
except:
    warnings.warn("Module `pysftp` was missing. Online functions for use of data via SSH will be unavailable.",warnings.ImportWarning)

#Some constants to be able to call
fittingfilters = ["EventCategory","PTWKf40","PTWKr40","P[A-Z]bs","P[A-Z]bspost","PTINTall","PFnorm","P[A-Z]OFamps","P[A-Z]WK[rf][24]0"]

onlinepaths = {
    'fritts': "/data/chocula/fritts/data/k100proc/midasrq/byseries",
    'neog': "/data/chocula/neog/rq/"
}
                     
#"Aliases"
def PTwid(x):
    return (x['PTWKf40'] - x['PTWKr40'])*1e6
def PTdbs(x):
    sumdiff = (x['PAbspost'] - x['PAbs'] + x['PBbspost'] - x['PBbs'] 
        + x['PCbspost'] - x['PCbs'] + x['PDbspost'] - x['PDbs'] + x['PEbspost'] - x['PEbs']
        + x['PFbspost'] - x['PFbs'])
    return sumdiff
def pt_keV(x):
    return 7.738820e+07*x['PTOFamps']+1.653756e+13*x['PTOFamps']**2
def PTINTall_PTbscorr(x):
    return x['PTINTall'] - ((x['PTdbs']<0)*0.5*x['PTdbs']*4096/x['PFnorm'])
def xdel(x):
    return 1e6*(x['PEWKr20'] - 0.5*(x['PCWKr20']+x['PDWKr20']))-1
def ydel(x):
    return 1e6*(0.866*(x['PDWKr20'] - x['PCWKr20']))+9
def phidel(x):
    ydelarr = ydel(x).array
    xdelarr = xdel(x).array
    return [180/np.pi*math.atan2(ydelarr[j],xdelarr[j]) for j in range(len(ydelarr))]
def PSUMbs(x):
    return x["PAbs"]+x["PBbs"]+x["PCbs"]+x["PDbs"]+x["PEbs"]+x["PFbs"]
def bscorr(x,ratio):
    try:
        return x["pt_keV"]/(1.+(x["PSUMbs"]-18000.)*ratio)
    except KeyError: #Create missing values if necessary
        x["PSUMbs"] = PSUMbs(x)
        x["pt_keV"] = pt_keV(x)
        return pt_keV/(1.+(x["PSUMbs"]-18000.)*ratio)

# Cuts
def crand(x):
    return np.array([(y==1) for y in x['EventCategory']])
def cam(x):
    return x['PFOFamps']/x['PTOFamps']>0.21 #Americium cut
def cphi1(x):
    return [(y>5) and (y < 20) for y in phidel(x)]

#Functions
def makechain(filelist,filters=None):
    warnings.filterwarnings("error")
    if(not filelist):
        raise ValueError("There are no files provided!")
    e_chain = pd.DataFrame(); z_chain = pd.DataFrame();
    listframes = [[],[]]
    try:
        estuff = uproot.iterate(filelist+":rqDir/eventTree",filter_name=filters,
                                library="pd")
        zstuff = uproot.iterate(filelist+":rqDir/zip1",filter_name=filters,
                                library="pd")
    except:
            warnings.resetwarnings()
            if(filters == None):
                warnings.warn("A warning was thrown when trying to open the first file and you are not using filters. Consider using filters to speed up the loading process. See the package-provided fittingfilters for ideas.")
            else:
                warnings.warn("A warning was thrown and I tried to intercept it, but failed. Sorry!")
    warnings.resetwarnings() #we are done catching warnings, so disable this behavior.
    warnings.simplefilter(action='ignore', category=pd.errors.PerformanceWarning)
    for ething,zthing in zip(estuff,zstuff):
            listframes[0].append(ething);listframes[1].append(zthing)
    z_chain = pd.concat(listframes[1])
    e_chain = pd.concat(listframes[0])
    if (e_chain.empty and z_chain.empty):
        if filters != None:
            warnings.warn("Returning nothing! Does the series contain the filters provided?"+str(filters))
        else:
            warnings.warn("Returning nothing! Files appear to be empty.")
    return e_chain,z_chain

def getonlinefiles(directory,c,path=onlinepaths["fritts"]):
    c.chdir(path)
    ls = c.listdir(directory); ls = fnmatch.filter(ls, 'umn*.root')
    filelist = [c.open(directory+"/"+filename) for filename in ls]
    return filelist

def makeonlinechain(directory,user=None,filters=None,path=onlinepaths["fritts"]):
    if user == None:
        user = getpass.getuser()
    with Connection("cdms2.spa.umn.edu",user) as c:
        filelist = getonlinefiles(directory,c)
        return makechain(filelist,fittingfilters)