// Am peaks at 59.54 keV
//   60keV (36%), 18keV (18%), 14keV (13%) according to one reference
//   13.9 (37%), 26.3 (2.3%), 59.54 (36%) from nudat
//   but just google image search americium spectrum, lots of examples of linesat 13.95, 17.74, 20.8, 26.4
//   try searching e.g. Am-241 "17.74 keV". Some references have 22.12 instead of 20.8, this seems closer to our data

// {0, 13.95, 17.74, 20.8, 26.4, 59.54}

/////////////
// setup   //
/////////////

gStyle->SetOptStat(0)
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"

float amp14 = 4.03878e-07
float wid1 = 113.545; 
float wid2 = 146.356; 
float f2 = 0.832213;
float pk = 0.085; 
float hv = 195.6 //<<<<<<
float gntl = 1.+hv/3.8
float tsechv = 3995

float tsecsim = 100e6/(1e-6*3.7e10)
float bf_60 = 0.3592
// source: http://www.nucleide.org/DDEP_WG/Nuclides/Am-241_tables.pdf
float bf_14 = bf_60*0.353/2.68

//for plotting
TLine l14(pk,40,pk,160); 
int xpix = 500, ypix = 500; //for position-dependent stuff
TCanvas *csquare = new TCanvas("csquare","csquare",xpix*5/4+4,ypix*5/4+24);
TGraph g

//cuts

TCut crand = "EventCategory"
TCut centrytime = "Entry$<40e3" //<<<<<<
TCut c14 = "PTOFkeV>12&&PTOFkeV<16"
TCut c18 = "PTOFkeV>16.6&&PTOFkeV<18.5" //<<<<<<
//TCut c60 = "PTOFkeV>50&&PTOFkeV<56" //<<<<<<
TCut c60 = "PTOFkeV>47&&PTOFkeV<52" //<<<<<<

TCut cofint_strict = "PTINTall/PTOFkeV>0&&PTINTall/PTOFkeV<0.0005"
TCut cofint_vstrict = "PTINTall/PTOFkeV>0.0002&&PTINTall/PTOFkeV<0.0005"
TCut caml = "phidel>-30&&phidel<20&&afdel<7"
TCut camdel = "(afdel+3.5)**2/5.5**2+(phidel+1)**2/20**2<1"
TCut cafbound = "20.**2>(phidel+1)**2"

TCut cbsf0 = "PFbs<2800" //<<<<<<
//Am-cut
TCut cam0 = "PDOFamps/PTOFamps>0.205&&PDOFamps/PTOFamps<0.225" //<<<<<<
TCut c14_0 = "PTOFkeV>12&&PTOFkeV<16"
TCut c18_0 = "PTOFkeV>16.6&&PTOFkeV<18.5" //<<<<<<
TCut c60_0 = "PTOFkeV>50&&PTOFkeV<56" //<<<<<<

TCut cofint0 = "PTINTall/PTOFwckeV>0&&PTINTall/PTOFwckeV<12e-6"
TCut cam0f = "PDOFamps/PTOFamps<PTOFwckeV*0.00025+0.255&&PDOFamps/PTOFamps>-8.333e-5*PTOFwckeV+0.195"

//////////////
// SIM DATA //
//////////////
TChain* sim = new TChain("simtree")
sim->Add("/data/chocula/fritts/data/k100sim/Am60Si_100M.root")
sim->SetAlias("keV","Sum$(D3)*1000")
//
//
TChain* sim14 = new TChain("simtree")
sim14->Add("/data/chocula/fritts/data/k100sim/Am14Si_10M.root")
sim14->SetAlias("keV","Sum$(D3)*1000")

/////////////
// 0V data //
/////////////

char* ser = "07220510_0917" //<<<<<<
//char* ser = "07220513_0853" //<<<<<<
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)
//z->SetLineColor(kRed)
z->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")

TCut cam = "PDOFamps/PTOFamps>0.205&&PDOFamps/PTOFamps<0.225" //<<<<<<
TCut cbsf = "PFbs<2800" //<<<<<<

z->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
    
//initial baseline cut
//z->Draw("PFbs:PFINTall",crand)

//identify peaks using partition
//z->Draw("PDOFamps/PTOFamps:PTOFamps>>h(300,0,2e-6,200,0,0.5)",!crand+cbsf,"colz")
//Am-cut
//TCut cam = "PDOFamps/PTOFamps>0.28&&PDOFamps/PTOFamps<0.36" //<<<<<<
//
// Figure
//h->SetTitle(Form("%s 0 Volt;PTOFamps;PDOFamps/PTOFamps",ser))
//h->Draw("colz");c1->SetLogz();
//c1->Print("~/K100/run076/r76_0V_pdfracvspt.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_pdfracvspt.png")
//

//correct amplitude to baseline
//z->SetMarkerColor(kMagenta);z->SetMarkerStyle(6);
//z->Draw("PTOFamps:PDbs",!crand+cam)
//TGraph g
//g.SetPoint(0,2660,0.39e-6) //<<<<<<
//g.Draw("l*")
//g.SetPoint(1,2800,0.36e-6) //<<<<<<
//g.Fit("pol1")
//z->SetAlias("PTOF1amps",Form("PTOFamps/(%e+%e*PDbs)",g.GetFunction("pol1")->GetParameter(0)/g.GetY()[0],g.GetFunction("pol1")->GetParameter(1)/g.GetY()[0]))
//cout<<endl<<"//z->SetAlias(\"PTOF1amps\",\""<<Form("PTOFamps/(%e+%e*PDbs)",g.GetFunction("pol1")->GetParameter(0)/g.GetY()[0],g.GetFunction("pol1")->GetParameter(1)/g.GetY()[0])<<"\")"<<endl<<endl
//z->SetAlias("PTOF1amps","PTOFamps/(2.461538e+00+-5.494505e-04*PDbs)") //<<<<<<
z->SetAlias("PTOF1amps","PTOFamps/(4.518152e+00+-1.320132e-03*PDbs)")
//
// Figure
//c1->Print("~/K100/run076/r76_0V_bscorrect.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_bscorrect.png")


//check for variation in time
//z->Draw("PTOF1amps:Entry$",!crand+cbsf+cam)
//TCut centrytime = "Entry$>0" //<<<<<<
//
// Figure
//TGraph g; g.SetPoint(0,40e3,0); g.SetPoint(1,40e3,1); g.Draw("l");
//c1->Print("~/K100/run076/r76_0V_timevar.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_timevar.png")

//calibrate to 14keV line
//z->Draw("PTOF1amps>>h(300,0.2e-6,1e-6)",!crand+cbsf+cam+centrytime)
//h->Fit("gaus","","",0.39e-6,0.42e-6) //<<<<<<
//float amp14 = h->GetFunction("gaus")->GetParameter(1)
//cout<<endl<<"//float amp14 = "<<amp14<<endl<<endl
//float amp14 = 3.02761e-07
z->SetAlias("PTOFkeV",Form("PTOF1amps*13.95/%e",amp14))
//
// Figure
//z->Draw("PTOF1amps>>hl(300,0.2e-6,1e-6)",!crand+cbsf+cam+!centrytime)
//hl->SetLineColor(kGray)
//hl->SetTitle(Form("%s 0 Volt;PTOFamps;counts/bin",ser))
//TLegend leg(0.6,0.7,0.88,0.88);leg.SetFillColor(kWhite);
//leg.AddEntry(h,"Early in series","l")
//leg.AddEntry(hl,"Late in series","l")
//hl->Draw();h->Draw("same");leg.Draw();
//c1->Print("~/K100/run076/r76_0V_ofspec_earlylate.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_ofspec_earlylate.png")
//h->SetTitle(Form("%s 0 Volt;PTOFamps;counts/bin",ser))
//h->Draw()
//c1->Print("~/K100/run076/r76_0V_ofspec_fit14.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_ofspec_fit14.png")
//
// Figure
//z->Draw("PTOF1amps>>h(300,0.2e-6,2e-6)",!crand+cbsf+cam+centrytime)
//z->Draw("PTOFamps>>h0(300,0.2e-6,2e-6)",!crand+cbsf+cam+centrytime,"same")
//h0->SetLineColor(kGray)
//h->SetTitle(Form("%s 0 Volt;PTOFamps;counts/bin",ser))
//TLegend leg(0.5,0.7,0.88,0.88);leg.SetFillColor(kWhite);
//leg.AddEntry(h0,"Without baseline correction","l")
//leg.AddEntry(h,"With baseline correction","l")
//h->Draw();h0->Draw("same");h->Draw("same");leg.Draw();
//c1->Print("~/K100/run076/r76_0V_ofspec_bscorr_w.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_ofspec_bscorr_w.png")

//correct energy to pulse width 
//z->Draw("PTwid:PTOFkeV",!crand+cbsf+centrytime+cam)
//
//z->Draw("PTOFkeV>>h(300,0,80)",!crand+cbsf+cam+centrytime)

//z->Draw("PTOFkeV>>h18(50)",!crand+cbsf+cam+centrytime+c18)
//h18->Fit("gaus")
//z->Draw("PTOFkeV>>h60(50)",!crand+cbsf+cam+centrytime+c60)
//h60->Fit("gaus")
//z->Draw("PTwid>>hw14(200,0,500)",!crand+cbsf+cam+centrytime+c14)
//hw14->Fit("gaus")
//z->Draw("PTwid>>hw18(200,0,500)",!crand+cbsf+cam+centrytime+c18)
//hw18->Fit("gaus")
//z->Draw("PTwid>>hw60(200,0,500)",!crand+cbsf+cam+centrytime+c60)
//hw60->Fit("gaus")
//TGraph g
//g.SetPoint(0,hw14->GetFunction("gaus")->GetParameter(1),1)
////g.SetPoint(1,hw18->GetFunction("gaus")->GetParameter(1),h18->GetFunction("gaus")->GetParameter(1)/17.74)
//g.SetPoint(1,hw60->GetFunction("gaus")->GetParameter(1),h60->GetFunction("gaus")->GetParameter(1)/59.54)
//g.Draw("a*")
//g.Fit("pol1")
//cout<<endl<<"//float wid1 = "<<g.GetX()[0]<<"; float wid2 = "<<g.GetX()[1]<<"; float f2 = "<<g.GetY()[1]<<";"<<endl<<endl
//float wid1 = 111.177; float wid2 = 146.052; float f2 = 0.891119; //<<<<<<
//
// figure
//z->Draw("PTwid:PTOFkeV",!crand+cbsf+centrytime+cam)
//float x[]={13.95,13.95,17.74,17.74,20.8,20.8,26.4,26.4,59.54,59.54};
//float y[]={0,1000,1000,0,0,1000,1000,0,0,1000,1000,0,0,1000,1000,0}
//TGraph g(10,x,y); g.SetLineColor(kGreen);
//g.Draw("l")
//c1->Print("~/K100/run076/r76_0V_widthvsenergy.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_widthvsenergy.png")

// figure
//z->Draw(Form("(59.54-13.95)/(%f-13.95)*(PTOFkeV-13.95)+13.95>>hc(300,0,80)",h60->GetFunction("gaus")->GetParameter(1)),!crand+cbsf+cam+centrytime)
//float x[]={13.95,13.95,17.74,17.74,59.54,59.54};
//float y[]={-10,1000,1000,-10,-10,1000,1000,-10,-10,1000,1000,-10}
//TGraph g(6,x,y); g.SetLineColor(kGreen);
//hc->SetTitle(Form("%s 0 Volt;PTOFkeV (width-corrected);counts/bin",ser))
//hc->Draw();g.Draw("l");
//c1->Print("~/K100/run076/r76_0V_espec_widcorr.png")
//c1->Print("/data/chocula/fritts/run076/r76_0V_espec_widcorr.png")

// check linearity of PTINTall
z->Draw("PTINTall*1000>>h(100,0,0.5)",!crand+cbsf+centrytime+!cam)
h->SetTitle(Form("%s 0 Volt;PTINTall*1000;counts/bin",ser))
h->Draw()
z->Draw("PTINTall*1000",!crand+cbsf+centrytime+!cam+c14,"same")
z->Draw("PTINTall*1000",!crand+cbsf+centrytime+!cam+c60,"same")
l14.Draw("same"); TLine l60(pk*59.54/13.95,40,pk*59.54/13.95,160); l60.Draw("same");
c1->Print("~/K100/run076/r76_0V_intspec.png")
c1->Print("/data/chocula/fritts/run076/r76_0V_intspec.png")

// look at position dependent stuff    
// note empirical offsets in xdel, ydel
z->Draw("ydel:xdel>>h(200,-30,30,200,-30,30)",!crand+cbsf+"PTOFkeV>12&&PTOFkeV<20","colz")
h->SetTitle(Form("%s 0 Volt, 12-20 keV;inner-ring x_del [#mus];inner-ring y_del [#mus]",ser))
h->Draw("colz")
csquare->Print("~/K100/run076/r76_0V_xydel_12-20keV.png")
csquare->Print("/data/chocula/fritts/run076/r76_0V_xydel_12-20keV.png")

c1->cd()
z->Draw("PTwid:180./3.14159*atan2(ydel,xdel)>>h(200,-180,180,150,80,200)",!crand+cbsf+"PTOFkeV>12&&PTOFkeV<20","colz")
h->SetTitle(Form("%s 0 Volt, 12-20 keV;inner-ring phi_del [degrees];pT 40%-width [#mus]",ser))
h->Draw("colz")
c1->Print("~/K100/run076/r76_0V_widvsphi_12-20keV.png")
c1->Print("/data/chocula/fritts/run076/r76_0V_widvsphi_12-20keV.png")
//
z->Draw("PTwid:180./3.14159*atan2(ydel,xdel)>>h(200,-180,180,150,80,200)",!crand+cbsf+"PTOFkeV>45&&PTOFkeV<60","colz")
h->SetTitle(Form("%s 0 Volt, 50-70 keV;inner-ring phi_del [degrees];pT 40%-width [#mus]",ser))
h->Draw("colz")
c1->Print("~/K100/run076/r76_0V_widvsphi_50-70keV.png")
c1->Print("/data/chocula/fritts/run076/r76_0V_widvsphi_50-70keV.png")
z->Draw("1e6*(PFWKr20-PAWKr20):sqrt(xdel**2+ydel**2)>>h(200,0,30,200,-30,30)",!crand+cbsf+"PTOFkeV>12&&PTOFkeV<20","colz")

z->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")

z->Draw("afdel:180./3.14159*atan2(ydel,xdel)>>h(200,-180,180,150,-40,40)",!crand+cbsf+"PTOFkeV>4&&PTOFkeV<70","colz")

z->Draw("(PAOFamps-PFOFamps)/(PAOFamps+PTOFamps):180./3.14159*atan2(ydel,xdel)>>h(200,-180,180,150,-0.3,0.3)",!crand+cbsf+"PTOFkeV>12&&PTOFkeV<70","colz")

z->Draw("1e6*(PTWKr40-PTWKr10):PTOFkeV>>h(300,0,70,200,5,20)",!crand+cbsf,"colz")
// Weird shape for 60keV, higher amplitude for slower-risetime events
// suggests lower amplitude for near-surface events

/////////////
// HV data //
/////////////

//07220510_1115	Side2	-195.6
char* ser = "07220510_1115" //<<<<<<
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)
z->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")

//correct amplitude to baseline
z->SetAlias("PTOF1amps","PTOFamps/(2.461538e+00+-5.494505e-04*PDbs)") //<<<<<< 2660-2800
z->SetAlias("PTOF01amps","PTOFamps0/(2.461538e+00+-5.494505e-04*PDbs)") //<<<<<< 2660-2800
//z->Draw("PDbs")
// note that PDbs extends a lot higher here than 0V
// for the bulk this correction is as large as 1/0.70

//calibrate based on 0V 14keV line and NTL gain
z->SetAlias("PTOF1keV",Form("PTOF1amps*13.95/%e",amp14*gntl))
z->SetAlias("PTOF01keV",Form("PTOF01amps*13.95/%e",amp14*gntl))

//refine baseline cut
//z->Draw("PDINTall/PTINTall:PTwid>>h(300,0,1000,200,0,0.5)",!crand,"colz")
// note: correcting PTwid to baseline creates a very sharp energy quantity
//z->SetAlias("widvarbs","1+(PDbs-2800)*3.05e-4");z->SetAlias("PTwidc","PTwid/widvarbs");.

//correct energy to pulse width 
float wid1 = 111.177; float wid2 = 146.052; float f2 = 0.891119; //<<<<<<<
//z->Draw("PTwid:PTOF1keV>>h(150,0,2,100,0,1000)",!crand+cbsf,"colz")
g.SetPoint(0,0,140)
//g.Draw("*")
g.SetPoint(1,0.4,150)
g.SetPoint(2,1,190)
g.SetPoint(3,2,370)
g.Fit("pol2")
z->SetAlias("ewid",Form("%f+%f*PTOF1keV+%f*PTOF1keV**2",g.GetFunction("pol2")->GetParameter(0),g.GetFunction("pol2")->GetParameter(1),g.GetFunction("pol2")->GetParameter(2)))
z->SetAlias("ewid0",Form("%f+%f*PTOF01keV+%f*PTOF01keV**2",g.GetFunction("pol2")->GetParameter(0),g.GetFunction("pol2")->GetParameter(1),g.GetFunction("pol2")->GetParameter(2)))
z->SetAlias("PTOFkeV",Form("PTOF1keV/(1+%f*(ewid-%f))",(f2-1.)/(wid2-wid1),wid1))
z->SetAlias("PTOF0keV",Form("PTOF01keV/(1+%f*(ewid0-%f))",(f2-1.)/(wid2-wid1),wid1))
//
// Figure
//c1->Print("~/K100/run076/r76_m196V_widvsamp.png")
//c1->Print("/data/chocula/fritts/run076/r76_m196V_widvsamp.png")

//check noise width 
//z->Draw("PTOF0keV>>h(200,-0.1,0.1)",crand+cbsf)
//h->Fit("gaus")
//cout<<"// sigma = "<<Form("%0.3f",1000.*h->GetFunction("gaus")->GetParameter(2))<<" +/- "<<Form("%0.3f",1000.*h->GetFunction("gaus")->GetParError(2))<<" eVee at "<<hv<<" V"<<endl
// sigma = 4.679 +/- 0.033 eVee at 195.6 V
//
// figure
//gStyle->SetOptFit(1)
//z->Draw("PTOF0keV>>h(200,-0.1,0.1)",crand+cbsf)
//h->Fit("gaus")
//c1->Print("~/K100/run076/r76_m196V_noisewidth.png")
//c1->Print("/data/chocula/fritts/run076/r76_m196V_noisewidth.png")

//compare low-energy spectrum to randoms
//z->Draw("PTOFkeV>>h(200,0,2)",!crand+cbsf)
//z->Draw("PTOFkeV>>hn(200,0,2)",crand+cbsf)
//hn->SetLineColor(kRed)
//h->Draw();hn->Draw("same");
//
// figure
//c1->Print("~/K100/run076/r76_m196V_spec_withpileup.png")
//c1->Print("/data/chocula/fritts/run076/r76_m196V_spec_withpileup.png")

//ANALYSIS NOTES
// there are a lot of events triggering below 0.8 K
// checked some events with BatViewer around 0.1 K - many are on downward slopes or have a late pileup event

//develop a int-OF agreement cut
//z->Draw("PTINTall:PTOFkeV",!crand+cbsf+"PTOFkeV>0&&PTOFkeV<4")
//z->Draw("PTINTall/PTOFkeV:PTOFkeV",!crand+cbsf+"PTOFkeV>0&&PTOFkeV<4&&PTINTall/PTOFkeV<0.04&&PTINTall/PTOFkeV>-0.03")
//
// figure
//c1->Print("~/K100/run076/r76_m196V_intvsof.png")
//c1->Print("/data/chocula/fritts/run076/r76_m196V_intvsof.png")
//c1->Print("~/K100/run076/r76_m196V_intvsof_z.png")
//c1->Print("/data/chocula/fritts/run076/r76_m196V_intvsof_z.png")

//z->Draw("PTOFkeV>>h(200,0,2)",!crand+cbsf+cofint_vstrict)
// figure
//c1->Print("~/K100/run076/r76_m196V_spec.png")
//c1->Print("/data/chocula/fritts/run076/r76_m196V_spec.png")

z->SetAlias("bsdiff","PAbspost+PBbspost+PCbspost+PDbspost+PEbspost+PFbspost-PAbs-PBbs-PCbs-PDbs-PEbs-PFbs")
// Position studies
z->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
z->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")

z->SetAlias("afboundp","sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
z->SetAlias("afboundm","-sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")

csquare->cd()
z->Draw("ydel:xdel>>h(200,-30,30,200,-30,30)",!crand+cbsf+cofint_vstrict+"PTOFkeV>0&&PTOFkeV<2","colz")
h->SetTitle(Form("%s -196 Volt, 0-2 keV;inner-ring x_del [#mus];inner-ring y_del [#mus]",ser))
h->Draw("colz")
csquare->Print("~/K100/run076/r76_HV_xydel_0-2keV.png")
csquare->Print("/data/chocula/fritts/run076/r76_HV_xydel_0-2keV.png")

c1->cd()
z->Draw("afdel:phidel>>h(300,-180,180,200,-30,30)",!crand+cbsf+cofint_vstrict+"PTOFkeV>0&&PTOFkeV<2","colz")
h->SetTitle(Form("%s -196 Volt, 0-2 keV;inner-ring phi_del [degrees];pF vs pA delay [#mus]",ser))
h->Draw("colz")
c1->Print("~/K100/run076/r76_HV_rphidel_0-2keV.png")
c1->Print("/data/chocula/fritts/run076/r76_HV_rphidel_0-2keV.png")
    
z->Draw("PTOFkeV>>h(100,0,2)",!crand+cbsf+cofint_vstrict+caml)
z->Draw("PTOFkeV>>hb(100,0,2)",!crand+cbsf+cofint_vstrict+!caml,"same")
h->SetLineColor(kRed)
h->SetTitle(Form("%s -196 Volt;corrected pTOF [keVee];counts / 2eVee",ser))
TLegend leg(0.5,0.7,0.85,0.85);leg.SetFillColor(kWhite);
leg.AddEntry(hb,"background","l")
leg.AddEntry(h,"source","l")
h->Draw();hb->Draw("same");leg.Draw();
c1->Print("~/K100/run076/r76_HV_spec_0-2keV_Amvsbg.png")
c1->Print("/data/chocula/fritts/run076/r76_HV_spec_0-2keV_Amvsbg.png")
//
cout << endl << z->GetEntries(!crand+cbsf+cofint_vstrict+!caml+"PTOFkeV>0&&PTOFkeV<=1") << " bg 0-1" << endl << z->GetEntries(!crand+cbsf+cofint_vstrict+!caml+"PTOFkeV>1&&PTOFkeV<=2") << " bg 1-2" << endl << z->GetEntries(!crand+cbsf+cofint_vstrict+caml+"PTOFkeV>0&&PTOFkeV<=1") << " Am 0-1" << endl << z->GetEntries(!crand+cbsf+cofint_vstrict+caml+"PTOFkeV>1&&PTOFkeV<=2") << " Am 1-2" << endl << endl
    
/* //when i found these they weren't commented out but i can't see how they would be anything but comments
424 bg 0-1 = 0.11 Hz
941 bg 1-2 = 0.24 Hz
1766 Am 0-1 = 0.44 Hz 
472 Am 1-2 = 0.12 Hz
3995 seconds
//
from sim:
14keV peak rate = 2.69 Hz * bf (0.27 Hz)
14keV 0-2keV rate = 0.0014 Hz * bf
60keV peak rate = 6.50 Hz * bf (3.64 Hz)
60keV 0-2keV rate < 0.00048 Hz * bf
60keV 0-12keV rate < 0.051 Hz * bf
//
from 0V:
60keV: 2.71 Hz; empirical bf = 0.56
14keV: 0.49 Hz; empirical bf = 0.10
12-30keV: 2.15 Hz
//
so: 60keV produces extremely few events < 2keV according to Geant
14keV produces more. If we assume everything from 12-30 produces similar
  amounts, then it's around 0.0066 * bf Hz
  that's still 100x smaller than what is observed 
*/

///////////////////////
// 0V data relabeled //
///////////////////////
char* ser0 = "07220510_0917" //<<<<<<
TChain *e0 = new TChain("rqDir/eventTree")
TChain *z0 = new TChain("rqDir/zip1")
e0->Add(Form("%s/%s/umn*root",datapath,ser0));
z0->Add(Form("%s/%s/umn*root",datapath,ser0));
z0->AddFriend(e0)
z0->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
z0->SetAlias("PTOF1amps","PTOFamps/(4.518152e+00+-1.320132e-03*PDbs)")
TCut centrytime = "Entry$<40e3" //<<<<<<
//z0->Draw("PTOF1amps>>h(300,0.2e-6,1e-6)",!crand+cbsf0+cam0+centrytime)
//h->Fit("gaus","","",0.39e-6,0.42e-6) //<<<<<<
float amp14 = 4.03878e-07
z0->SetAlias("PTOFkeV",Form("PTOF1amps*13.95/%e",amp14))
float wid1 = 113.545; float wid2 = 146.356; float f2 = 0.832213;
// look at position dependent stuff
z0->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
z0->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
z0->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
z0->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
TCut camdel = "(afdel+3.5)**2/5.5**2+(phidel+1)**2/20**2<1"
//
z0->SetAlias("afboundp","sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
z0->SetAlias("afboundm","-sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
TCut cafbound = "20.**2>(phidel+1)**2"
//
z0->Draw("afdel:phidel>>h(200,-180,180,150,-40,40)",!crand+cbsf0+"PTOFkeV>12&&PTOFkeV<30","colz")
z0->Draw("afboundp:phidel",cafbound,"same")
z0->Draw("afboundm:phidel",cafbound,"same")
//
h->SetTitle(Form("%s 0 Volt, 12-30 keV;inner-ring phi_del [degrees];pT 40%-width [#mus]",ser))
//
z0->Scan("EventTime",Form("Entry$==1001||Entry$==%d",z0->GetEntries()-1),"colsize=11")
float tsec0 = 3724

//////////////////////
// Compare Am rates //
//////////////////////
z0->SetAlias("PTOFwckeV","(59.54-13.95)/(52.91-13.95)*(PTOFkeV-13.95)+13.95")

//scatter with errorbars
sim->Draw("keV>>hsim(80,0,40)","keV>0&&keV<40")
//hsim->SetBinContent(hsim->GetNbinsX()+1,0)
hsim->Scale(1./tsecsim/hsim->GetBinWidth(1))
hsim->Scale(bf_60)
hsim->SetTitle(";keV;counts/keV/s")
//hsim->Draw()
//

z0->Draw("PTOFwckeV>>h0V(150,0,75)",!crand+cbsf0+cofint0+camdel+cam0f)
h0V->Scale(1./tsec0/h0V->GetBinWidth(1))
h0V->SetTitle(";keV;counts/keV/s")
h0V->Draw()

//scatter with errorbars
TH1F* hsim60 = new TH1F("hsim60",";keV;counts/keV/s",150,0,75)
for(int p=0;p<10000;p++)hsim60->Fill(59.54+gRandom->Gaus()*2.275)
hsim60->Scale(1.*sim->GetEntries("keV>50")/10000/tsecsim/hsim60->GetBinWidth(1))
hsim60->Scale(bf_60)
hsim60->Draw()

//scatter with errorbars -- gaussian
TH1F* hsim14 = new TH1F("hsim14",";keV;counts/keV/s",150,0,75)
for(int p=0;p<10000;p++)hsim14->Fill(13.95+gRandom->Gaus()*0.98)
hsim14->Scale(1.*sim14->GetEntries("keV>13")/10000/tsecsim14/hsim14->GetBinWidth(1))
hsim14->Scale(bf_14)
hsim14->Draw()

//
z->Draw("PTOFkeV>>hHV(20,0,4)",!crand+cbsf+cofint_vstrict+camdel+"PTOFkeV<4")
//hHV->SetBinContent(hHV->GetNbinsX()+1,0)
hHV->Scale(1./tsechv/hHV->GetBinWidth(1))
hHV->SetTitle(";keV;counts/keV/s")
hHV->Draw()

//
hsim->SetLineColor(kGreen);hsim60->SetLineColor(kGreen);h0V->SetLineColor(kBlue);hHV->SetLineColor(kRed);
hsim60->Draw();hsim->Draw("same");hHV->Draw("same");h0V->Draw("same");

//
z0->Draw("PTOFwckeV>>h0Vb(150,0,75)",!crand+cbsf0+cofint0+!camdel)
h0Vb->Scale(1./tsec0/h0Vb->GetBinWidth(1))
h0Vb->SetTitle(";keV;counts/keV/s")
h0Vb->Draw()
//
z->Draw("PTOFkeV>>hHVb(20,0,4)",!crand+cbsf+cofint_vstrict+!camdel+"PTOFkeV<4")
hHVb->Scale(1./tsechv/hHVb->GetBinWidth(1))
hHVb->SetTitle(";keV;counts/keV/s")
hHVb->Draw()
//
h0Vb->SetLineColor(kBlue-10)
hHVb->SetLineColor(kRed-10)
hsim60->Draw();hHVb->Draw("same");h0Vb->Draw("same");hsim->Draw("same");hHV->Draw("same");h0V->Draw("same")
//
TLegend leg(0.32,0.55,0.67,0.8);leg.SetFillColor(kWhite);
leg.AddEntry(h0V,"0V Am-241","l")
leg.AddEntry(hHV,"HV Am-241","l")
leg.AddEntry(hsim,"60keV simulation","l")
leg.AddEntry(h0Vb,"0V background","l")
leg.AddEntry(hHVb,"HV background","l")
leg.Draw()
//
hsim60->GetYaxis()->SetRangeUser(0,hHV->GetMaximum()*1.05)
hsim60->Draw();hHVb->Draw("same");h0Vb->Draw("same");hsim->Draw("same");hHV->Draw("same");h0V->Draw("same");leg.Draw();
//
c1->Print("~/K100/run076/r76_rates_0V_HV_sim.png")
c1->Print("/data/chocula/fritts/run076/r76_rates_0V_HV_sim.png")
//
hsim14->SetLineColor(kGreen)
hsim14->Draw("same")

sim->SetAlias("x","Sum$(X3*D3)/Sum$(D3)")
sim->SetAlias("y","Sum$(Y3*D3)/Sum$(D3)")
sim->SetAlias("z","Sum$(Z3*D3)/Sum$(D3)")
int xpix = 500, ypix = 500;
TCanvas *csquare = new TCanvas("csquare","csquare",xpix*5/4+4,ypix*5/4+24);
TGraph gc
gc.SetPoint(0,50,0)
gc.SetPoint(1,0,50)
gc.SetPoint(2,-50,0)
gc.SetPoint(3,0,-50)
gc.Draw("ap")
TF1* fcu = new TF1("fcu","sqrt(50**2-x**2)",-50.6,50.6)
TF1* fcl = new TF1("fcu","-sqrt(50**2-x**2)",-50.6,50.6)
sim->Draw("y:x>>h(200,-50,50,200,-50,50)","keV>0","colz same")
fcu->Draw("same");fcl->Draw("same")

TCut cl = "keV>0&&keV<21"
TCut cm = "keV>40&&keV<59"
TCut ch = "keV>59"

gc.SetTitle("60 keV (mean r = 5.44 mm);x [mm];y [mm]")
gc.Draw("ap")
sim->Draw("y:x>>h(200,-50,50,200,-50,50)",ch,"colz same")
fcu->Draw("same");fcl->Draw("same")
csquare->Print("~/K100/run076/r76_sim_yx_60keV.png")
csquare->Print("/data/chocula/fritts/run076/r76_sim_yx_60keV.png")
//
gc.SetTitle("Compton spectrum from 60 keV (mean r = 3.03 mm);x [mm];y [mm]")
gc.Draw("ap")
sim->Draw("y:x>>h(200,-50,50,200,-50,50)",cl,"colz same")
fcu->Draw("same");fcl->Draw("same")
csquare->Print("~/K100/run076/r76_sim_yx_60keVComp.png")
csquare->Print("/data/chocula/fritts/run076/r76_sim_yx_60keVComp.png")
//
sim->SetAlias("r","sqrt(x**2+y**2)")
c1->cd()
sim->Draw("r",cl) // mean 3.032
sim->Draw("r",cm) // mean 10.9
sim->Draw("r",ch) // mean 5.436

sim14->SetAlias("x","Sum$(X3*D3)/Sum$(D3)")
sim14->SetAlias("y","Sum$(Y3*D3)/Sum$(D3)")

csquare->cd()
gc.SetTitle("14 keV (mean r = 0.65 mm);x [mm];y [mm]")
gc.Draw("ap")
sim14->Draw("y:x>>h(500,-50,50,500,-50,50)","keV>0","colz same")
fcu->Draw("same");fcl->Draw("same")
csquare->Print("~/K100/run076/r76_sim_yx_14keV.png")
csquare->Print("/data/chocula/fritts/run076/r76_sim_yx_14keV.png")
//
sim14->SetAlias("r","sqrt(x**2+y**2)")
c1->cd()
sim14->Draw("r","keV>0") // mean 0.6529

//
z0->Draw("afdel:phidel>>h(200,-180,180,150,-40,40)",!crand+cbsf0+"PTOFwckeV>12&&PTOFwckeV<30","colz")
h->SetTitle("0 Volt, 12-30 keV;inner-ring phi_del [degrees];pF vs pA delay [#mus]")
h->Draw("colz")
z0->Draw("afboundp:phidel",cafbound,"same")
z0->Draw("afboundm:phidel",cafbound,"same")
c1->Print("~/K100/run076/r76_0V_afvsphi_12-30.png")
c1->Print("/data/chocula/fritts/run076/r76_0V_afvsphi_12-30.png")
//
z0->Draw("afdel:phidel>>h(200,-180,180,150,-40,40)",!crand+cbsf0+"PTOFwckeV>54&&PTOFwckeV<66","colz")
h->SetTitle("0 Volt, 60 keV;inner-ring phi_del [degrees];pF vs pA delay [#mus]")
h->Draw("colz")
z0->Draw("afboundp:phidel",cafbound,"same")
z0->Draw("afboundm:phidel",cafbound,"same")
c1->Print("~/K100/run076/r76_0V_afvsphi_60.png")
c1->Print("/data/chocula/fritts/run076/r76_0V_afvsphi_60.png")
//
z->SetAlias("afboundp","sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
z->SetAlias("afboundm","-sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
z->Draw("afdel:phidel>>h(200,-180,180,150,-40,40)",!crand+cbsf+cofint_vstrict+"PTOFkeV>0&&PTOFkeV<2","colz")
h->SetTitle("-196 Volt, 0-2 keV;inner-ring phi_del [degrees];pF vs pA delay [#mus]")
h->Draw("colz")
z->Draw("afboundp:phidel",cafbound,"same")
z->Draw("afboundm:phidel",cafbound,"same")
c1->Print("~/K100/run076/r76_HV_afvsphi_0-2.png")
c1->Print("/data/chocula/fritts/run076/r76_HV_afvsphi_0-2.png")
//




/////////////////////////
// 0V data and HV data //
/////////////////////////

gStyle->SetOptStat(0)

char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser0 = "07220510_0917" //<<<<<<
TChain *e0 = new TChain("rqDir/eventTree")
TChain *z0 = new TChain("rqDir/zip1")
e0->Add(Form("%s/%s/umn*root",datapath,ser0));
z0->Add(Form("%s/%s/umn*root",datapath,ser0));
z0->AddFriend(e0)
z0->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")

//07220510_1115	Side2	-195.6
char* ser = "07220510_1115" //<<<<<<
float hv = 195.6 //<<<<<<
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)
z->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
z->SetLineColor(kRed)

TCut crand = "EventCategory"

TCut cbsf = "PFbs<3700" //<<<<<<
TCut cbsf0 = "PFbs<2800" //<<<<<<

TCut cam0 = "PDOFamps/PTOFamps>0.205&&PDOFamps/PTOFamps<0.225" //<<<<<<
//correct amplitude to baseline
TGraph g
g.SetPoint(0,2660,0.39e-6) //<<<<<<
//g.Draw("l*")
g.SetPoint(1,2800,0.36e-6) //<<<<<<
g.Fit("pol1")
z0->SetAlias("PTOF1amps",Form("PTOFamps/(%e+%e*PDbs)",g.GetFunction("pol1")->GetParameter(0)/g.GetY()[0],g.GetFunction("pol1")->GetParameter(1)/g.GetY()[0]))
cout<<endl<<"//z->SetAlias(\"PTOF1amps\",\""<<Form("PTOFamps/(%e+%e*PDbs)",g.GetFunction("pol1")->GetParameter(0)/g.GetY()[0],g.GetFunction("pol1")->GetParameter(1)/g.GetY()[0])<<"\")"<<endl<<endl
//check for variation in time
TCut centrytime = "Entry$<40e3"
//calibrate to 14keV line
float amp14 = 4.03878e-07
z0->SetAlias("PTOFkeV",Form("PTOF1amps*13.95/%e",amp14))
TCut c14 = "PTOFkeV>12&&PTOFkeV<16"



// OF-INT cut
TCut cofint_strict = "PTINTall/PTOFkeV>0&&PTINTall/PTOFkeV<0.0005"
TCut cofint_vstrict = "PTINTall/PTOFkeV>0.0002&&PTINTall/PTOFkeV<0.0005"
// Position studies
z->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
z->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
// Delay-based Am-selection
TCut camdel = "(afdel+3.5)**2/5.5**2+(phidel+1)**2/20**2<1"
//
z->SetAlias("afboundp","sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
z->SetAlias("afboundm","-sqrt(5.5**2*(1.-(phidel+1)**2/20**2))-3.5")
TCut cafbound = "20.**2>(phidel+1)**2"


////////////
// traces //
////////////

/* wasn't commented out but I don't know what this would be besides a comment?
07220510_1115 110504 // -195.6 V 266 eVee
07220510_1115 111577 // -195.6 V 266 eVee
07220510_0917 20051 // 0V 14keV Am
07220510_0917 20145 // 0V 14keV Am
07220510_0917 20239 // 0V 14keV not Am
07220510_0917 20350 // 0V 14keV not Am
07220510_0917 21724 // 0V 60keV wide * 152.5832577 *
07220510_0917 22311 // 0V 60keV narrow * 135.3246724 *
07220510_1115 220241 // -195.6 V  1135 eVee narrow* 183.1175314 * // HAS PILEUP
07220510_1115 482448 // -195.6 V  1135 eVee narrow* 180.4565273 *
07220510_1115 410783 // -195.6 V  1135 eVee wide* 212.2044027 *
*/

root CdmsTraces.root
char* serid = "07220510_0917"
int evno = 20051

char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"}
TGraph praw[6]
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))

float bs[6]
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
cout<<bs[3]<<endl

TGraph pt
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt.SetPoint(p,praw[0]->GetX()[p],yval);}
pt.SetTitle(Form("%s %i PDbs = %0.1f",serid,evno,bs[3]))
pt.Draw("al")

//char* label = "trace_0_14keV_notAm"
//c1->Print(Form("~/K100/run076/r76_%s.png",label))
//c1->Print(Form("/data/chocula/fritts/run076/r76_%s.png",label))

float ymax=0;for(int p=0;p<pt.GetN();p++)if(ymax<pt.GetY()[p])ymax=pt.GetY()[p];

TGraph pta
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pta.SetPoint(p,praw[0]->GetX()[p],yval);}

TF1 ftemplate("ftemplate","(x>400)*628.03/0.39477*(exp(-(x-400)/65)-exp(-(x-400)/21))",0,4000)

// compare 0V 14keV with 196V 0.26 keV
//
root CdmsTraces.root
char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"}
TGraph praw[6]
float bs[6]
//
//07220510_1115 110504 // -195.6 V 266 eVee
char* serid = "07220510_1115"
int evno = 110504
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph ptHV
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];ptHV.SetPoint(p,praw[0]->GetX()[p],yval);}
ptHV.SetTitle("-195.6 V 266 eVee")
//
//07220510_0917 20051 // 0V 14keV Am
char* serid = "07220510_0917"
int evno = 20051
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph pt0VAm
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt0VAm.SetPoint(p,praw[0]->GetX()[p],yval);}
pt0VAm.SetTitle("0 V 13.95 keVee, Am peak")
//
//07220510_0917 20239 // 0V 14keV not Am
char* serid = "07220510_0917"
int evno = 20239
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph pt0Vbg
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt0Vbg.SetPoint(p,praw[0]->GetX()[p],yval);}
pt0Vbg.SetTitle("0 V 13.95 keVee, not Am peak")
//
pt0VAm.SetLineColor(kRed);pt0Vbg.SetLineColor(kMagenta);ptHV.SetLineColor(kBlue);
pt0VAm.GetXaxis()->SetRangeUser(200,1200);
pt0VAm.SetTitle("pTotal")
TLegend leg(0.45,0.55,0.88,0.88);leg.SetFillColor(kWhite);
leg.AddEntry(&pt0VAm,"0V 13.95 keVee in Am peak","l")
leg.AddEntry(&pt0Vbg,"0V 13.95 keVee background","l")
leg.AddEntry(&ptHV,"-196V 266 eVee","l")
pt0VAm.Draw("al");pt0Vbg.Draw("l");ptHV.Draw("l");leg.Draw();
c1->Print("~/K100/run076/r76_pulses_14keVt.png")
c1->Print("/data/chocula/fritts/run076/r76_pulses_14keVt.png")
//
TGraph ptHVn
float ymax=0;for(int p=0;p<ptHV.GetN();p++)if(ymax<ptHV.GetY()[p])ymax=ptHV.GetY()[p];
for(int p=0;p<ptHV.GetN();p++)ptHVn.SetPoint(p,ptHV.GetX()[p],ptHV.GetY()[p]/ymax)
//
TGraph pt0VAmn
float ymax=0;for(int p=0;p<pt0VAm.GetN();p++)if(ymax<pt0VAm.GetY()[p])ymax=pt0VAm.GetY()[p];
for(int p=0;p<pt0VAm.GetN();p++)pt0VAmn.SetPoint(p,pt0VAm.GetX()[p],pt0VAm.GetY()[p]/ymax)
//
TGraph pt0Vbgn
float ymax=0;for(int p=0;p<pt0Vbg.GetN();p++)if(ymax<pt0Vbg.GetY()[p])ymax=pt0Vbg.GetY()[p];
for(int p=0;p<pt0Vbg.GetN();p++)pt0Vbgn.SetPoint(p,pt0Vbg.GetX()[p],pt0Vbg.GetY()[p]/ymax)
//
pt0VAmn.SetLineColor(kRed);pt0Vbgn.SetLineColor(kMagenta);ptHVn.SetLineColor(kBlue);
pt0VAmn.GetXaxis()->SetRangeUser(200,1200);
pt0VAmn.SetTitle("pTotal normalized")
pt0VAmn.Draw("al");pt0Vbgn.Draw("l");ptHVn.Draw("l");leg.Draw();
c1->Print("~/K100/run076/r76_pulses_14keVt_norm.png")
c1->Print("/data/chocula/fritts/run076/r76_pulses_14keVt_norm.png")
//
TF1 ftemplate("ftemplate","(x>400)*[0]/0.39477*(exp(-(x-400)/65)-exp(-(x-400)/21))",0,4000)
ftemplate.SetParameter(0,1)
ftemplate.SetLineColor(kBlack);ftemplate.SetLineStyle(7);
leg.AddEntry(&ftemplate,"template","l")
ftemplate.Draw("same")
pt0VAmn.Draw("l");pt0Vbgn.Draw("l");ptHVn.Draw("l");leg.Draw();
c1->Print("~/K100/run076/r76_pulses_14keVt_norm.png")
c1->Print("/data/chocula/fritts/run076/r76_pulses_14keVt_norm.png")
//
pt0VAm.Draw("al")
pt0VAm.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=pt0VAm.GetY()[p]
cout<<"// "<<ftemplate.Integral(200,1200)/pint<<endl
// 0.852852
pt0VAm.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
pt0VAm.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_0V_14keVt_Am.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_0V_14keVt_Am.png")
//
pt0Vbg.GetXaxis()->SetRangeUser(200,1200);
pt0Vbg.Draw("al")
pt0Vbg.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=pt0Vbg.GetY()[p]
cout<<"// "<<ftemplate.Integral(200,1200)/pint<<endl
// 0.608335
pt0Vbg.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
pt0Vbg.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_0V_14keVt_bg.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_0V_14keVt_bg.png")
//
ptHV.GetXaxis()->SetRangeUser(200,1200);
ptHV.Draw("al")
ptHV.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=ptHV.GetY()[p]
cout<<"// "<<ftemplate.Integral(200,1200)/pint<<endl
// 0.614539
ptHV.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
ptHV.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_0V_14keVt_HV.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_0V_14keVt_HV.png")


// compare 0V 60keV with 196V 1.14 keV
//
root CdmsTraces.root
char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"}
TGraph praw[6]
float bs[6]
//
//07220510_0917 21724 // 0V 60keV wide * 152.5832577 *
char* serid = "07220510_0917"
int evno = 21724
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph pt0Vw
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt0Vw.SetPoint(p,praw[0]->GetX()[p],yval);}
pt0Vw.SetTitle("0 V 60 keVee")
//
//07220510_0917 22311 // 0V 60keV narrow * 135.3246724 *
char* serid = "07220510_0917"
int evno = 22311
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph pt0Vn
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt0Vn.SetPoint(p,praw[0]->GetX()[p],yval);}
pt0Vn.SetTitle("0 V 60 keVee")
//
//07220510_1115 482448 // -195.6 V  1135 eVee narrow* 180.4565273 *
char* serid = "07220510_1115"
int evno = 482448
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph ptHVn
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];ptHVn.SetPoint(p,praw[0]->GetX()[p],yval);}
ptHVn.SetTitle("-195.6 V 1135 eVee")
//
//07220510_1115 410783 // -195.6 V  1135 eVee wide* 212.2044027 *
char* serid = "07220510_1115"
int evno = 410783
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
TGraph ptHVw
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];ptHVw.SetPoint(p,praw[0]->GetX()[p],yval);}
ptHVw.SetTitle("-195.6 V 1135 eVee")
//
pt0Vw.SetLineColor(kRed+2);pt0Vn.SetLineColor(kRed);ptHVw.SetLineColor(kBlue+2);ptHVn.SetLineColor(kBlue);
pt0Vn.GetXaxis()->SetRangeUser(200,1200);
pt0Vn.SetTitle("pTotal")
TLegend leg(0.45,0.55,0.88,0.88);leg.SetFillColor(kWhite);
leg.AddEntry(&pt0Vn,"0V 60 keVee (narrow)","l")
leg.AddEntry(&pt0Vw,"0V 60 keVee (wide)","l")
leg.AddEntry(&ptHVn,"-196V 1135 eVee (narrow)","l")
leg.AddEntry(&ptHVw,"-196V 1135 eVee (wide)","l")
pt0Vn.Draw("al");pt0Vw.Draw("l");ptHVn.Draw("l");ptHVw.Draw("l");leg.Draw();
c1->Print("~/K100/run076/r76_pulses_60keVt.png")
c1->Print("/data/chocula/fritts/run076/r76_pulses_60keVt.png")
//
TGraph ptHVnn
float ymax=0;for(int p=0;p<ptHVn.GetN();p++)if(ymax<ptHVn.GetY()[p])ymax=ptHVn.GetY()[p];
for(int p=0;p<ptHVn.GetN();p++)ptHVnn.SetPoint(p,ptHVn.GetX()[p],ptHVn.GetY()[p]/ymax)
//
TGraph ptHVwn
float ymax=0;for(int p=0;p<ptHVw.GetN();p++)if(ymax<ptHVw.GetY()[p])ymax=ptHVw.GetY()[p];
for(int p=0;p<ptHVw.GetN();p++)ptHVwn.SetPoint(p,ptHVw.GetX()[p],ptHVw.GetY()[p]/ymax)
//
TGraph pt0Vnn
float ymax=0;for(int p=0;p<pt0Vn.GetN();p++)if(ymax<pt0Vn.GetY()[p])ymax=pt0Vn.GetY()[p];
for(int p=0;p<pt0Vn.GetN();p++)pt0Vnn.SetPoint(p,pt0Vn.GetX()[p],pt0Vn.GetY()[p]/ymax)
//
TGraph pt0Vwn
float ymax=0;for(int p=0;p<pt0Vw.GetN();p++)if(ymax<pt0Vw.GetY()[p])ymax=pt0Vw.GetY()[p];
for(int p=0;p<pt0Vw.GetN();p++)pt0Vwn.SetPoint(p,pt0Vw.GetX()[p],pt0Vw.GetY()[p]/ymax)
//
pt0Vwn.SetLineColor(kRed+2);pt0Vnn.SetLineColor(kRed);ptHVwn.SetLineColor(kBlue+2);ptHVnn.SetLineColor(kBlue);
pt0Vnn.GetXaxis()->SetRangeUser(200,1200);
pt0Vnn.SetTitle("pTotal normalized")
pt0Vnn.Draw("al");pt0Vwn.Draw("l");ptHVnn.Draw("l");ptHVwn.Draw("l");leg.Draw();
//
TF1 ftemplate("ftemplate","(x>400)*[0]/0.39477*(exp(-(x-400)/65)-exp(-(x-400)/21))",0,4000)
ftemplate.SetParameter(0,1)
ftemplate.SetLineColor(kBlack);ftemplate.SetLineStyle(7);
leg.AddEntry(&ftemplate,"template","l")
pt0Vnn.Draw("al");pt0Vwn.Draw("l");ptHVnn.Draw("l");ptHVwn.Draw("l");ftemplate.Draw("same");leg.Draw();
c1->Print("~/K100/run076/r76_pulses_60keVt_norm.png")
c1->Print("/data/chocula/fritts/run076/r76_pulses_60keVt_norm.png")
//
pt0Vnn.Draw("al")
pt0Vnn.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=pt0Vnn.GetY()[p]
pt0Vnn.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
pt0Vnn.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_0V_60keVt_nar.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_0V_60keVt_nar.png")
//
pt0Vwn.GetXaxis()->SetRangeUser(200,1200);
pt0Vwn.Draw("al")
pt0Vwn.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=pt0Vwn.GetY()[p]
pt0Vwn.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
pt0Vwn.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_0V_60keVt_wid.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_0V_60keVt_wid.png")
//
ptHVnn.GetXaxis()->SetRangeUser(200,1200);
ptHVnn.Draw("al")
ptHVnn.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=ptHVnn.GetY()[p]
ptHVnn.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
ptHVnn.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_HV_60keVt_nar.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_HV_60keVt_nar.png")
//
ptHVwn.GetXaxis()->SetRangeUser(200,1200);
ptHVwn.Draw("al")
ptHVwn.Fit(&ftemplate)
float pint=0;for(int p=250;p<1500;p++)pint+=ptHVwn.GetY()[p]
ptHVwn.SetTitle(Form("Fit integral/pulse integral = %0.3f",ftemplate.Integral(200,1200)/pint))
ptHVwn.Draw("al")
c1->Print("~/K100/run076/r76_pulsefit_HV_60keVt_wid.png")
c1->Print("/data/chocula/fritts/run076/r76_pulsefit_HV_60keVt_wid.png")


//
root CdmsTraces.root
char* serid = "07220510_1115"
int evno[12] = {72508, 90782, 92010, 20141, 20456, 21062, 20712, 22826, 30628, 21029, 21215, 22115}
// these are HV events between 0.4 and 0.6 keV
// first three: Am-source-related, too-low INT/OF (fails cofint cut)
// second three: Am-source-related, pass cofint_vstrict
// third three: background, pass cofint_vstrict
// fourth three: fail cofint_vstrict but pass simpler PFbspost-PFbs cut 

char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"}
TGraph praw[12][6]
float bs[12][6]

for(int e=0;e<12;e++)for(int d=0;d<6;d++)praw[e][d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno[e],chans[d]))

for(int e=0;e<12;e++)for(int d=0;d<6;d++){float bs[e][d]=0;for(int p=0;p<200;p++)bs[e][d]+=praw[e][d]->GetY()[p];bs[e][d]/=200.;}

TGraph pt[12]
for(int e=0;e<12;e++)for(int p=0;p<praw[0][0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[e][d]->GetY()[p]-bs[e][d];pt[e].SetPoint(p,praw[e][0]->GetX()[p],yval);}
for(int e=0;e<12;e++)pt[e].SetTitle(Form("%s %i PDbs = %0.1f",serid,evno[e],bs[e][3]))

int cols[12] = {kRed, kRed, kRed, kMagenta, kMagenta, kMagenta, kBlue, kBlue, kBlue, kGreen, kGreen, kGreen} 
for(int e=0;e<12;e++)pt[e].SetLineColor(cols[e])
	
pt[0].Draw("al")










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





// HV-varying test
07220712_1336

char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser = "07220712_1336" //<<<<<<
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)
//z->SetLineColor(kRed)
TCut crand = "EventCategory"
z->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
// parameterize HV and Vtest
z->SetAlias("toff_min","(EventTime-1657651308)/60") // the time at which the HV started ramping down 
z->SetAlias("HV_V","(toff_min<0)*(198.45)+(toff_min>=0&&toff_min<7.1)*(198.45-27.946*toff_min)+(toff_min>=7.1)*(0.144-0.000308*toff_min)")
z->SetAlias("Vtest_mV","(toff_min<0)*(32)+(toff_min>=0&&toff_min<7.375)*(56./(toff_min+1)-24-6.7*(toff_min))+(toff_min>=7.375)*(-17.367/(abs(toff_min-7.201))^0.77)")
z->SetAlias("Vdet_V","HV_V-0.44*Vtest_mV") // this has an artifact from t=7.1 to t=7.375, could try to fix that in the Vtest formula
//
z->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
z->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")




// 3.8 V check 
07220513_0853	Side2	Am / Pb&poly E&W only	0	DCRC3pD 10	Position 3	50
07220513_0952	Side2	Am / Pb&poly E&W only	-3.8	DCRC3pD 10	Position 3	46


// HV varying test
// parameterize HV and Vtest
z->SetAlias("toff_min","(EventTime-1657651308)/60") // the time at which the HV started ramping down 
z->SetAlias("HV_V","(toff_min<0)*(198.45)+(toff_min>=0&&toff_min<7.1)*(198.45-27.946*toff_min)+(toff_min>=7.1)*(0.144-0.000308*toff_min)")
z->SetAlias("Vtest_mV","(toff_min<0)*(32)+(toff_min>=0&&toff_min<7.375)*(56./(toff_min+1)-24-6.7*(toff_min))+(toff_min>=7.375)*(-17.367/(abs(toff_min-7.201))^0.77)")
z->SetAlias("Vdet_V","HV_V-0.44*Vtest_mV") // this has an artifact from t=7.1 to t=7.375, could try to fix that in the Vtest formula
//


// compare 0V to biased data
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser[] = {"07220513_0853","07220513_0952"} //<<<<<<
TChain *e[2]
TChain *z[2]
for(int s=0;s<2;s++)TChain *e[s] = new TChain("rqDir/eventTree")
for(int s=0;s<2;s++)TChain *z[s] = new TChain("rqDir/zip1")
for(int s=0;s<2;s++)e[s]->Add(Form("%s/%s/umn*root",datapath,ser[s]));
for(int s=0;s<2;s++)z[s]->Add(Form("%s/%s/umn*root",datapath,ser[s]));
for(int s=0;s<2;s++)z[s]->AddFriend(e[s])
//z->SetLineColor(kRed)
TCut crand = "EventCategory"
for(int s=0;s<2;s++)z[s]->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
for(int s=0;s<2;s++)z[s]->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
for(int s=0;s<2;s++)z[s]->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
for(int s=0;s<2;s++)z[s]->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
for(int s=0;s<2;s++)z[s]->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
//
// length of each series in seconds
int nentries[10]
for(int s=0;s<2;s++)nentries[s]=z[s]->GetEntries()
double timestart[10], timeend[10], timesec[10]
for(int s=0;s<2;s++){z[s]->GetEntry(1000);timestart[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<2;s++){z[s]->GetEntry(nentries[s]-1);timeend[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<2;s++)timesec[s]=timeend[s]-timestart[s]



/////////////////////////
// Ladder Calibration
///////////////////////// 

07220812_1805	Side2	PuBe / Pb&poly, lead stack, borax	0
07220812_1958	Side2	PuBe / Pb&poly, lead stack, borax	-5.8
07220816_0838	Side2	PuBe / Pb&poly, lead stack, borax	-20



char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
char* ser[] = {"07220812_1805","07220812_1958","07220816_0838"} //<<<<<<
int nser = 3
TChain *e[10]
TChain *z[10]
for(int s=0;s<nser;s++)TChain *e[s] = new TChain("rqDir/eventTree")
for(int s=0;s<nser;s++)TChain *z[s] = new TChain("rqDir/zip1")
for(int s=0;s<nser;s++)e[s]->Add(Form("%s/%s/umn*root",datapath,ser[s]));
for(int s=0;s<nser;s++)z[s]->Add(Form("%s/%s/umn*root",datapath,ser[s]));
for(int s=0;s<nser;s++)z[s]->AddFriend(e[s])
//z->SetLineColor(kRed)
TCut crand = "EventCategory"
for(int s=0;s<nser;s++)z[s]->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
for(int s=0;s<nser;s++)z[s]->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
for(int s=0;s<nser;s++)z[s]->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
for(int s=0;s<nser;s++)z[s]->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
for(int s=0;s<nser;s++)z[s]->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
//
// length of each series in seconds
int nentries[10]
for(int s=0;s<nser;s++)nentries[s]=z[s]->GetEntries()
double timestart[10], timeend[10], timesec[10]
for(int s=0;s<nser;s++){z[s]->GetEntry(1000);timestart[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<nser;s++){z[s]->GetEntry(nentries[s]-1);timeend[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<nser;s++)timesec[s]=timeend[s]-timestart[s]




// CentOS version
// need to change char* to TString, and use TString->Data() as argument in Form()
// need to use proper . or ->, Root is pickier now
TString datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
//TString ser[] = {"07220812_1805","07220812_1958","07220816_0838"} //<<<<<<
TString ser[] = {"07220711_0828","07220711_0828_p0","07220812_1805"} //<<<<<<
int nser = 3
TChain *e[10]
TChain *z[10]
for(int s=0;s<nser;s++)e[s] = new TChain("rqDir/eventTree")
for(int s=0;s<nser;s++)z[s] = new TChain("rqDir/zip1")
for(int s=0;s<nser;s++)e[s]->Add(Form("%s/%s/umn*root",datapath.Data(),ser[s].Data()));
for(int s=0;s<nser;s++)z[s]->Add(Form("%s/%s/umn*root",datapath.Data(),ser[s].Data()));
for(int s=0;s<nser;s++)z[s]->AddFriend(e[s])
//z->SetLineColor(kRed)
TCut crand = "EventCategory"
for(int s=0;s<nser;s++)z[s]->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
for(int s=0;s<nser;s++)z[s]->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
for(int s=0;s<nser;s++)z[s]->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
for(int s=0;s<nser;s++)z[s]->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
for(int s=0;s<nser;s++)z[s]->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
//
// length of each series in seconds
int nentries[10]
for(int s=0;s<nser;s++)nentries[s]=z[s]->GetEntries()
double timestart[10], timeend[10], timesec[10]
for(int s=0;s<nser;s++){z[s]->GetEntry(1000);timestart[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<nser;s++){z[s]->GetEntry(nentries[s]-1);timeend[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<nser;s++)timesec[s]=timeend[s]-timestart[s]


 z[0]->Draw("phidel:log(PTINTall)>>h(300,-15,-5,200,-180,180)",!crand,"colz")
 z[0]->Draw("phidel:log(PTOFamps)>>h(300,-17,-12,200,-180,180)",!crand,"colz")
z[0]->Draw("PTOFamps>>h(200,0,0.1e-6)",!crand)

 z[1]->Draw("PTINTall*4.7>>h1(200,0,0.001)",!crand,"same")
 
TCut cgoodwalk = "PAWKr20>3e-4&&PBWKr20>3e-4&&PCWKr20>3e-4&&PDWKr20>3e-4&&PEWKr20>3e-4&&PFWKr20>3e-4&&PAWKr20<5e-4&&PBWKr20<5e-4&&PCWKr20<5e-4&&PDWKr20<5e-4&&PEWKr20<5e-4&&PFWKr20<5e-4"
TCut cabs = "PAbs<2900"

z[1]->SetMarkerColor(kGreen)
z[2]->SetMarkerColor(kRed) 
z[1]->Draw("PTwid:PTINTall",!crand+cgoodwalk+cabs)
z[2]->Draw("PTwid:PTINTall",!crand+cgoodwalk+cabs,"same")



// noise psds

int colsSoft[7]={46,30,9,41,38,40, kBlack};

TString filtPath="/data/chocula/fritts/data/k100proc/midasfilterFiles/all/umn_Filter_"
TString series[] = {"07220711_0828_p0","07220812_1805"} //<<<<<<

int numSeries=2

TFile f[numSeries]
for(int i=0; i<numSeries; i++){f[i]=new TFile(Form("%s%s.root",filtPath.Data(),series[i].Data()));}

TString namesHV[]={"PA","PB","PC","PD","PE","PF","PT"}

TH1D *psd[numSeries][7]//[series][channel]

for(int i=0;i<numSeries;i++)for(int chan=0;chan<7;chan++)psd[i][chan]=(TH1D*)f[i]->Get(Form("zip1/%sNoisePSD",namesHV[chan].Data()))



