#Imports
import uproot, getpass, fnmatch, math, warnings
import pandas as pd
import numpy as np
try:
    from pysftp import Connection
except ModuleNotFoundError:
    warnings.warn("Module `pysftp` was missing. Online functions for use of data via SSH will be unavailable.")#,warnings.ImportWarning)

#Some constants to be able to call
fittingfilters = ["EventCategory","PTWKf40","PTWKr40","P[A-Z]bs","P[A-Z]bspost","PTINTall","PFnorm","P[A-Z]OFamps","P[A-Z]WK[rf][24]0"]
qualityfilters = fittingfilters+["PTOFamps0"] #May be able to trim this
coincidencefilters = qualityfilters+["PTOFdelay"] #^same
calibrationaliases = ["crand","PTwid","xdel","ydel","phidel","pt_keV","pt0_keV","PSUMbs","BSel","cbs"]
qualityaliases = calibrationaliases+["cgoodwalk","PTINTall_PTbscorr","cofintl","cofintt"]
y_calib = [13.95,17.74]

paths = {
    'fritts': "/data/chocula/fritts/data/k100proc/midasrq/byseries/",
    'neog': "/data/chocula/neog/rq/"
}

goodseries = [ #A full list of series on the R76 page of the spreadsheet that don't have red text in the notes columns
    #Also excludes events that say they were tests
    '07220330_1639', '07220405_1646', '07220406_1720', '07220420_0842', '07220421_1010', '07220422_1144', '07220422_1250', '07220425_0722',
    '07220425_1144', '07220425_1621', '07220425_2032', '07220426_1120', '07220426_2022', '07220427_1630', '07220428_1659', '07220503_0851',
    '07220510_0917', '07220510_1115', '07220513_0853', '07220513_0952', '07220531_0810', '07220531_1214', '07220601_1310', '07220608_1539',
    '07220608_1606', '07220609_1114', '07220615_1205', '07220617_1209', '07220617_1305', '07220701_1439', '07220702_0911', '07220702_1055',
    '07220703_1904', '07220704_1140', '07220706_0836', '07220706_1254', '07220706_1303', '07220706_1652', '07220706_1659', '07220706_2041',
    '07220706_2048', '07220707_0632', '07220707_0644', '07220707_1643', '07220707_1748', '07220707_1758', '07220707_2149', '07220707_2205',
    '07220708_0826', '07220708_0833', '07220708_1518', '07220708_1618', '07220708_1632', '07220708_2207', '07220708_2217', '07220709_0926',
    '07220709_0935', '07220709_1415', '07220709_1717', '07220709_1807', '07220709_2144', '07220711_0828', '07220711_1649', '07220711_1709',
    '07220712_1016', '07220712_1324', '07220712_1336', '07220812_1805', '07220822_1828', '07220826_1219', '07220826_1536', '07220826_2007',
    '07220827_1153', '07220830_1724', '07220830_1829', '07220830_2033', '07220830_2118', '07220831_0955', '07220831_1002', '07220831_1151',
    '07220831_1846', '07220831_2022', '07220906_1510', '07220906_1614', '07220909_1256', '07220909_1340', '07220909_1648', '07220909_2023',
    '07220909_2102', '07220909_2243', '07220910_1210', '07220910_1517', '07220910_1608', '07220912_1955', '07220912_2107', '07220913_0814',
    '07220913_1457', '07220913_1546', '07220914_1133', '07220916_2059', '07220916_2200', '07220917_1039', '07220917_1225', '07220917_2125',
    '07220917_2155', '07220919_1723', '07220919_2012', '07220920_0925', '07220920_1007', '07220920_2052', '07220920_2138', '07220921_0915',
    '07220921_1034', '07220921_1541', '07220921_1631', '07220921_2055', '07220923_1709', '07220923_1746', '07220924_1233', '07220924_1331',
    '07220925_1006', '07220925_1056', '07220925_1320', '07220925_1649', '07220925_1737', '07220929_1526', '07220929_1629', '07221001_2314',
    '07221017_1600', '07221019_1741', '07221019_1832', '07221019_2157', '07221019_2240', '07221020_0849', '07221020_0954', '07221022_1630',
    '07221023_1051', '07221025_1751', '07221025_1956', '07221026_0951', '07221026_1049', '07221026_2145', '07221026_2224', '07221027_0915',
    '07221027_0959', '07221111_2153', '07221111_2249', '07221112_0943', '07221112_1127', '07221112_2249', '07221113_0000', '07221113_1159',
    '07221114_2141', '07221114_2223', '07221115_0845', '07221115_0919', '07221115_2109', '07221115_2201', '07221116_1003', '07221116_1048',
    '07221118_1404', '07221118_1850', '07221119_1016', '07221121_2214', '07221124_1408', '07221124_1603', '07221124_1748', '07221124_2022',
    '07221124_2331', '07221125_0547', '07221201_0118', '07221203_1255', '07221203_1418', '07221203_1622', '07221203_1744', '07221203_1906',
    '07221203_2025', '07221206_1738', '07221206_1858', '07221207_0928'
]
                     
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
def BSel(x,base=18400):
    return x["PSUMbs"]-base
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
        return x["Bsel"]<1100
    except: #build if does not yet exist.
        return BSel(x)<1100
def cgoodwalk(x):
    return (x["PCWKr20"] > 0.25e-3) & (x["PCWKr20"] < 0.5e-3) & (x["PDWKr20"] > 0.25e-3) & (x["PDWKr20"] < 0.5e-3) & (
        x["PEWKr20"] > 0.25e-3) & (x["PEWKr20"] < 0.5e-3)
def cofintl(x):
    try:
        PTINTall_arr = x["PTINTall_PTbscorr"]
    except KeyError:
        PTINTall_arr = PTINTall_PTbscorr(x)
    return (PTINTall_arr/x["PTOFamps"]>50) & (PTINTall_arr/x["PTOFamps"]<750)
def cofintt(x):
    try:
        PTINTall_arr = x["PTINTall_PTbscorr"]
    except KeyError:
        PTINTall_arr = PTINTall_PTbscorr(x)
    return (PTINTall_arr/x["PTOFamps"]>250) & (PTINTall_arr/x["PTOFamps"]<550)

aliasdeps = {
    'PTwid':["PTWKf40","PTWKr40"],
    'PTdbs':["PAbspost","PAbs","PBbspost","PBbs","PCbspost","PCbs","PDbspost","PDbs","PEbspost","PEbs","PFbspost","PFbs"],
    'pt_keV':['PTOFamps'],
    'pt0_keV':['PTOFamps0'],
    'PTINTall_PTbscorr':['PTdbs','PTINTall','PFnorm'],
    'xdel':['PEWKr20','PCWr20','PDWKr20'],
    'ydel':['PDWKr20','PCWKr20'],
    'phidel':['PEWKr20','PCWKr20','PDWKr20'],
    'afdel':['PFWKr20','PAWKr20'],
    'PSUMbs':['PAbs','PBbs','PCbs','PDbs','PEbs','PFbs'],
    'BSel':['PAbs','PBbs','PCbs','PDbs','PEbs','PFbs'],
    'bscorr':['PTOFamps','PAbs','PBbs','PCbs','PDbs','PEbs','PFbs'],
    'crand':['EventCategory'],
    'cam':['PFOFamps','PTOFamps'],
    'cphi1':['PEWKr20','PCWKr20','PDWKr20'],
    'cbs':['PAbs','PBbs','PCbs','PDbs','PEbs','PFbs'],
    'cgoodwalk':['PCWKr20','PDWKr20','PEWKr20'],
    'cofintl':['PTdbs','PTINTall','PFnorm','PTOFamps','PTINTall_PTbscorr'],
    'cofintt':['PTdbs','PTINTall','PFnorm','PTOFamps']
}

#Systematically create dictionary that associates all these things with their tree.
filterdeps = { #I am uncertain of the ones manually assigned here.
    'EventNumber': "e",
    'pt_keVee': "z",
    'PTOFdelay': "z"
} 
for letter in ['A','B','C','D','E','F','T']:
    for simple in ['OFamps','bs']:
        filterdeps['P'+letter+simple] = "z4"
    for rf in ['r','f']:
        for number in ['20','50','min','max']:
            filterdeps['P'+letter+'WK'+rf+number] = "z4"

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
    debug = False #"commenting" out some useful debugging code
    for x in aliases:
        try:
            for y in aliasdeps[x]:
                if y not in aliases: aliases.insert(0,y)
        except KeyError: #the key doesn't have dependencies
            pass
        if debug: print(aliases)
    if "e" in trees:
        e = list(np.zeros(len(serieslist)))
    if "z" in trees:
        z = list(np.zeros(len(serieslist)))
    if "z4" in trees:
        z4 = list(np.zeros(len(serieslist)))
    if isinstance(serieslist, str):
        serieslist = [serieslist] #convert to list to prevent iterating over individual characters
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

#These are super slow, you don't want to use them.
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
    

def applybscorr(z,ser,corrections=None,m=None,b=None,path="baseline_correction/"):
    #z: a list of chains as output by makechain_list or iteration over makechain
    #ser: a list of series used for generating z
    #corrections: a pandas dataframe holding baseline correction data (optional).
    #m,b: slope,intercept (optional). If unprovided will be derived from csv.
    #path: the relative path leading to baselinecorrectionvalues, if corrections is not none.
    debug = False #currently using this to comment out unwanted debugging
    if debug: print(corrections)
    if corrections == None:
        corrections = pd.read_csv(path+"baselinecorrectionvalues.csv",index=None)
        print(corrections["series"])
        corrections = corrections[corrections["series"] == ser]
        temp = corrections[corrections["series"] == x]
    m = temp['m']; b = temp['b']
    return bscorr(x,m/b)
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