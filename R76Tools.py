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

# Cuts
def crand(x):
    return np.array([(y==1) for y in x['EventCategory']])
def cam(x):
    return x['PFOFamps']/x['PTOFamps']>0.21 #Americium cut
def cphi1(x):
    return [(y>5) and (y < 20) for y in phidel(x)]

#Functions
def makechain(filelist,filters=None):
    e_chain = pd.DataFrame(); z_chain = pd.DataFrame();        
    for file in filelist:
        if e_chain.empty: #For first entry, generate columns
                #Thin data before concatenating; runs much faster
                toadd = uproot.open(file)["rqDir/eventTree"].arrays(library="pd",filter_name=filters)
                e_chain = pd.concat([e_chain,toadd],axis=1)
                
                toadd = uproot.open(file)["rqDir/zip1"].arrays(library="pd",filter_name=filters)
                z_chain = pd.concat([z_chain,toadd],axis=1)
        else:
            toadd = uproot.open(file)["rqDir/eventTree"].arrays(library="pd",filter_name=filters)
            e_chain = pd.concat([e_chain,toadd],ignore_index=True)

            toadd = uproot.open(file)["rqDir/zip1"].arrays(library="pd",filter_name=filters)
            z_chain = pd.concat([z_chain,toadd],ignore_index=True)
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