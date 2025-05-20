// Am peaks at 59.54 keV
//   60keV (36%), 18keV (18%), 14keV (13%) according to one reference
//   13.9 (37%), 26.3 (2.3%), 59.54 (36%) from nudat
//   but just google image search americium spectrum, lots of examples of linesat 13.95, 17.74, 20.8, 26.4
//   try searching e.g. Am-241 "17.74 keV". Some references have 22.12 instead of 20.8, this seems closer to our data

// {0, 13.95, 17.74, 20.8, 26.4, 59.54}

07220706_1112
07220706_1659 218 minutes
07220706_2048	Side2	PuBe	-197.3	NaI	575
07220707_0644	Side2	PuBe	-197.3	NaI	289
07220707_1758	Side2	PuBe	-197.3	NaI 227
07220707_2205	Side2	PuBe	-197.3	NaI	611
07220708_0833	Side2	PuBe	-197.3	NaI	160
07220708_1632	Side2	PuBe	-197.4	NaI 332

char* ser = "07220708_1632"
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
TChain *z4 = new TChain("rqDir/zip4")
e->Add(Form("/data/chocula/fritts/data/k100proc/midasrq/byseries/%s/umn*root",ser))
z->Add(Form("/data/chocula/fritts/data/k100proc/midasrq/byseries/%s/umn*root",ser))
z4->Add(Form("/data/chocula/fritts/data/k100proc/midasrq/byseries/%s/umn*root",ser))
z->AddFriend(e)
z->AddFriend(z4)
TCut crand = "EventCategory"
z->SetAlias("PTwid","(zip1.PTWKf40-zip1.PTWKr40)*1e6")
z->GetEntries()
z->GetEntries(!crand)
z->Draw("PTOFamps",!crand)
z->Draw("PTOFdelay",!crand+"PTOFamps>0.5e-6")
z->Draw("PTOFdelay",crand+"PTOFamps>0.5e-6")
z->Draw("PTOFdelay",!crand+"PTOFamps>0.5e-6")
z->Draw("PTOFdelay/0.8e-6",!crand+"PTOFamps>0.5e-6")
z->Draw("PTINTall",!crand+"PTOFamps>0.5e-6")
z->Draw("PTOFamps",!crand)
z->Draw("PTOFamps>>h(300)",!crand)
z->Draw("PTOFamps",crand,"same")
z->Draw("PTOFamps>>hr(300,-0.5e-6,4e-6)",crand)
z->Draw("PTOFamps>>ht(300,-0.5e-6,4e-6)",!crand)
hr->SetLineColor(kRed)
hr->Draw()
ht->Draw("same")
z->Draw("PTOFamps>>hr(150,-0.5e-6,4e-6)",crand)
z->Draw("PTOFamps>>ht(150,-0.5e-6,4e-6)",!crand)
hr->SetLineColor(kRed)
hr->Draw()
ht->Draw("same")
z->GetEntries(!crand+"PTOFamps>0.4e-6")
z->Draw("PTOFdelay/0.8e-6",crand+"PTOFamps>0.5e-6")
TCut cdel = "PTOFdelay/0.8e-6>-90&&PTOFdelay/0.8e-6<75"
z->Draw("PTOFamps>>hr(150,-0.5e-6,4e-6)",crand+cdel)
z->Draw("PTOFamps>>ht(150,-0.5e-6,4e-6)",!crand+cdel)
hr->SetLineColor(kRed)
hr->Draw()
ht->Draw("same")
hr->Draw()
ht->Draw()
hr->Draw()
ht->Draw("same")
hr->Draw()
ht->Draw("same")
z->Draw("PTOFdelay:PTOFamps",!crand)
z->Draw("PTOFdelay:PTOFamps",crand,"same")
z->Draw("PTOFdelay",!crand+"PTOFamps>0.2e-6")
z->Draw("PTOFdelay",crand+"PTOFamps>0.2e-6","same")
TCut ccoin = "PTOFdelay>0.005e-3&&PTOFdelay<0.25e-3"
z->Draw("PTOFamps",!crand+ccoin)

//NaI trigger rate
TCut cwindow = "zip4.PAWKr100>0.0005&&zip4.PAWKr100<0.003"
float twindow = 0.003-0.0005 // 2.5 ms
// choose an amplitude threshold
TCut cpeak = "zip4.PAWKmax>0.7e-6"
// calculate event rate
float ntraces = z->GetEntries(crand)
float nevents = z->GetEntries(crand+cwindow+cpeak)
cout<<"// "<<ser<<": "<<nevents<<" of "<<ntraces<<endl
// 07220706_1112: 42 of 30786
// 07220706_1659: 1189 of 66569
// 07220708_1632: 193 of 100810
float rate = -log((ntraces-nevents)/ntraces)/twindow
cout<<"// "<<ser<<": "<<rate<<" Hz"<<endl
// 07220706_1112: 0.546075 Hz
// 07220706_1659: 7.20904 Hz
// 07220706_2048: 7.25974 Hz
// 07220707_0644: 7.13728 Hz
// 07220707_1758: 6.92956 Hz
// 07220707_2205: 6.7667 Hz
// 07220708_0833: 7.21856 Hz
// 07220708_1632: 0.766531 Hz

// Calibration: rough version
float l14 = 2.48280e-07 // from 0-Volt data
float hv_nom = 197.3
float vtest_mV = 10
float hv = hv_nom-0.44*vtest_mV
float gluke = 1.+hv/3.8
z->SetAlias("PTOF_ptot_0_keV",Form("PTOFamps*%e",13.95/l14)) // based on 0V 14 keV line only
z->SetAlias("PTOF_ee_0_keV",Form("PTOF_ptot_0_keV/%f",gluke)) // based on 0V 14 keV line only 
float l60 = 9.74669e-07 // from 0-Volt data
float par1 = (59.54-13.95)/(l60-l14)
float par0 = 13.95-(59.54-13.95)/(l60-l14)*l14
z->SetAlias("PTOF_ptot_1_keV",Form("PTOFamps*%e+%e",par1,par0)) // based on 0V 14 keV and 60 keV lines
z->SetAlias("PTOF_ee_1_keV",Form("PTOF_ptot_1_keV/%f",gluke)) // based on 0V 14 keV and 60 keV lines




//0V data
char* ser = "07220706_0836"
TChain *e = new TChain("rqDir/eventTree")
TChain *z = new TChain("rqDir/zip1")
TChain *z4 = new TChain("rqDir/zip4")
e->Add(Form("/data/chocula/fritts/data/k100proc/midasrq/byseries/%s/umn*root",ser))
z->Add(Form("/data/chocula/fritts/data/k100proc/midasrq/byseries/%s/umn*root",ser))
z4->Add(Form("/data/chocula/fritts/data/k100proc/midasrq/byseries/%s/umn*root",ser))
z->AddFriend(e)
z->AddFriend(z4)
TCut crand = "EventCategory"
z->SetAlias("PTwid","(zip1.PTWKf40-zip1.PTWKr40)*1e6")
float l14 = 2.48280e-07
float l60 = 9.74669e-07
