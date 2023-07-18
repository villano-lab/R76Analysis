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
qualityfilters = fittingfilters+["PTOFamps0"]
calibrationaliases = ["crand","PTwid","xdel","ydel","phidel","pt_keV","pt0_keV","PSUMbs","BSel","cbs"]
qualityaliases = calibrationaliases+["cgoodwalk","PTINTall_PTbscorr","cofintl","cofintt"]
y_calib = [13.95,17.74]

paths = {
    'fritts': "/data/chocula/fritts/data/k100proc/midasrq/byseries/",
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
def pt0_keV(x):
    return 7.738820e+07*x['PTOFamps0']+1.653756e+13*x['PTOFamps0']**2
def PTINTall_PTbscorr(x):
    try:
        PTdbsarr = PTdbs(x)
    except:
        x['PTdbs'] = PTdbs(x)
    return x['PTINTall'] - (PTdbsarr<0)*0.5*PTdbsarr*4096/x['PFnorm']
def xdel(x):
    return 1e6*(x['PEWKr20'] - 0.5*(x['PCWKr20']+x['PDWKr20']))-1
def ydel(x):
    return 1e6*(0.866*(x['PDWKr20'] - x['PCWKr20']))+9
def phidel(x):
    try: #try loading instead of computing if possible
        ydelarr = x["ydel"]
    except:
        ydelarr = ydel(x).array
    try: #same here
        xdelarr = x["xdel"]
    except:
        xdelarr = xdel(x).array
    return [180/np.pi*math.atan2(ydelarr[j],xdelarr[j]) for j in range(len(ydelarr))]
def afdel(x):
    return 1e6*(x["PFWKr20"]-x["PAWKr20"])
def PSUMbs(x):
    return x["PAbs"]+x["PBbs"]+x["PCbs"]+x["PDbs"]+x["PEbs"]+x["PFbs"]
def BSel(x):
    return x["PSUMbs"]-17900
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
def cbs(x):
    try: #try to call exist
        return x["Bsel"]<1250
    except: #build if does not yet exist.
        return BSel(x)<1250
def cgoodwalk(x):
    return (x["PCWKr20"] > 0.25e-3) & (x["PCWKr20"] < 0.5e-3) & (x["PDWKr20"] > 0.25e-3) & (x["PDWKr20"] < 0.5e-3) & (
        x["PEWKr20"] > 0.25e-3) & (x["PEWKr20"] < 0.5e-3)
def cofintl(x):
    try:
        PTINTall_arr = x["PTINTall_PTbscorr"]
    except:
        PTINTall_arr = x["PTINTall_PTbscorr"]
    return (PTINTall_arr/x["PTOFamps"]>50) & (PTINTall_arr/x["PTOFamps"]<750)
def cofintt(x):
    try:
        PTINTall_arr = x["PTINTall_PTbscorr"]
    except:
        PTINTall_arr = x["PTINTall_PTbscorr"]
    return (PTINTall_arr/x["PTOFamps"]>250) & (PTINTall_arr/x["PTOFamps"]<550)
        
#Functions
def makechain(filelist,filters=None,friends=True,aliases=[],trees=["e","z"]):
    #filelist: matches a list of files to import as the chain
    #filters: matches branches to import. If none, import everything
    #friends: if true, makes all entries in all trees accessible through z. only works if z is in trees list
    #aliases: list of standard 'aliases' to generate by name (string).
    #trees: list of trees to import, by nickname (str). these should probably be redone
    warnings.filterwarnings("error")
    if(not filelist):
        raise ValueError("There are no files provided!")
    e_chain = pd.DataFrame(); 
    z_chain = pd.DataFrame();
    z4_chain = pd.DataFrame();
    listframes = [[],[],[]]
    retry = False
    try:
        if "e" in trees:
            estuff = uproot.iterate(filelist+":rqDir/eventTree",filter_name=filters,
                                library="pd")
        if "z" in trees:
            zstuff = uproot.iterate(filelist+":rqDir/zip1",filter_name=filters,
                                library="pd")
        if "z4" in trees:
            z4stuff = uproot.iterate(filelist+":rqDir/zip4",filter_name=filters,
                                library="pd")
    except:
            warnings.resetwarnings()
            if(filters == None):
                warnings.warn("A warning was thrown when trying to open the first file and you are not using filters. Consider using filters to speed up the loading process. See the package-provided fittingfilters for ideas.")
            else:
                warnings.warn("A warning was thrown and I tried to intercept it, but failed. Retrying with this behavior disabled.")
                retry = True
    warnings.resetwarnings() #we are done catching warnings, so disable this behavior.
    warnings.simplefilter(action='ignore', category=pd.errors.PerformanceWarning)
    if retry:
        if "e" in trees:
            estuff = uproot.iterate(filelist+":rqDir/eventTree",filter_name=filters,
                                library="pd")
        if "z" in trees:
            zstuff = uproot.iterate(filelist+":rqDir/zip1",filter_name=filters,
                                library="pd")
        if "z4" in trees:
            z4stuff = uproot.iterate(filelist+":rqDir/zip4",filter_name=filters,
                                library="pd")
    if "e" in trees:
        for ething in estuff:
            listframes[0].append(ething)
            e_chain = pd.concat(listframes[0])
    if "z" in trees:
        for zthing in zstuff:
            listframes[1].append(zthing)
            z_chain = pd.concat(listframes[1])
    if "z4" in trees:
        for z4thing in z4stuff:
            listframes[2].append(z4thing)
            z4_chain = pd.concat(listframes[2])
    if (e_chain.empty and z_chain.empty and z4_chain.empty):
        if filters != None:
            warnings.warn("Returning nothing! Does the series contain the filters provided?"+str(filters))
        else:
            warnings.warn("Returning nothing! Files appear to be empty.")
    if friends:
        if "e" in trees:
            z_chain = pd.concat([e_chain,z_chain],axis=1)
        if "z4" in trees:
            z4_temp = z4_chain.copy()
            for name in z4_temp.keys():
                z4_temp = z4_temp.rename(columns={name:"zip4."+name})
            z_chain = pd.concat([z_chain,z4_temp],axis=1)
    for alias in aliases:
        z_chain[alias] = globals()[alias](z_chain)
    if "e" in trees:
        if "z" in trees:
            if "z4" in trees:
                return e_chain,z_chain,z4_chain
            else:
                return e_chain,z_chain
def makechain_list(serieslist,path="",filters=None,friends=True,aliases=[],trees=["e","z"]):
    if "e" in trees:
        e = list(np.zeros(len(serieslist)))
    if "z" in trees:
        z = list(np.zeros(len(serieslist)))
    if "z4" in trees:
        z4 = list(np.zeros(len(serieslist)))
    for i,s in enumerate(serieslist):
        if "e" in trees:
            if "z" in trees:
                if "z4" in trees:
                    e[i],z[i],z4[i] = makechain(path+s+"/umn_*.root",filters=filters,friends=friends,aliases=aliases,trees=trees)
                else:
                    e[i],z[i] = makechain(path+s+"/umn_*.root",filters=filters,friends=friends,aliases=aliases,trees=trees)
    if "e" in trees:
        if "z" in trees:
            if "z4" in trees:
                return e,z,z4
            else:
                return e,z

def getonlinefiles(directory,c,path=paths["fritts"]):
    c.chdir(path)
    ls = c.listdir(directory); ls = fnmatch.filter(ls, 'umn*.root')
    filelist = [c.open(directory+"/"+filename) for filename in ls]
    return filelist

def makeonlinechain(directory,user=None,filters=None,path=paths["fritts"]):
    if user == None:
        user = getpass.getuser()
    with Connection("cdms2.spa.umn.edu",user) as c:
        filelist = getonlinefiles(directory,c)
        return makechain(filelist,fittingfilters)
    
def applybscorr(z,ser,corrections=None,m=None,b=None,path="./"):
    #z: a list of chains as output by makechain_list or iteration over makechain
    #ser: a list of series used for generating z
    #corrections: a pandas dataframe holding baseline correction data (optional).
    #m,b: slope,intercept (optional). If unprovided will be derived from csv.
    #path: the relative path leading to baselinecorrectionvalues, if corrections is not none.
    for i,x in enumerate(z):
        if (corrections == None) and (m == None) and (b == None): #read correction data if missing
            corrections = pd.read_csv(path+"baselinecorrectionvalues.csv")
        if (m == None) and (b == None):
            pass #actually this should assign them but y'know
    raise ValueError("This function isn't done yet!")
    
def gaus(x,mu=0,sigma=1,A=1):
    #x: data
    #mu: mean
    #sigma: std dev
    #A: amplitude
    return A*np.exp(-(x-mu)**2/(2*sigma**2))

def E_cal(E_raw,a,c):
    #E_raw: uncalibrated value
    #a,c: calibration constants
    return c*E_raw/(a-E_raw)