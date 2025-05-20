// Am peaks at 59.54 keV
//   60keV (36%), 18keV (18%), 14keV (13%) according to one reference
//   13.9 (37%), 26.3 (2.3%), 59.54 (36%) from nudat
//   but just google image search americium spectrum, lots of examples of linesat 13.95, 17.74, 20.8, 26.4
//   try searching e.g. Am-241 "17.74 keV". Some references have 22.12 instead of 20.8, this seems closer to our data

// {0, 13.95, 17.74, 20.8, 26.4, 59.54}

gStyle->SetOptStat(0)
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser = "07220420_0842" // Side2readout 100V with prebias, dP/dI=0.7 uV  

TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)

z->SetLineColor(kRed)

TCut crand = "EventCategory" 

TCut cam = "PDINTall/PTINTall>0.165&&PDINTall/PTINTall<0.23"


TCut cearly = "Entry$<113000"

z->Draw("1e6*(0.866*(PDWKr20-PCWKr20)):1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))>>h(200,-30,30,200,-30,30)",!crand+cearly+"PTINTall>0.0005&&PTINTall<0.0015","colz")
h->SetTitle("Side2-readout 100V, ~10 to ~30 keVee;Side2 inner-ring x_delay [#mus];Side2 inner-ring y_delay [#mus]")
h->Draw("colz")
c1->Print("~/K100/run076/r76_s2_100V_xydel.png")
c1->Print("/data/chocula/fritts/run076/r76_s2_100V_xydel.png")

TCut cd = "1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))>14.5&&1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))<18&&1e6*(0.866*(PDWKr20-PCWKr20))>-8&&1e6*(0.866*(PDWKr20-PCWKr20))<-4"

TCut cvearly = "Entry$<76e3"

z->Draw("1e6*(PFWKr20-PAWKr20)>>hfa0(200,-30,30)",!crand+cvearly+"PTINTall>0.0005&&PTINTall<0.0015")
z->Draw("1e6*(PFWKr20-PAWKr20)>>hfa1(200,-30,30)",!crand+cvearly+"PTINTall>0.0005&&PTINTall<0.0015"+cd)
z->Draw("1e6*(PFWKr20-PBWKr20)>>hfb0(200,-30,30)",!crand+cearly+"PTINTall>0.0005&&PTINTall<0.0015")
z->Draw("1e6*(PFWKr20-PBWKr20)>>hfb1(200,-30,30)",!crand+cearly+"PTINTall>0.0005&&PTINTall<0.0015"+cd)

hfa0->SetLineColor(kBlue);hfa1->SetLineColor(kRed);
hfb0->SetLineColor(kBlue);hfb1->SetLineColor(kRed);
hfa0->SetTitle("Side2-readout 100V, ~10 to ~30 keVee;pF versus pA r_delay [#mus]")
hfb0->SetTitle("Side2-readout 100V, ~10 to ~30 keVee;pF versus pB r_delay [#mus]")
TLegend leg(0.6,0.75,0.89,0.89);leg.SetFillColor(kWhite);
leg.AddEntry(hfa0,"all events","l")
leg.AddEntry(hfa1,"xy_del peak selected","l")


hfa0->Draw();hfa1->Draw("same");leg.Draw();
c1->Print("~/K100/run076/r76_s2_100V_rdel_fa.png")
c1->Print("/data/chocula/fritts/run076/r76_s2_100V_rdel_fa.png")

hfb0->Draw();hfb1->Draw("same");leg.Draw();
c1->Print("~/K100/run076/r76_s2_100V_rdel_fb.png")
c1->Print("/data/chocula/fritts/run076/r76_s2_100V_rdel_fb.png")

z->Draw("PTOFamps",!crand+cam)
z->Draw("PTINTall>>h(200,0,0.004)",!crand+camquad) // wow - in 100V data this shows all the lines *much* more clearly (60keV included)

z->Draw("log10(PTOFamps)>>hl(200,-7,-5)",!crand+cam)

z->SetAlias("PTOFkeV","PTOFamps*13.9/1.29130e-06") // 20V, based on 14keV line
z->SetAlias("PTOFkeV","PTOFamps*13.9/3.41801e-06") // 100V, based on 14keV line



//success:

gStyle->SetOptStat(0)
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser = "07220420_0842" // Side2readout 100V with prebias, dP/dI=0.7 uV
char* ser = "07220422_1250"
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)
z->SetLineColor(kRed)
TCut crand = "EventCategory"
//z->Draw("PDINTall/PTINTall:PTWKf40-PTWKr40>>h(300,0,0.001,200,0,0.5)",!crand,"colz")



z->Draw("PFbs:PFINTall",crand)
TCut cbsf = "PFbs<3300"
z->Draw("PDINTall/PTINTall:PTWKf40-PTWKr40>>h(300,0,0.001,200,0,0.5)",!crand+cbsf,"colz")


TFile f("spectra_20keV.root","update")
z->Draw("PTOFkeV>>h(200,0,20)",!crand+cbsf)
h->SetName(Form("spec_%s",ser))
h->Write()



// ------------------------------------------------------------------------
// Calculate Absorbed Phonon Energy - New method with nonlinear term
// ------------------------------------------------------------------------
// HV setup data
// dataset-specific parameters
char* chanIDs[6]={"PA", "PB", "PC", "PD", "PE", "PF"} // channel names in Detector order, all below are also in Detector order
float gdcrc = 2 
float gdriver = 1
float prefac1[6] = {0.481, 0.482, 0.495, 0.448, 0.384, 0.483} // from settings beginning 2022-04-20
float qetbias_muamp[6] = {182.1, 188.1, 174.5, 198.8, 229.6, 182.1} // from settings beginning 2022-04-20
float res_ratio[6] = {1.848, 1.953, 2.371, 2.522, 2.890, 2.149} // derived from IbIs. this is 1+Rpar/Rshunt. From R75 IbIs.
// fixed parameters
float adcPERvolt = 6553.6, secPERsample = 8e-7;
float rfb_ohms = 1233; float gloop = 10.5;
float rshunt_ohm = 8.e-3 
//float joulePERkev = 1.60218e-16
// New courtesy of Nick: conversion for the INTall RQs to convert them to ADC*sample units, using the "norm" RQ:
for(int c=0;c<6;c++)z->SetAlias(Form("%srawINT",chanIDs[c]),Form("%sINTall*%snorm",chanIDs[c],chanIDs[c]))
// convert from units ADC*samples to V*s
for(int c=0;c<6;c++)z->SetAlias(Form("%sINT_Vs",chanIDs[c]),Form("%srawINT/%f*%e",chanIDs[c],adcPERvolt,secPERsample))
// convert from V*s to coulombs (amount of charge through TES). need to know gains.
float rgain_ohm[6]; for(int c=0;c<6;c++)rgain_ohm[c]=rfb_ohms*gloop*gdcrc*gdriver
for(int c=0;c<6;c++)z->SetAlias(Form("%sINT_C",chanIDs[c]),Form("%sINT_Vs/%f",chanIDs[c],rgain_ohm[c]))
// calculate tau
for(int c=0;c<6;c++)z->SetAlias(Form("%s_it",chanIDs[c]),Form("-20*%sWKr10+-5*%sWKr20+-10*%sWKr30+-10*%sWKr40+-10*%sWKr50+-10*%sWKr60+-10*%sWKr70+-10*%sWKr80+-10*%sWKr90+0*%sWKr100+10*%sWKf90+25*%sWKf80+20*%sWKf40+40*%sWKf20",chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c]))
for(int c=0;c<6;c++)z->SetAlias(Form("%s_i2t",chanIDs[c]),Form("-300*%sWKr10+-350*%sWKr20+-600*%sWKr30+-800*%sWKr40+-1000*%sWKr50+-1200*%sWKr60+-1400*%sWKr70+-1600*%sWKr80+-1800*%sWKr90+0*%sWKr100+1800*%sWKf90+3250*%sWKf80+2800*%sWKf40+1200*%sWKf20",chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c],chanIDs[c]))
for(int c=0;c<6;c++)z->SetAlias(Form("%s_tau_s",chanIDs[c]),Form("%s_it**2/%s_i2t",chanIDs[c],chanIDs[c]))
// nonlinear correction factor. This is multiplied by INTall to make it proportionial to absorbed energy.
for(int c=0;c<6;c++)z->SetAlias(Form("%s_nlc",chanIDs[c]),Form("1+%f/%f*%sINT_C/%s_tau_s/%e",res_ratio[c],prefac1[c],chanIDs[c],chanIDs[c],qetbias_muamp[c]/1e6))
	
TCut cam = "PDINTall/PTINTall>0.19&&PDINTall/PTINTall<0.24"
TCut c14 = "PTWKf40-PTWKr40>0.00044&&PTWKf40-PTWKr40<0.00048"
z->Draw("PE_nlc:PEINTall>>h(300,0,0.0015,200,1,1.5)",!crand,"colz")
z->Draw("PE_nlc:PEINTall",!crand+c14+cam,"same")





