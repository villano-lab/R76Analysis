z[0]->Draw("PTOFamps>>h0(300,0,4e-6)",!crand)
z[1]->Draw("PTOFamps>>h1(300,0,4e-6)",!crand,"same")
z[0]->Draw("PTOFamps*2>>h0x2(300,0,4e-6)",!crand)
z[0]->Draw("PTOFamps>>h0(300,0,4e-6)",!crand)
z[1]->Draw("PTOFamps>>h1(300,0,4e-6)",!crand,"same")
z[0]->Draw("PTOFamps*2>>h0x2(300,0,4e-6)",!crand,"same")
h2->Fit("gaus","","",0.7e-6,0.8e-6)
h2->Fit("gaus",0,0,0.7e-6,0.8e-6)
h1->Fit("gaus",0,0,0.7e-6,0.8e-6)
h0->Draw()
h0->Fit("gaus","","",0.26e-6,0.315e-6)
7.59716e-07/2.91324e-07
3.8/(0.6078)
1.607804*3.8
z[1]->Draw("PTOFamps/2.6",!crand,"same")
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
z->SetAlias("xdel","1e6*(PEWKr20-0.5*(PCWKr20+PDWKr20))-1")
z->SetAlias("ydel","1e6*(0.866*(PDWKr20-PCWKr20))+9")
// note empirical offsets in xdel, ydel
z->SetAlias("phidel","180./3.14159*atan2(ydel,xdel)")
z->SetAlias("afdel","1e6*(PFWKr20-PAWKr20)")
//
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
int nentries[]
for(int s=0;s<2;s++)nentries[s]=z[s]->GetEntries()
.q
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
int nentries[]
for(int s=0;s<2;s++)nentries[s]=z[s]->GetEntries()
.q
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
int nentries[10]
for(int s=0;s<2;s++)nentries[s]=z[s]->GetEntries()
double timestart[10], timeend[10], timesec[10]
for(int s=0;s<2;s++){z[s]->GetEntry(1000);timestart[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<2;s++){z[s]->GetEntry(nentries[s]-1));timeend[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<2;s++)timesec[s]=timeend[s]-timestart[s]
z[0]->GetEntry(nentriess[0]-1)
z[0]->GetEntry(nentries[0]-1)
s=0
for(int s=0;s<2;s++){z[s]->GetEntry(nentries[s]-1);timeend[s]=e[s]->GetLeaf("EventTime")->GetValue();}
for(int s=0;s<2;s++)timesec[s]=timeend[s]-timestart[s]
timesec[0]/60
timesec[1]/60
z[0]->Draw("PTOFamps>>h(300,0,2e-6)",!crand)
h->Fit("gaus","","",0.25e-6,0.35e-6)
h->Fit("gaus","","",0.25e-6,0.33e-6)
h->Fit("gaus","","",0.26e-6,0.33e-6)
z[1]->Draw("PTOFamps>>h1(300,0,2e-6)",!crand)
h1->Fit("gaus","","",0.7e-6,0.8e-6)
h1->Fit("gaus","","",0.7e-6,0.82e-6)
7.60144e-07/2.92913e-07
z[0]->Draw("PTOFamps*2.595>>h0s(300,0,3e-6)",!crand)
z[1]->Draw("PTOFamps>>h1(300,0,3e-6)",!crand,"same")
timesec[0]
h0s->Scale(1./timesec[0])
h1->Scale(1./timesec[1])
h1->SetLineColor(kRed)
h0s->Draw()
h1->Draw("same")
h0s->Rebin()
h1->Rebin()
z[0]->Draw("PTOFamps>>h0(300,0,2e-6)",!crand)
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6)",!crand)
z[1]->Draw("PTOFamps/2.595>>h1s(300,0,3e-6)",!crand,"same")
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[1])
h1s->SetLineColor(kRed)
h0->Draw()
h1s->Draw("same")
z[1]->Draw("PTOFamps/2.0>>h1l(300,0,3e-6)",!crand,"same")
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6)",!crand)
z[1]->Draw("PTOFamps/2.595>>h1s(140,0,1.4e-6)",!crand,"same")
z[1]->Draw("PTOFamps/2.0>>h1l(140,0,1.4e-6)",!crand,"same")
h1s->SetLineColor(kRed)
h1l->SetLineColor(kMagenta)
h0->Draw()
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[0])
h1l->Scale(1./timesec[0])
h0->Draw()
h1l->Draw("same")
h1s->Draw("same")
gStyle->SetOptStat(0)
z[0]->Draw("PTOFamps>>h0w(300,0,3e-6",!crand)
z[1]->Draw("PTOFamps>>h1w(300,0,3e-6",!crand)]
h0w->Scale(1./timesec[0])
h1w->Scale(1./timesec[1])
h1w->SetLineColor(kRed)
h0w->SetTitle(";PTOFamps;counts / bin / second")
TLegend leg1(0.3,0.5,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h1w->Draw();h0w->Draw("same");leg1.Draw();
z[1]->Draw("PTOFamps>>h1w(300,0,3e-6",!crand)
h0w->Scale(1./timesec[0])
h1w->Scale(1./timesec[1])
h1w->SetLineColor(kRed)
h0w->SetTitle(";PTOFamps;counts / bin / second")
TLegend leg1(0.3,0.5,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h1w->Draw();h0w->Draw("same");leg1.Draw();
z[0]->Draw("PTOFamps>>h0w(300,0,3e-6",!crand)
z[1]->Draw("PTOFamps>>h1w(300,0,3e-6",!crand)
h0w->Scale(1./timesec[0])
h1w->Scale(1./timesec[1])
h1w->SetLineColor(kRed)
h0w->SetTitle(";PTOFamps;counts / bin / second")
TLegend leg1(0.3,0.75,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h1w->Draw();h0w->Draw("same");leg1.Draw();
TLegend leg1(0.5,0.8,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h1w->Draw();h0w->Draw("same");leg1.Draw();
TLegend leg1(0.5,0.75,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h0w->Draw();h1w->Draw("same");leg1.Draw();
TLegend leg1(0.5,0.7,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h0w->Draw();h1w->Draw("same");leg1.Draw();
float gluke, hvluke
(2.-1)*3.8
(2.595-1)*3.8
c1->Print("~/K100/run076/r76_0v3;8v_ptofamps.png")
c1->Print("/data/chocula/fritts/run076/r76_0v3;8v_ptofamps.png")
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw("PTOFamps/2.0>>h1l(300,0,3e-6",!crand)
h0->Scale(1./timesec[0])
h1l->Scale(1./timesec[1])
h1l->SetLineColor(kMagenta)
h0->SetTitle(";PTOFamps;counts / bin / second")
TLegend leg1(0.5,0.7,0.85,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0,"0-V data","l")
leg1.AddEntry(h1l,"3.8-V data, amp/2.0","l")
h0->Draw();h1l->Draw("same");leg1.Draw();
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw("PTOFamps/2.0>>h1l(140,0,1.4e-6",!crand)
h0->Scale(1./timesec[0])
h1l->Scale(1./timesec[1])
h1l->SetLineColor(kMagenta)
h0->SetTitle(";PTOFamps;counts / bin / second")
TLegend leg1(0.2,0.7,0.55,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0,"0-V data","l")
leg1.AddEntry(h1l,"3.8-V data, amp/2.0","l")
h0->Draw();h1l->Draw("same");leg1.Draw();
TLegend leg1(0.5,0.7,0.7,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0w,"0-V data","l")
leg1.AddEntry(h1w,"3.8-V data","l")
h0w->Draw();h1w->Draw("same");leg1.Draw();
c1->Print("~/K100/run076/r76_0v3p8v_ptofamps.png")
c1->Print("/data/chocula/fritts/run076/r76_0v3p8v_ptofamps.png")
h0->Draw();h1l->Draw("same");leg1.Draw();
c1->Print("~/K100/run076/r76_0v3p8v_lukescale.png")
c1->Print("/data/chocula/fritts/run076/r76_0v3p8v_lukescale.png")
TLegend leg1(0.2,0.7,0.55,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0,"0-V data","l")
leg1.AddEntry(h1l,"3.8-V data, amp/2.0","l")
h0->Draw();h1l->Draw("same");leg1.Draw();
c1->Print("~/K100/run076/r76_0v3p8v_lukescale.png")
c1->Print("/data/chocula/fritts/run076/r76_0v3p8v_lukescale.png")
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw("PTOFamps/2.595>>h1s(140,0,1.4e-6",!crand)
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[1])
h1s->SetLineColor(kRed)
h0->SetTitle(";PTOFamps;counts / bin / second")
TLegend leg1(0.2,0.7,0.55,0.85);leg1.SetFillColor(kWhite);
leg1.AddEntry(h0,"0-V data","l")
leg1.AddEntry(h1s,"3.8-V data, amp/2.595","l")
h0->Draw();h1s->Draw("same");leg1.Draw();
c1->Print("~/K100/run076/r76_0v3p8v_empscale.png")
c1->Print("/data/chocula/fritts/run076/r76_0v3p8v_empscale.png")
float scale=2.7
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw(Form("PTOFamps/%0.3f>>h1s(140,0,1.4e-6)",scale),!crand)
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[1])
h1s->SetLineColor(kRed)
h0->SetTitle(";PTOFamps;counts / bin / second")
h0->Draw();h1s->Draw("same");
float scale=2.4
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw(Form("PTOFamps/%0.3f>>h1s(140,0,1.4e-6)",scale),!crand)
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[1])
h1s->SetLineColor(kRed)
h0->SetTitle(";PTOFamps;counts / bin / second")
h0->Draw();h1s->Draw("same");
float scale=2.35
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw(Form("PTOFamps/%0.3f>>h1s(140,0,1.4e-6)",scale),!crand)
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[1])
h1s->SetLineColor(kRed)
h0->SetTitle(";PTOFamps;counts / bin / second")
h0->Draw();h1s->Draw("same");
float scale=2.38
z[0]->Draw("PTOFamps>>h0(140,0,1.4e-6",!crand)
z[1]->Draw(Form("PTOFamps/%0.3f>>h1s(140,0,1.4e-6)",scale),!crand)
h0->Scale(1./timesec[0])
h1s->Scale(1./timesec[1])
h1s->SetLineColor(kRed)
h0->SetTitle(";PTOFamps;counts / bin / second")
h0->Draw();h1s->Draw("same");
1.+6./3.8
1.+6.1/3.8
3.8/(2.595-1)
h1->Draw()
z[1]->Draw("PTwid:PTOFamps",!crand)
z[1]->Draw("PTwid:PTOFamps>>h(300,0,3e-6,200,0,1200)",!crand,"colz")
z[1]->Draw("PTwid:PTOFamps>>h(300,0,3e-6,200,0,250)",!crand,"colz")
z[1]->Draw("PTwid:PTOFamps/2.595>>h(300,0,1.4e-6,200,0,250)",!crand,"colz")
z[0]->Draw("PTwid:PTOFamps>>hh(300,0,1.4e-6,200,0,250)",!crand,"colz")
h->Draw("colz")
hh->Draw("colz")
h->Draw("colz")
hh->Draw("colz")
h->Draw("colz")
z[0]->Draw("PFbs:PTOFamps>>hh(300,0,1.4e-6,200,0,250)",!crand,"colz")
z[0]->Draw("PFbs:PTOFamps>>hh(300,0,1.4e-6,200,0,5000)",!crand,"colz")
z[0]->Draw("PFbs:PTOFamps>>hh(300,0,1.4e-6,200,2500,3000)",!crand,"colz")
z[0]->Draw("PFbs:PTOFamps>>hh(300,0,1.4e-6,200,2600,2800)",!crand,"colz")
z[1]->Draw("PFbs:PTOFamps/2.595>>h(300,0,1.4e-6,200,2600,2800)",!crand,"colz")
h->Draw("colz")
hh->Draw("colz")
h->Draw("colz")
hh->Draw("colz")
h->Draw("colz")
hh->Draw("colz")
2.595/2
gStyle->SetOptStat(0)
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
//
z[0]->Draw("PTOFamps>>h0w(300,0,3e-6",!crand)
z[0]->Draw("PFOFamps/PTOFamps:PTOFamps>>h(300,0,3e-6,200,0.1,0.5)",!crand)
z[0]->Draw("PTOFamps>>h0w(300,0,3e-6)",!crand+"PFOFamps/PTOFamps>0.25")
z[0]->Draw("PTwid:PTOFamps>>h(300,0,3e-6,200,0,1000)",!crand+"PFOFamps/PTOFamps>0.25","colz")
z[0]->Draw("PTwid:PTOFamps>>h(300,0,3e-6,200,0,300)",!crand+"PFOFamps/PTOFamps>0.25","colz")
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
e->Add("TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
1
e->Add("/data/chocula/fritts/data/k100proc/midasrq/byseries/07180806_1630/umn*root")
z->Add("/data/chocula/fritts/data/k100proc/midasrq/byseries/07180806_1630/umn*root")
z->AddFriend(e)
z->Draw("PFOFamps/PTOFamps:PTOFamps",!crand)
TCut crand = "EventCategory" 
z->Draw("PFOFamps/PTOFamps:PTOFamps",!crand)
z->Draw("PFOFamps/PTOFamps:PTOFamps>>h(300,0,10e-6,200,0,1)",!crand,"colz")
z->Draw("PFOFamps/PTOFamps:Entry$",!crand+"PTOFamps<1")
z->Draw("PFOFamps/PTOFamps:PTOFamps>>h(300,0,10e-6,200,0,1)",!crand,"colz")
z->SetAlias("PTwid","(PTWKf40-PTWKr40)*1e6")
z->Draw("PTwid:PTOFamps>>h(300,0,3e-6,200,0,300)",!crand+"PFOFamps/PTOFamps>0.25","colz")
z->Scan("SeriesNumber:EventNumber",!crand+"PFOFamps/PTOFamps>0.25&&PTOFamps>0.9e-6&&PTOFamps<1.13e-6&&PTwid>130&&PTwid<160","colsize==11")
z[0]->Scan("SeriesNumber:EventNumber",!crand+"PFOFamps/PTOFamps>0.25&&PTOFamps>0.9e-6&&PTOFamps<1.13e-6&&PTwid>130&&PTwid<160","colsize==11")
z[0]->Scan("SeriesNumber:EventNumber",!crand+"PFOFamps/PTOFamps>0.25&&PTOFamps>0.9e-6&&PTOFamps<1.13e-6&&PTwid>130&&PTwid<160","colsize=11")
z]->Scan("SeriesNumber:EventNumber",!crand+"PFOFamps/PTOFamps>0.25&&PTOFamps>1.45e-6&&PTOFamps<1.7e-6&&PTwid>70&&PTwid<90","colsize=11")
z->Scan("SeriesNumber:EventNumber",!crand+"PFOFamps/PTOFamps>0.25&&PTOFamps>1.45e-6&&PTOFamps<1.7e-6&&PTwid>70&&PTwid<90","colsize=11")
.q
TFile *_file0 = TFile::Open("CdmsTraces.root")
.ls
_file0.cd("07180806_1630")
.ls
_file0.cd("07180806_1630/20004")
.ls
.q
TFile *_file0 = TFile::Open("CdmsTraces.root")
TFile *_file0 = TFile::Open("CdmsTraces.root")
.ls
_file0.cd("07180806_1630")
.ls
_file0.cd("07220513_0853")
.ls
.q
TFile *_file0 = TFile::Open("CdmsTraces.root")
char* serid = "07180806_1630"
int evno = 20004
char* chans[6] = {"Detector3_PB", "Detector1_PB", "Detector3_PC", "Detector1_PC", "Detector1_PA", "Detector3_PD"} // for R68, S101
//char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"} // for R76, S104
TGraph praw[6]
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
float bs[6]
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
cout<<bs[3]<<endl
TGraph pt
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt.SetPoint(p,praw[0]->GetX()[p],yval);}
pt.SetTitle(Form("%s %i PDbs = %0.1f",serid,evno,bs[3]))
pt.Draw("al")
TGraph pc[6]
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++)pc[d].SetPoint(p,praw[0]->GetX()[p],praw[d]->GetY[p]-bs[d]);}
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++)pc[d].SetPoint(p,praw[0]->GetX()[p],praw[d]->GetY[p]-bs[d])
praw[1]
for(int d=0;d<1;d++)cout<<praw[d]->GetN()<<endl
for(int d=0;d<6;d++)cout<<praw[d]->GetN()<<endl
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++)pc[d].SetPoint(p,praw[0]->GetX()[p],praw[d]->GetY[p]-bs[d])
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++){float yval = praw[d]->GetY()[p]-bs[d] ;pc[d].SetPoint(p,praw[0]->GetX()[p],yval);}
pc[5]->Draw("al")
for(int d=1;d<5;d++)pc[d]->Draw("same")
kBlack
int cols[6] = {kBlack, kGray, kRed, kBlue, kMagenta, kGreen}
for(int d=0;d<6;d++)pc[d]->SetLineColor(cols[d])
pc[5]->Draw();for(int d=0;d<6;d++)pc[d]->Draw("same");
.q
.q
TFile *_file0 = TFile::Open("CdmsTraces.root")
char* serid = "07220513_0853"
int evno = 20003
char* chans[6] = {"Detector3_PB", "Detector1_PB", "Detector3_PC", "Detector1_PC", "Detector1_PA", "Detector3_PD"} // for R68, S101
//char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"} // for R76, S104
TGraph praw[6]
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
float bs[6]
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
cout<<bs[3]<<endl
TGraph pt
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt.SetPoint(p,praw[0]->GetX()[p],yval);}
pt.SetTitle(Form("%s %i PDbs = %0.1f",serid,evno,bs[3]))
pt.Draw("al")
int cols[6] = {kBlack, kGray, kRed, kBlue, kMagenta, kGreen}
TGraph pc[6]
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++)pc[d].SetPoint(p,praw[0]->GetX()[p],praw[d]->GetY[p]-bs[d])
for(int d=0;d<6;d++)pc[d]->SetLineColor(cols[d])
pc[5]->Draw();for(int d=0;d<6;d++)pc[d]->Draw("same");
.q
root CdmsTraces.root
char* serid = "07220513_0853"
int evno = 20003
//char* chans[6] = {"Detector3_PB", "Detector1_PB", "Detector3_PC", "Detector1_PC", "Detector1_PA", "Detector3_PD"} // for R68, S101
char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"} // for R76, S104
TGraph praw[6]
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
.q
TFile *_file0 = TFile::Open("CdmsTraces.root")
.ls
_file0
_file0->Get("07220513_0853/20003/Detector3_PB")
(TGraph*)_file0->Get("07220513_0853/20003/Detector3_PB")
char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"} // for R76, S104
TGraph praw[6]
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
char* serid = "07220513_0853"
int evno = 20003
//char* chans[6] = {"Detector3_PB", "Detector1_PB", "Detector3_PC", "Detector1_PC", "Detector1_PA", "Detector3_PD"} // for R68, S101
char* chans[6] = {"Detector3_PB", "Detector3_PA", "Detector1_PA", "Detector3_PD", "Detector1_PD", "Detector1_PB"} // for R76, S104
TGraph praw[6]
for(int d=0;d<6;d++)praw[d]=(TGraph*)_file0->Get(Form("%s/%i/%s",serid,evno,chans[d]))
float bs[6]
for(int d=0;d<6;d++){float bs[d]=0;for(int p=0;p<200;p++)bs[d]+=praw[d]->GetY()[p];bs[d]/=200.;}
cout<<bs[3]<<endl
TGraph pt
for(int p=0;p<praw[0]->GetN();p++){float yval=0;for(int d=0;d<6;d++)yval+=praw[d]->GetY()[p]-bs[d];pt.SetPoint(p,praw[0]->GetX()[p],yval);}
pt.SetTitle(Form("%s %i PDbs = %0.1f",serid,evno,bs[3]))
pt.Draw("al")
int cols[6] = {kBlack, kGray, kRed, kBlue, kMagenta, kGreen}
TGraph pc[6]
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++)pc[d].SetPoint(p,praw[0]->GetX()[p],praw[d]->GetY[p]-bs[d])
for(int d=0;d<6;d++)pc[d]->SetLineColor(cols[d])
pc[5]->Draw();for(int d=0;d<6;d++)pc[d]->Draw("same");
for(int d=0;d<6;d++)for(int p=0;p<praw[0]->GetN();p++){float yval = praw[d]->GetY()[p]-bs[d] ;pc[d].SetPoint(p,praw[0]->GetX()[p],yval);}
for(int d=0;d<6;d++)pc[d]->SetLineColor(cols[d])
pc[5]->Draw();for(int d=0;d<6;d++)pc[d]->Draw("same");
pc[5]->Draw("al");for(int d=0;d<6;d++)pc[d]->Draw("same");
pc[3]->Draw("al");for(int d=0;d<6;d++)pc[d]->Draw("same");
pc[3]->Draw("al")
for(int d=0;d<6;d++)pc[d]->Draw("same");
pc[3]->Draw("al")
pc[3]->Draw("al");for(int d=0;d<6;d++)pc[d]->Draw("same");
