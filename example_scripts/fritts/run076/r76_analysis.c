// Am peaks at 59.54 keV
//   60keV (36%), 18keV (18%), 14keV (13%) according to one reference
//   13.9 (37%), 26.3 (2.3%), 59.54 (36%) from nudat
//   but just google image search americium spectrum, lots of examples of linesat 13.95, 17.74, 20.8, 26.4
//   try searching e.g. Am-241 "17.74 keV". Some references have 22.12 instead of 20.8, this seems closer to our data

// {0, 13.95, 17.74, 20.8, 26.4, 59.54}

// first three source-position data series, and tuned cuts for each to select 14keV Am events and background events above 60keV
char* sers[3] = {"07220330_1639", "07220405_1646", "07220406_1720"} 
char* c14s[3] = {"PFS1OFamps/PTOFamps>0.25&&PTOFamps>0.48e-6&&PTOFamps<0.53e-6", "PFS1OFamps/PTOFamps>0.15&&PTOFamps>0.37e-6&&PTOFamps<0.47e-6", "PES1OFamps/PTOFamps>0.25&&PTOFamps>0.45e-6&&PTOFamps<0.52e-6"}
char* cbacks[3] = {"PTOFamps>2.3e-6&&PTOFamps<3e-6","PTOFamps>1.91e-6&&PTOFamps<2.5e-6","PTOFamps>2.21e-6&&PTOFamps<2.88e-6"}

char* chans[6] = {"A","B","C","D","E","F"}

// choose one series below to load into data (0, 1, or 2)
int s = 2

gStyle->SetOptStat(0)
char* ser = sers[s]
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
int nsparse = 1
char* datapath ="/data/chocula/fritts/data/k100proc/midasrq/byseries/"
e->Add(Form("%s/%s/umn*root",datapath,ser));
z->Add(Form("%s/%s/umn*root",datapath,ser));
z->AddFriend(e)
TCut crand = "EventCategory"

TCut c14 = c14s[s]
TCut cback = cbacks[s]

// good time info cut removes some of the events for which the walk algorithm fails
TCut ctgood = "PFS1WKr20>0.00036&&PFS1WKr20<0.00043&&PES1WKr20>0.00036&&PES1WKr20<0.00043&&PDS1WKr20>0.00036&&PDS1WKr20<0.00043&&PCS1WKr20>0.00036&&PCS1WKr20<0.00043"

// ===============================================================================================
// plots of time delays between two channels. select channel names (A, B, C, D, E, F) and sides (1, 2)
TH1F* hb = new TH1F("hb","",200,-80,80);hb->SetLineColor(kRed);
TH1F* hs = new TH1F("hs","",200,-80,80)
TLegend leg(0.73,0.76,0.88,0.88); leg.SetFillColor(kWhite);
leg.AddEntry(hb,"background","l")
leg.AddEntry(hs,"source","l")
TLegend leg2(0.79,0.13,0.89,0.18);leg2.SetFillColor(kWhite);
// ---one at a time-------------------------------------------------------------------------------
char* chan1 = "C"
char* chan2 = "D"
char* side1 = "2"
char* side2 = "2"

hb->SetTitle(Form("Difference in 20% risetime, S%sp%s minus S%sp%s;#mus",side1,chan1,side2,chan2))
//z->Draw(Form("(P%sS%sWKr20-P%sS%sWKr20)*1e6>>hb",chan1,side1,chan2,side2),!crand+ctgood+cback)
//z->Draw(Form("(P%sS%sWKr20-P%sS%sWKr20)*1e6>>hs",chan1,side1,chan2,side2),!crand+ctgood+c14,"same")
z->Draw(Form("(P%sS%sr20-P%sS%sr20)*1e6>>hb",chan1,side1,chan2,side2),!crand+ctgood+cback)
z->Draw(Form("(P%sS%sr20-P%sS%sr20)*1e6>>hs",chan1,side1,chan2,side2),!crand+ctgood+c14,"same")
leg.Draw(); leg2.SetHeader(Form("%0.2f",(hs->GetMean()-hb->GetMean())/hb->GetStdDev())); leg2.Draw();
c1->Print(Form("~/K100/run076/r76_%s_delay_S%sp%s_S%sp%s.png",ser,side1,chan1,side2,chan2))
// ---or all at once-------------------------------------------------------------------------------
char* chan1s[13] = {"C","C","C","D","D","E","C","C","C","D","D","E","F"}
char* chan2s[13] = {"D","E","F","E","F","F","D","E","F","E","F","F","F"}
char* side1s[13] = {"1","1","1","1","1","1","2","2","2","2","2","2","1"}
char* side2s[13] = {"1","1","1","1","1","1","2","2","2","2","2","2","2"}
char* chan1; char* chan2; char* side1; char* side2;

//for(int p=0;p<13;p++){chan1=chan1s[p];chan2=chan2s[p];side1=side1s[p];side2=side2s[p];hb->SetTitle(Form("Difference in 20% risetime, S%sp%s minus S%sp%s;#mus",side1,chan1,side2,chan2));z->Draw(Form("(P%sS%sWKr20-P%sS%sWKr20)*1e6>>hb",chan1,side1,chan2,side2),!crand+ctgood+cback);z->Draw(Form("(P%sS%sWKr20-P%sS%sWKr20)*1e6>>hs",chan1,side1,chan2,side2),!crand+ctgood+c14,"same");leg.Draw(); leg2.SetHeader(Form("%0.2f",(hs->GetMean()-hb->GetMean())/hb->GetStdDev())); leg2.Draw();c1->Print(Form("~/K100/run076/r76_%s_delay_S%sp%s_S%sp%s.png",ser,side1,chan1,side2,chan2));}
for(int p=0;p<13;p++){chan1=chan1s[p];chan2=chan2s[p];side1=side1s[p];side2=side2s[p];hb->SetTitle(Form("Difference in 20% risetime, S%sp%s minus S%sp%s;#mus",side1,chan1,side2,chan2));z->Draw(Form("(P%sS%sr20-P%sS%sr20)*1e6>>hb",chan1,side1,chan2,side2),!crand+ctgood+cback);z->Draw(Form("(P%sS%sr20-P%sS%sr20)*1e6>>hs",chan1,side1,chan2,side2),!crand+ctgood+c14,"same");leg.Draw(); leg2.SetHeader(Form("%0.2f",(hs->GetMean()-hb->GetMean())/hb->GetStdDev())); leg2.Draw();c1->Print(Form("~/K100/run076/r76_%s_delay_S%sp%s_S%sp%s.png",ser,side1,chan1,side2,chan2));}
// -----------------------------------------------------------------------------------------------


// ===============================================================================================
// plots of amplitude comparisons (partitions) between two channels. select channel names (A, B, C, D, E, F) and sides (1, 2)
// note: the distributions here become more "compressed" at higher energy, i.e. there's more apparent sharing. 
// (likely because of saturation effects distorting OF - for INT it's a smaller effect)
// Since "background" is ~ 100keV and "source" is 14 keV then the position of the source peak will be more extreme than expected 
TH1F* hb = new TH1F("hb","",200,-1.2,1.2);hb->SetLineColor(kRed);
TH1F* hs = new TH1F("hs","",200,-1.2,1.2)
TLegend leg(0.73,0.76,0.88,0.88); leg.SetFillColor(kWhite);
leg.AddEntry(hb,"background","l")
leg.AddEntry(hs,"source","l")
TLegend leg2(0.79,0.13,0.89,0.18);leg2.SetFillColor(kWhite);
// ---one at a time-------------------------------------------------------------------------------
char* chan1 = "F"
char* chan2 = "F"
char* side1 = "1"
char* side2 = "2"

hb->SetTitle(Form("Partition between S%sp%s (neg) and S%sp%s (pos);OF amplitude difference / OF amplitude sum",side1,chan1,side2,chan2))
hs->SetTitle(Form("Partition between S%sp%s (neg) and S%sp%s (pos);OF amplitude difference / OF amplitude sum",side1,chan1,side2,chan2))
z->Draw(Form("(P%sS%sOFamps-P%sS%sOFamps)/(P%sS%sOFamps+P%sS%sOFamps)>>hs",chan2,side2,chan1,side1,chan2,side2,chan1,side1),!crand+c14)
z->Draw(Form("(P%sS%sOFamps-P%sS%sOFamps)/(P%sS%sOFamps+P%sS%sOFamps)>>hb",chan2,side2,chan1,side1,chan2,side2,chan1,side1),!crand+cback,"same")
if( hb->GetBinContent(hb->GetMaximumBin()) > hs->GetBinContent(hs->GetMaximumBin())){hb->Draw();hs->Draw("same");}
leg.Draw(); leg2.SetHeader(Form("%0.2f",(hs->GetMean()-hb->GetMean())/hb->GetStdDev())); leg2.Draw();
c1->Print(Form("~/K100/run076/r76_%s_partition_S%sp%s_S%sp%s.png",ser,side1,chan1,side2,chan2))
// ---or all at once-------------------------------------------------------------------------------
char* chan1s[13] = {"C","C","C","D","D","E","C","C","C","D","D","E","F"}
char* chan2s[13] = {"D","E","F","E","F","F","D","E","F","E","F","F","F"}
char* side1s[13] = {"1","1","1","1","1","1","2","2","2","2","2","2","1"}
char* side2s[13] = {"1","1","1","1","1","1","2","2","2","2","2","2","2"}
char* chan1,chan2,side1,side2

for(int p=0;p<13;p++){chan1=chan1s[p];chan2=chan2s[p];side1=side1s[p];side2=side2s[p];hb->SetTitle(Form("Partition between S%sp%s (neg) and S%sp%s (pos);OF amplitude difference / OF amplitude sum",side1,chan1,side2,chan2));hs->SetTitle(Form("Partition between S%sp%s (neg) and S%sp%s (pos);OF amplitude difference / OF amplitude sum",side1,chan1,side2,chan2));z->Draw(Form("(P%sS%sOFamps-P%sS%sOFamps)/(P%sS%sOFamps+P%sS%sOFamps)>>hs",chan2,side2,chan1,side1,chan2,side2,chan1,side1),!crand+c14);z->Draw(Form("(P%sS%sOFamps-P%sS%sOFamps)/(P%sS%sOFamps+P%sS%sOFamps)>>hb",chan2,side2,chan1,side1,chan2,side2,chan1,side1),!crand+cback,"same");if( hb->GetBinContent(hb->GetMaximumBin()) > hs->GetBinContent(hs->GetMaximumBin())){hb->Draw();hs->Draw("same");}leg.Draw(); leg2.SetHeader(Form("%0.2f",(hs->GetMean()-hb->GetMean())/hb->GetStdDev())); leg2.Draw();c1->Print(Form("~/K100/run076/r76_%s_partition_S%sp%s_S%sp%s.png",ser,side1,chan1,side2,chan2));}
// -----------------------------------------------------------------------------------------------






z->Draw(Form("(P%sS%sINTall-P%sS%sINTall)/(P%sS%sINTall+P%sS%sINTall)>>hs",chan1,side1,chan2,side2,chan1,side1,chan2,side2),!crand+c14)
z->Draw(Form("(P%sS%sINTall-P%sS%sINTall)/(P%sS%sINTall+P%sS%sINTall)>>hb",chan1,side1,chan2,side2,chan1,side1,chan2,side2),!crand+cback,"same")
if( hb->GetBinContent(hb->GetMaximumBin()) > hs->GetBinContent(hs->GetMaximumBin())){hb->Draw();hs->Draw("same");}
leg.Draw()



z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,3e-6,200,0.0,0.4)",!crand,"colz")



char* side1 = "2"
char* chan1 = "A"
z->Draw(Form("P%sS%sWKr20:Entry$",chan1,side1),!crand+ctgood+c14)



for(int c=0;c<6;c++)z->SetAlias(Form("P%sS1r20",chans[c]),Form("P%sS1WKr20",chans[c]))
for(int c=0;c<6;c++)z->SetAlias(Form("P%sS2r20",chans[c]),Form("P%sS2WKr20",chans[c]))
if(s==0)z->SetAlias("PBS1r20","PBS1WKr20-(Entry$>220e3)*5e-6")
if(s==0)z->SetAlias("PCS2r20","PCS2WKr20-(Entry$>60e3)*5.5e-6")
if(s==0)z->SetAlias("PFS2r20","PFS2WKr20-(Entry$>160e3)*5e-6")
if(s==1)z->SetAlias("PCS1r20","PCS1WKr20-(Entry$>120e3)*5.5e-6")
if(s==1)z->SetAlias("PFS2r20","PFS2WKr20-(Entry$>105e3)*5.5e-6")
if(s==1)z->SetAlias("PCS2r20","PCS2WKr20-(Entry$>55e3)*5e-6")
if(s==2)z->SetAlias("PCS2r20","PCS2WKr20-(Entry$>90e3)*5.5e-6")



z->Draw("PTINTall",!crand)
z->Draw("PTOFamps:PTINTall",!crand)
z->Draw("PFS1INTall/PTINTall:PTINTall",!crand,"colz")
z->Draw("PFS1INTall/PTINTall:PTINTall>>h2(300,0,0.05,200,-1,2)",!crand,"colz")
z->Draw("PFS1INTall/PTINTall:PTINTall>>h2(300,0,0.05,200,0,0.5)",!crand,"colz")
z->Draw("PFS1INTall/PTINTall:PTINTall>>h2(300,0,0.03,200,0,0.5)",!crand,"colz")
z->Draw("PFS1INTall/PTINTall:PTINTall>>h2(300,0,0.03,200,0,0.2)",!crand,"colz")
z->Draw("PFS1INTall/PTINTall:PTINTall>>h2(300,0,0.015,200,0.06,0.14)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,1e-6,200,0.06,0.14)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,1e-5,200,0.06,0.14)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,1e-5,200,0.0,00.5)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,3e-7,200,0.0,0.4)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,1e-5,200,0.0,00.5)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,1e-5,200,0.05,0.4)",!crand,"colz")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,3e-6,200,0.05,0.4)",!crand,"colz")
1./12
z->Draw("PTOFamps>>h2(300,0.15e-6,3e-6)",!crand+"PFS1OFamps/PTOFamps>0.25")
z->Draw("PTOFamps>>h2(300,0.15e-6,1e-6)",!crand+"PFS1OFamps/PTOFamps>0.25")
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,3e-6,200,0.05,0.4)",!crand,"colz")
z->Draw("PTOFamps>>h2(300,0.15e-6,1e-6)",!crand+"PFS1OFamps/PTOFamps>0.25")
TCut c14 = "PFS1OFamps/PTOFamps>0.25&&PTOFamps>0.48e-6&&PTOFamps>0.53e-6"
z->Draw("PFS1OFamps/PTOFamps:PTOFamps>>h2(300,0,3e-6,200,0.05,0.4)",!crand,"colz")
TCut cback = "PTOFamps>2.3e-6&&PTOFamps<3e-6"
z->Draw("PDS2WKr20-PCS2WKr20",!crand+cback)
z->Draw("PDS2WKr20",!crand+cback)
z->Draw("PDS2WKr20",crabd)
z->Draw("PDS2WKr20",crand)
z->Draw("PDS2WKr20",!crand+cback)
z->Draw("PDS2WKr20>>h(200,0.0003,0.0005)",!crand+cback)
TCut cdgood = "PDS2WKr20>0.385e-3&&PDS2WKr20<0.425e-3"
z->Draw("PDS2WKr20",!crand+cback+cdgood)
z->Draw("PDS2WKr20-PCS2WKr20",!crand+cdgood+cback)
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6",!crand+cdgood+cback)
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6>>h(200,-100,100)",!crand+cdgood+cback)
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6>>h2(200,-100,100)",!crand+cdgood+c14)
h2->Draw()
h->Draw("same")
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6:Entry$",!crand+cdgood+c14+""(PDS2WKr20-PCS2WKr20)*1e6>-20&&"(PDS2WKr20-PCS2WKr20)*1e6<0")
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6:Entry$",!crand+cdgood+c14+"(PDS2WKr20-PCS2WKr20)*1e6>-20&&(PDS2WKr20-PCS2WKr20)*1e6<0")
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6:EventTime",!crand+cdgood+c14+"(PDS2WKr20-PCS2WKr20)*1e6>-20&&(PDS2WKr20-PCS2WKr20)*1e6<0")
h2->Draw()
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6>>h2(200,-40,20)",!crand+cdgood+c14)
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6>>h2(200,-40,20)",!crand+cdgood+c14+"Entry$>50000")
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6>>h2(200,-40,20)",!crand+cdgood+c14+"Entry$<20000")
z->Draw("(PDS2WKr20-PCS2WKr20)*1e6>>h2(200,-40,20)",!crand+cdgood+c14)
h2->Draw()
h1->Draw("same")
h->Draw("same")
