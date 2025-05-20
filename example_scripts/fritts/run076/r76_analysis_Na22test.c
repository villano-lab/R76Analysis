// Am peaks at 59.54 keV
//   60keV (36%), 18keV (18%), 14keV (13%) according to one reference
//   13.9 (37%), 26.3 (2.3%), 59.54 (36%) from nudat
//   but just google image search americium spectrum, lots of examples of linesat 13.95, 17.74, 20.8, 26.4
//   try searching e.g. Am-241 "17.74 keV". Some references have 22.12 instead of 20.8, this seems closer to our data

// {0, 13.95, 17.74, 20.8, 26.4, 59.54}

// mu-metal outer radius: 26.8 cm

07220531_0810/ : no Na-22
07220531_1214/ : with Na-22, 48cm from mu-metal
07220601_1310/ : with Na-22, 40cm from mu-metal

/////////////
// 0V data //
/////////////

int s=2
gStyle->SetOptStat(0)
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser[] = {"07220531_0810", "07220531_1214", "07220601_1310"} //<<<<<<
char* serlabel[] = {"no_source","22Na_75cm","22Na_67cm"}
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
TChain *z4 = new TChain("rqDir/zip4")
e->Add(Form("%s/%s/umn*root",datapath,ser[s]));
z->Add(Form("%s/%s/umn*root",datapath,ser[s]));
z4->Add(Form("%s/%s/umn*root",datapath,ser[s]));
z->AddFriend(e)
z->AddFriend(z4)
TCut crand = "EventCategory"
z->SetAlias("PTwid","(zip1.PTWKf40-zip1.PTWKr40)*1e6")

//initial baseline cut
//z->Draw("PFbs:PFINTall",crand)
TCut cbsf[] = {"zip1.PFbs<2780", "zip1.PFbs<2730", "zip1.PFbs<2680"} //<<<<<<

//identify peaks using partition
//z->Draw("zip1.PDOFamps/zip1.PTOFamps:zip1.PTOFamps>>h(300,0,2e-6,200,0,0.5)",!crand+cbsf[s],"colz")
//Am-cut
TCut cam[] = {"zip1.PDOFamps/zip1.PTOFamps>0.215&&zip1.PDOFamps/zip1.PTOFamps<0.265", "zip1.PDOFamps/zip1.PTOFamps>0.235&&zip1.PDOFamps/zip1.PTOFamps<0.31", "zip1.PDOFamps/zip1.PTOFamps>0.22&&zip1.PDOFamps/zip1.PTOFamps<0.31"}

//correct amplitude to baseline
//z->SetMarkerColor(kMagenta);z->SetMarkerStyle(6);
//z->Draw("zip1.PTOFamps:zip1.PDbs",!crand+cam[s])
//TGraph g
//g.SetPoint(0,5950,0.373e-6)
//g.SetPoint(1,6050,0.35e-6)
//g.Draw("l*")
//g.Fit("pol1")
//z->SetAlias("PTOF1amps",Form("zip1.PTOFamps/(%e+%e*zip1.PDbs)",g.GetFunction("pol1")->GetParameter(0)/g.GetY()[0],g.GetFunction("pol1")->GetParameter(1)/g.GetY()[0]))
//cout<<endl<<"//z->SetAlias(\"PTOF1amps\",\""<<Form("zip1.PTOFamps/(%e+%e*zip1.PDbs)",g.GetFunction("pol1")->GetParameter(0)/g.GetY()[0],g.GetFunction("pol1")->GetParameter(1)/g.GetY()[0])<<"\")"<<endl<<endl
char* basecorrect[] = {"zip1.PTOFamps/(4.668901e+00+-6.166220e-04*PDbs)", "zip1.PTOFamps/(4.141528e+00+-5.315615e-04*zip1.PDbs)", "zip1.PTOFamps/(3.089520e+00+-7.914847e-04*zip1.PDbs)"}
z->SetAlias("PTOF1amps",basecorrect[s]) //<<<<<<

//calibrate to 14keV line
//z->Draw("PTOF1amps>>h(300,0.2e-6,1e-6)",!crand+cbsf[s]+cam[s])
//h->Fit("gaus","","",0.34e-6,0.40e-6) //<<<<<<
//float amp14 = h->GetFunction("gaus")->GetParameter(1)
//cout<<endl<<"//float amp14 = "<<amp14<<endl<<endl
float amp14[] = {3.70667e-07, 3.46075e-07, 3.71292e-07} //<<<<<<
z->SetAlias("PTOFkeV_l",Form("PTOF1amps*13.95/%e",amp14[s]))

//nonlinear energy correction
//z->Draw("PTOFkeV_l>>h(300,40,80)",!crand+cbsf[s]+cam[s])
//h->Fit("gaus","","",51,56)
//cout<<h->GetFunction("gaus")->GetParameter(1)<<endl
float l60[] = {54.0526,50.8580,51.2514}
z->SetAlias("PTOFkeV",Form("(59.54-13.95)/(%f-13.95)*(PTOFkeV_l-13.95)+13.95",l60[s]))

//calibrate pT_integral 
//z->Draw("PTINTall>>h(200,0,0.001)",!crand+cbsf[s]+cam[s]+"PTOFkeV>56&&PTOFkeV<68")
//h->Fit("gaus")
//cout<<h->GetFunction("gaus")->GetParameter(1)<<endl
float l60int[]={3.56721e-04,0.000349538,0.000356507}
z->SetAlias("PTINTkeV",Form("PTINTall*59.54/%e",l60int[s]))

//look at NaI trigger signals
//make a baseline cut
//z->Draw("zip4.PAbs>>h(200,7900,8100)")
TCut cnaibs[] = {"zip4.PAbs<7975", "zip4.PAbs<7975", "zip4.PAbs<7959"}
//look at integral
//z->Draw("zip4.PAINTall",cnaibs[s]) // this seems to show traces with zero, one, or two signals
//single trigger cut
TCut c1trig = "zip4.PAINTall>0.05e-3&&zip4.PAINTall<0.13e-3"

//z->Draw("zip1.PTWKr40>>h(100,0,0.001)",!crand+cam+"PTOFkeV>10&&PTOFkeV")
//z->Draw("zip4.PAWKr40",cnaibs[s]+c1trig,"same")

//coincidence peak cut
TCut cnaitime = "zip4.PAWKr40>0.00034&&zip4.PAWKr40<0.00043"
TCut cnaitimetight = "zip4.PAWKr40>0.00036&&zip4.PAWKr40<0.00040"


z->Draw("zip4.PAWKr40>>h(300,0,0.0035)",cnaibs[s]+c1trig+!crand)
z->Draw("zip4.PAWKr40>>hr(100,0,0.0035)",cnaibs[s]+c1trig+crand)
hr->SetLineColor(kRed)
h->Draw();hr->Draw("same")
//
float f_true_coin[] ={0.585624, 0.605176, 0.598126}
h->SetTitle(Form("%s: NaI trigger pulse timing (%0.3f);start time of NaI trigger pulse [s]",serlabel[s],f_true_coin[s]))
TLegend leg(0.3,.74,0.7,0.87);leg.SetFillColor(kWhite);
leg.AddEntry(h,"S104-triggered events","l")
leg.AddEntry(hr,"random-triggered events","l")
h->Draw();hr->Draw("same");leg.Draw();
c1->Print(Form("~/K100/run076/r76_Na22_%s_NaItrigtime.png",serlabel[s]))
c1->Print(Form("/data/chocula/fritts/run076/r76_Na22_%s_NaItrigtime.png",serlabel[s]))
//
h->Fit("pol0","","",0.0005,0.0031)
float accrate = h->GetFunction("pol0")->GetParameter(0)/h->GetBinWidth(1)*(0.00040-0.00036)
float frac_true_coin = 1.0/(1.0+accrate/z->GetEntries(cnaibs[s]+c1trig+!crand+cnaitimetight))
cout<<frac_true_coin<<endl
0.585624, 0.605176, 0.598126

z->SetAlias("xdel","1e6*(zip1.PEWKr20-0.5*(zip1.PCWKr20+zip1.PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(zip1.PDWKr20-zip1.PCWKr20))+9")
// note empirical offsets in xdel, ydel
z->SetAlias("afdel","1e6*(zip1.PFWKr20-zip1.PAWKr20)")
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")

TCut ctime[] = {"Entry$<52050", "Entry$<85e3", "Entry$>35000"}

//this cut gets rid of some high-energy pileup
TCut cintwid[] = {"zip1.PTWKf20-zip1.PTWKr20>0.00013+1.65e-7*PTINTkeV+4.35e-10*PTINTkeV**2", "zip1.PTWKf20-zip1.PTWKr20>0.00013+1.65e-7*PTINTkeV+4.35e-10*PTINTkeV**2", "zip1.PTWKf20-zip1.PTWKr20>0.00013+2.083e-7*PTINTkeV+2.9167e-10*PTINTkeV**2"}

z->Draw("phidel:PTINTkeV>>h(300,0,3000,200,-180,180)",!crand+cbsf[s]+ctime[s]+cintwid[s],"colz")
z->Draw("phidel:PTINTkeV>>hc(300,0,3000,200,-180,180)",!crand+cbsf[s]+ctime[s]+cintwid[s]+cnaitimetight,"colz")
//
z->Draw("phidel:PTINTkeV>>h(300,0,2300,200,-180,180)",!crand+cbsf[s]+ctime[s]+cintwid[s],"colz")
c1->SetLogz()
h->SetTitle(";energy [keV] (simple PTINT calibration);phi_del [degrees]")
h->Draw("colz")
c1->Print(Form("~/K100/run076/r76_Na22_%s_phivsenergy.png",serlabel[s]))
c1->Print(Form("/data/chocula/fritts/run076/r76_Na22_%s_phivsenergy.png",serlabel[s]))
//

TCut camphi[] = {"phidel>-10+0.051*PTINTkeV-1.3e-5*PTINTkeV**2 && phidel<10+0.0455*PTINTkeV-1.05e-5*PTINTkeV**2", "phidel>-12+0.04367*PTINTkeV-1.167e-5*PTINTkeV**2 && phidel<8+0.02725*PTINTkeV-3.75e-6*PTINTkeV**2", "phidel>-10+0.051*PTINTkeV-1.3e-5*PTINTkeV**2 && phidel<10+0.0455*PTINTkeV-1.05e-5*PTINTkeV**2"}


z->Draw("phidel:PTINTkeV>>h(100,0,3000,70,-180,180)",!crand+cbsf[s]+ctime[s]+cintwid[s],"lego")
z->Draw("phidel:PTINTkeV>>hc(100,0,3000,70,-180,180)",!crand+cbsf[s]+ctime[s]+cintwid[s]+cnaitimetight,"lego")


z->Draw("zip4.PAWKr40>>h(300,0,0.0035)",cnaibs[s]+c1trig+!crand+!camphi[s])
h->Fit("pol0","","",0.0005,0.0031)
float accrate = h->GetFunction("pol0")->GetParameter(0)/h->GetBinWidth(1)*(0.00040-0.00036)
float frac_true_coin = 1.0/(1.0+accrate/z->GetEntries(cnaibs[s]+c1trig+!crand+!camphi[s]+cnaitimetight))
cout<<frac_true_coin<<endl
0.590097, 0.604627, 0.603045 // no improvement with Am events removed

z->Draw("PTINTkeV>>h(20,0,2000)",!crand+cbsf[s]+ctime[s]+cintwid[s]+cnaibs[s]+c1trig)
z->Draw("PTINTkeV>>hc(20,0,2000)",!crand+cbsf[s]+ctime[s]+cintwid[s]+cnaibs[s]+c1trig+cnaitimetight)
hc->SetLineColor(kRed)
h->Scale(0.0285225)
h->Draw();hc->Draw("same")

z->Draw("PTOFkeV>>h(150,0,150)",!crand+cbsf[s]+ctime[s]+cintwid[s]+cnaibs[s]+c1trig+!cnaitimetight)
z->Draw("PTOFkeV>>hc(30,0,150)",!crand+cbsf[s]+ctime[s]+cintwid[s]+cnaibs[s]+c1trig+cnaitimetight)
hc->SetLineColor(kRed)
h->Scale(0.1)
hc->Draw();h->Draw("same")
//
TLegend leg(0.55,0.6,0.8,0.85);leg.SetFillColor(kWhite);
leg.AddEntry(h,"coincident","l")
leg.AddEntry(hc,"not coincident","l")
hc->SetTitle(";energy [keV] (PTOF calibration)")
hc->Draw();h->Draw("same");leg.Draw();
c1->Print(Form("~/K100/run076/r76_Na22_%s_spec_ncvc.png",serlabel[s]))
c1->Print(Form("/data/chocula/fritts/run076/r76_Na22_%s_spec_ncvc.png",serlabel[s]))
//

z->Draw("zip1.PTWKr20-zip4.PAWKr40:PTOFkeV",!crand+cbsf[s]+ctime[s]+cintwid[s]+!camphi[s]+cnaibs[s]+c1trig+cnaitimetight)

/////////////////////////
//check timing resolution
gStyle->SetOptFit()
TF1* fpeak = new TF1("fpeak","gaus(0)+pol0(3)",-100,100)
fpeak->SetParNames("Constant","Mean","Sigma","Background")
fpeak->SetParameters(40,40,10,10)

//z->Draw("(zip1.PTWKr50-zip4.PAWKr40)*1e6>>h(100,-100,100)",!crand+cbsf[s]+ctime[s]+cintwid[s]+!camphi[s]+cnaibs[s]+c1trig+cnaitimetight)
//h->Fit("gaus")
//cout<<h->GetFunction("gaus")->GetParameter(2)<<endl
//1.77935e-05, 7.73973e-06, 1.25295e-05

z->Draw("(zip1.PTWKr50-zip4.PAWKr40)*1e6>>h(100,-100,100)",!crand+cbsf[s]+ctime[s]+cintwid[s]+!camphi[s]+cnaibs[s]+c1trig)
h->Fit(fpeak,"","",-100,80)
cout<<h->GetFunction("fpeak")->GetParameter(2)<<endl
// 3.45719, 5.31002, 3.45164
h->SetTitle(Form("%s; SiHV_50%_time - NaI_40%_time [#mus]",serlabel[s]))
h->Draw()
c1->Print(Form("~/K100/run076/r76_Na22_%s_wktimediff.png",serlabel[s]))
c1->Print(Form("/data/chocula/fritts/run076/r76_Na22_%s_wktimediff.png",serlabel[s]))

//z->Draw("(zip1.PTOFdelay+0.4e-3-zip4.PAWKr40)*1e6>>h(100,-100,100)",!crand+cbsf[s]+ctime[s]+cintwid[s]+!camphi[s]+cnaibs[s]+c1trig+cnaitimetight)
//h->Fit("gaus")
//cout<<h->GetFunction("gaus")->GetParameter(2)<<endl
//1.24243e-05, 9.79557e-06, 1.12015e-05

z->Draw("(zip1.PTOFdelay+0.4e-3-zip4.PAWKr40)*1e6>>h(100,-100,100)",!crand+cbsf[s]+ctime[s]+cintwid[s]+!camphi[s]+cnaibs[s]+c1trig)
h->Fit(fpeak,"","",-100,80)
cout<<h->GetFunction("fpeak")->GetParameter(2)<<endl
// 7.57387, 5.79016, 5.66982
h->SetTitle(Form("%s; SiHV_OF_delay_time - NaI_40%_time [#mus]",serlabel[s]))
h->Draw()
c1->Print(Form("~/K100/run076/r76_Na22_%s_oftimediff.png",serlabel[s]))
c1->Print(Form("/data/chocula/fritts/run076/r76_Na22_%s_oftimediff.png",serlabel[s]))






z->Draw("zip1.PTINTall>>hall(200,0,0.025)",!crand+cbsf[s]+!cam[s])
z->Draw("zip1.PTINTall>>hcoin(200,0,0.025)",!crand+cbsf[s]+!cam[s]+cnaitime)
hall->Draw()
hcoin->Scale(20)
hcoin->SetLineColor(kRed)
hcoin->Draw("same")






TCut clow = "zip1.PTINTall>0&&zip1.PTINTall<0.45e-3"
TCut cmid = "zip1.PTINTall>0.45e-3&&zip1.PTINTall<0.005"
TCut chi = "zip1.PTINTall>0.005"

z->Draw("afdel:phidel>>h(300,-180,180,200,-30,30)",!crand+cbsf[s]+ctime[s]+clow,"colz")
z->Draw("afdel:phidel>>h(300,-180,180,200,-30,30)",!crand+cbsf[s]+ctime[s]+cmid,"colz")
z->Draw("afdel:phidel>>h(300,-180,180,200,-30,30)",!crand+cbsf[s]+ctime[s]+chi,"colz")


z->Draw("zip4.PAWKr40",!crand+cnaibs[s]+c1trig+cbsf[s]+ctime[s]+cmid)

// ok, above is showing coincidence peak in the Na-22 data and for "cmid" only

z->Draw("zip4.PAWKr40",!crand+cnaibs[s]+c1trig+cbsf[s]+ctime[s])
z->Draw("zip4.PAWKr40",!crand+cnaibs[s]+c1trig+cbsf[s]+!ctime[s])
// in the background series, a coincidence peak is seen at late time,
//  after the glitch, but not at early time. 
// in the Na-22 series the peak is present at all times

z->Draw("PTINTkeV",!crand+cbsf[s]+ctime[s])
// peak seen around 1400keV - probably not an accurate energy
// seen in both background and Na-22 data


z->Draw("zip4.PAWKr40:PTINTkeV",!crand+cnaibs[s]+c1trig+cbsf[s]+ctime[s])


z->Draw("PTINTkeV>>hall(200,0,3000)",!crand+cbsf[s]+ctime[s])
z->Draw("PTINTkeV>>hcoin(200,0,3000)",!crand+cbsf[s]+ctime[s]+cnaitime)
hall->Draw()
hcoin->Scale(20)
hcoin->SetLineColor(kRed)
hcoin->Draw("same")
// in Na-22 data not seeing any difference in energy spec with/without coincidence


z->Draw("PTINTkeV>>hall(200,0,3000)",!crand+cbsf[s]+ctime[s])
z->Draw("PTINTkeV>>hcoin(200,0,3000)",!crand+cbsf[s]+ctime[s]+cnaitimetight)
hall->Draw()
hcoin->Scale(35)
hcoin->SetLineColor(kRed)
hcoin->Draw("same")
// in Na-22 data still not seeing any difference in energy spec with/without coincidence

z->Draw("zip4.PAINTall",cnaibs[s])
float ntraces = z->GetEntries(crand+cnaibs[s])
float nnopulse = z->GetEntries(crand+cnaibs[s]+"zip4.PAINTall<0.06e-3")
float rate = -log(nnopulse/ntraces)/3.2768e-3
// no-source: rate = 56.1 Hz
// 48cm: rate = 83.1 Hz
// 40cm: rate = 64.6 Hz (why lower than 48cm??)

z->Draw("PTOFkeV>>hall(50,0,150)",!crand+cbsf[s]+cnaibs[s]+c1trig+ctime[s])
z->Draw("PTOFkeV>>hcoin(50,0,150)",!crand+cbsf[s]+cnaibs[s]+c1trig+ctime[s]+cnaitimetight)
hall->Draw()
hcoin->Scale(50)
hcoin->SetLineColor(kRed)
hcoin->Draw("same")
//FIXED: previously didn't have the NaI "quality" cuts. With those included, spectra look different!

z->Draw("PTINTkeV>>hall(50,0,2500)",!crand+cbsf[s]+cnaibs[s]+c1trig+ctime[s])
z->Draw("PTINTkeV>>hcoin(50,0,2500)",!crand+cbsf[s]+cnaibs[s]+c1trig+ctime[s]+cnaitimetight)
hall->Draw()
hcoin->Scale(50)
hcoin->SetLineColor(kRed)
hcoin->Draw("same")
//FIXED: previously didn't have the NaI "quality" cuts. With those included, spectra look different!


//
z->Draw("phidel:PTINTkeV>>h(150,0,2300,100,-180,180)",!crand+cbsf[s]+ctime[s]+cintwid[s],"colz")
// this shows the weird high-energy peak, showing up around 1400 keV according to PTINTkeV
// I'm beginning to think this is alphas
// in phidel it's not at the same place as the Am gammas, but it looks like there could be a continuous 
//   curve back to that spot
// could try looking in data taken with the source in a different spot (position 2?)

TFile f("~/K100/run076/spectra_Na22.root","update")
z->Draw("PTOFkeV>>hof(300,0,150)",!crand+cbsf[s]+ctime[s]+cintwid[s])
hof->SetName(Form("PTOFkeV_%s",serlabel[s]))
z->Draw("PTINTkeV>>hint(300,0,2000)",!crand+cbsf[s]+ctime[s]+cintwid[s])
hint->SetName(Form("PTINTkeV_%s",serlabel[s]))
hof->Write()
hint->Write()



/////////
// Sim //
/////////
gStyle->SetOptStat(0)
TChain *sim = new TChain("simtree")
//sim->Add("~/K100/k100SimBuild2/Na22_Si_10M.root") // 1274 keV
//sim->Add("~/K100/k100SimBuild2/Na22_511_Si_20M.root") // 511 keV, twice as many events
sim->Add("/data/chocula/fritts/k100sim/build/Na22_1274_Si_NaI_10M/*root") // 1274 keV
sim->Add("/data/chocula/fritts/k100sim/build/Na22_511_Si_NaI_20M/*root") // 511 keV, twice as many events
//
sim->SetAlias("keV_si","Sum$(D3*(DT==1))*1000")
sim->SetAlias("keV_10","Sum$(D3*(DT==2010))*1000")
sim->SetAlias("keV_11","Sum$(D3*(DT==2011))*1000")
sim->SetAlias("keV_12","Sum$(D3*(DT==2012))*1000")
// spectrum in Si detector
sim->Draw("keV_si>>h(300)","keV_si>0")
h->SetTitle("k100Sim: Na-22 gammas on Si detector;deposited energy [keV]")
h->Draw()
c1->Print("~/K100/run076/r76_sim_Na22_spec.png")
c1->Print("/data/chocula/fritts/run076/r76_sim_Na22_spec.png")
//
sim->GetEntries("keV_si>0") //(Long64_t)26581
sim->GetEntries("keV_si>0&&(keV_10>0||keV_11>0||keV_12>0)")
// (Long64_t)322 -- 1.2%
sim->GetEntries("keV_si>0&&(keV_10>950||keV_11>950||keV_12>950)")
// (Long64_t)33 -- 0.12%
sim->Draw("keV_si>>h(15,0,300)","keV_si>0&&(keV_10>950||keV_11>950||keV_12>950)")
h->SetTitle("k100Sim: spectrum in Si with energy>950keV in NaI;deposited energy [keV]")
h->Draw()
c1->Print("~/K100/run076/r76_sim_Na22_spec_coin.png")
c1->Print("/data/chocula/fritts/run076/r76_sim_Na22_spec_coin.png")
//
TCut c511 = "Entry$>166.91e3"
1.0*sim->GetEntries("keV_10>950||keV_11>950||keV_12>950"+!c511)/10e6*sim->GetEntries("keV_si>0"+c511)/sim->GetEntries("keV_si>0")//(const double)2.11382581543207554e-04
// -- 0.02%



///////////////////////////
// compare saved spectra /////////////////////////////

root ~/K100/run076/spectra_Na22.root
PTOFkeV_22Na_40cm->SetLineColor(kRed)
PTOFkeV_22Na_48cm->SetLineColor(kMagenta)
PTINTkeV_22Na_40cm->SetLineColor(kRed)
PTINTkeV_22Na_48cm->SetLineColor(kMagenta)
//normalize to 60keV peak count
float n60_ns = PTOFkeV_no_source->Integral(109,140)-PTOFkeV_no_source->Integral(93,108)-PTOFkeV_no_source->Integral(141,156)
float n60_40 = PTOFkeV_22Na_40cm->Integral(109,140)-PTOFkeV_22Na_40cm->Integral(93,108)-PTOFkeV_22Na_40cm->Integral(141,156)
float n60_48 = PTOFkeV_22Na_48cm->Integral(109,140)-PTOFkeV_22Na_48cm->Integral(93,108)-PTOFkeV_22Na_48cm->Integral(141,156)
PTOFkeV_22Na_40cm->Scale(n60_ns/n60_40)
PTOFkeV_22Na_48cm->Scale(n60_ns/n60_48)
PTINTkeV_22Na_40cm->Scale(n60_ns/n60_40)
PTINTkeV_22Na_48cm->Scale(n60_ns/n60_48)
PTOFkeV_no_source->GetYaxis()->SetRangeUser(0,610)
PTOFkeV_no_source->Draw();PTOFkeV_22Na_48cm->Draw("same");PTOFkeV_22Na_40cm->Draw("same");
//
PTINTkeV_no_source->GetYaxis()->SetRangeUser(0,2600)
PTINTkeV_no_source->SetTitle(";energy [keV] (simple PTINT calibration);counts normalized to 60keV peak")
TLegend leg(0.5,0.55,0.85,0.85);leg.SetFillColor(kWhite);
leg.AddEntry(PTINTkeV_22Na_40cm,"Na-22 source @ 67 cm","l")
leg.AddEntry(PTINTkeV_22Na_48cm,"Na-22 source @ 75 cm","l")
leg.AddEntry(PTINTkeV_no_source,"no external source","l")
PTINTkeV_no_source->Draw();PTINTkeV_22Na_48cm->Draw("same");PTINTkeV_22Na_40cm->Draw("same");leg.Draw();
c1->Print("~/K100/run076/r76_Na22_speccompare.png")
c1->Print("/data/chocula/fritts/run076/r76_Na22_speccompare.png")



