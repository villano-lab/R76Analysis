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
07220617_1209/ : muon setup
07220617_1305/ : muon setup, NaI-triggered

/////////////
// 0V data //
/////////////

// choose a series:
int s=3
gStyle->SetOptStat(0)
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser[] = {"07220531_0810", "07220531_1214", "07220601_1310", "07220617_1209", "07220617_1305"} //<<<<<<
char* serlabel[] = {"no_source","22Na_48cm","22Na_40cm","muons","muons_NaI-triggered"}
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
TCut cbsf[] = {"zip1.PFbs<2780", "zip1.PFbs<2730", "zip1.PFbs<2680", "zip1.PFbs<3050", "zip1.PFbs<3050"} //<<<<<<

//Am-cut
TCut cam[] = {"zip1.PDOFamps/zip1.PTOFamps>0.215&&zip1.PDOFamps/zip1.PTOFamps<0.265", "zip1.PDOFamps/zip1.PTOFamps>0.235&&zip1.PDOFamps/zip1.PTOFamps<0.31", "zip1.PDOFamps/zip1.PTOFamps>0.22&&zip1.PDOFamps/zip1.PTOFamps<0.31", "zip1.PDOFamps/zip1.PTOFamps>0.27&&zip1.PDOFamps/zip1.PTOFamps<0.33", "zip1.PDOFamps/zip1.PTOFamps>0.27&&zip1.PDOFamps/zip1.PTOFamps<0.33"}

//correct amplitude to baseline
char* basecorrect[] = {"zip1.PTOFamps/(4.668901e+00+-6.166220e-04*PDbs)", "zip1.PTOFamps/(4.141528e+00+-5.315615e-04*zip1.PDbs)", "zip1.PTOFamps/(3.089520e+00+-7.914847e-04*zip1.PDbs)", "zip1.PTOFamps/(3.230769e+00+-7.692308e-04*zip1.PDbs)", "zip1.PTOFamps/(3.230769e+00+-7.692308e-04*zip1.PDbs)"}
z->SetAlias("PTOF1amps",basecorrect[s]) //<<<<<<

//calibrate to 14keV line
float amp14[] = {3.70667e-07, 3.46075e-07, 3.71292e-07, 2.60033e-07, 2.60033e-07} //<<<<<<
z->SetAlias("PTOFkeV_l",Form("PTOF1amps*13.95/%e",amp14[s]))

//nonlinear energy correction
float l60[] = {54.0526,50.8580,51.2514,53.4266,53.4266}
z->SetAlias("PTOFkeV",Form("(59.54-13.95)/(%f-13.95)*(PTOFkeV_l-13.95)+13.95",l60[s]))

//calibrate pT_integral 
float l60int[]={3.56721e-04,0.000349538,0.000356507,0.000287587,0.000287587}
z->SetAlias("PTINTkeV",Form("PTINTall*59.54/%e",l60int[s]))

z->SetAlias("xdel","1e6*(zip1.PEWKr20-0.5*(zip1.PCWKr20+zip1.PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(zip1.PDWKr20-zip1.PCWKr20))+9")
// note empirical offsets in xdel, ydel
z->SetAlias("afdel","1e6*(zip1.PFWKr20-zip1.PAWKr20)")
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")

TCut camlo = "PTOFkeV>12&&PTOFkeV<25"
TCut cam60 = "PTOFkeV>50&&PTOFkeV<75"
z->Draw("PTOFkeV>>h(300,0,100)",!crand+cbsf[s])
z->SetLineColor(kRed)
z->Draw("PTOFkeV>>hsel(300,0,100)",!crand+cbsf[s]+camlo,"same")
c1->Print("~/K100/run076/r76_rrecon_spec_low.png")
c1->Print("/data/chocula/fritts/run076/r76_rrecon_spec_low.png")

z->Draw("ydel:xdel>>h(300,-60,60,200,-40,40)",!crand+cbsf[s]+camlo,"colz")
TLine l1(0,0,30.*cos(7.57/180*3.14159),30.*sin(7.57/180*3.14159))
l1.Draw("same")
TLine l2(0,0,30.*cos(-0.212/180*3.14159),30.*sin(-0.212/180*3.14159))
l2.Draw("same")
TLine l3(0,0,30.*cos(-9.94/180*3.14159),30.*sin(-9.94/180*3.14159))
l3.Draw("same")
TLine l4(0,0,30.*cos(17.29/180*3.14159),30.*sin(17.29/180*3.14159))
l4.Draw("same")
l1.SetLineColor(kGreen);l2.SetLineColor(kGreen);
l3.SetLineColor(kRed);l4.SetLineColor(kRed);
l1.Draw("same");l2.Draw("same");l3.Draw("same");l4.Draw("same");
c1->Print("~/K100/run076/r76_rrecon_phiselect.png")
c1->Print("/data/chocula/fritts/run076/r76_rrecon_phiselect.png")

z->SetLineColor(kBlue)
z->Draw("phidel>>h(300,-50,50)",!crand+cbsf[s]+camlo)
h->Fit("gaus")
c1->Print("~/K100/run076/r76_rrecon_phidist.png")
c1->Print("/data/chocula/fritts/run076/r76_rrecon_phidist.png")

TCut cphitight = "phidel>-0.212&&phidel<7.57"
TCut cphiloose = "phidel>-9.94&&phidel<17.29"

gStyle->SetOptFit()

z->Draw("1e6*(zip1.PFWKr20-zip1.PAWKr20)>>hb(300,-30,30)",!crand+cbsf[s]+camlo+!cphiloose)
z->Draw("1e6*(zip1.PFWKr20-zip1.PAWKr20)>>hs(300,-30,30)",!crand+cbsf[s]+camlo+cphitight)
hb->SetLineColor(kRed)
hs->SetTitle("12-25 keV;pF 20%-time minus pA 20%-time")
TLegend leg1(0.52,0.6,0.88,0.8);leg1.SetFillColor(kWhite);
leg1.AddEntry(hs,"Am-241 events (based on phi_del)","l")
leg1.AddEntry(hb,"background events","l")
hs->Draw();hb->Draw("same");leg1.Draw();
c1->Print("~/K100/run076/r76_rrecon_pf-pa-distrib_withspike.png")
c1->Print("/data/chocula/fritts/run076/r76_rrecon_pf-pa-distrib_withspike.png")

TCut cspike = "1e6*(zip1.PFWKr20-zip1.PAWKr20)>-3&&1e6*(zip1.PFWKr20-zip1.PAWKr20)<-2"
z->Draw("ydel:xdel>>h(300,-30,30,200,-20,20)",!crand+cbsf[s]+camlo,"colz")
TGraph g
g.SetPoint(0,5,3.9)
g.SetPoint(1,5,3)
g.SetPoint(2,6.4,3)
g.SetPoint(3,6.4,3.9)
g.SetPoint(4,5,3.9)
g.SetLineColor(kRed)
g.Draw("l")


TCut coutcrop = "xdel>5&&xdel<6.4&&ydel<3.9&&ydel>3"
z->Draw("1e6*(zip1.PFWKr20-zip1.PAWKr20)>>hs(300,-30,30)",!crand+cbsf[s]+camlo+!coutcrop+cphitight)
z->Draw("1e6*(zip1.PFWKr20-zip1.PAWKr20)>>hb(300,-30,30)",!crand+cbsf[s]+camlo+!coutcrop+!cphiloose)
hs->Draw()
hb->SetLineColor(kRed)
hb->Draw("same")
hs->SetTitle("12-25 keV;pF 20%-time minus pA 20%-time")
hs->Draw();hb->Draw("same");leg1.Draw();
c1->Print("~/K100/run076/r76_rrecon_pf-pa-distrib.png")
c1->Print("/data/chocula/fritts/run076/r76_rrecon_pf-pa-distrib.png")

hs->Fit("gaus")
float amphi_deg = hs->GetFunction("gaus")->GetParameter(1)
float amphi_deg_err = hs->GetFunction("gaus")->GetParError(1)

float amfadel = hs->GetFunction("gaus")->GetParameter(1)
int ambin = hb->FindBin(amfadel)
float amr_mm_low = 50.*sqrt(hb->Integral(1,ambin-1)/hb->Integral(1,300))
float amr_mm_high = 50.*sqrt(hb->Integral(1,ambin)/hb->Integral(1,300))
float amr_mm = 0.5*(amr_mm_low+amr_mm_high)
float amr_mm_err = 0.5*(amr_mm_high-amr_mm_low)

cout<<"phi of source = "<<amphi_deg<<" +/- "<<amphi_deg_err<<" degrees"<<endl 
// phi of source = -1.89251 +/- 0.0195322 degrees
// 60keV: phi of source = -2.52222 +/- 0.0317306 degrees

cout<<"r of source = "<<amr_mm<<" +/- "<<amr_mm_err<<" mm"<<endl 
// r of source = 22.0671 +/- 0.206736 mm
// 60keV: r of source = 23.7012 +/- 0.158923 mm

z->Draw("PTwid>>hwids",!crand+cbsf[s]+camlo+!coutcrop+cphitight)
z->Draw("PTwid>>hwidb",!crand+cbsf[s]+camlo+!coutcrop+!cphiloose)
