TFile *_file0 = TFile::Open("DataF_CH0_DT5751_626_NaI-Array-Det1-1121V-Bknd-20220322-Na22.root")
TFile *_file1 = TFile::Open("muontree_1det.root")

TTree* tdata = (TTree*)_file0->Get("Data_F")
TTree* tcalc = (TTree*)_file1->Get("muontree")

tdata->SetAlias("EkeV1","CalibEnergy*4.20382-102.61")
tdata->SetAlias("EkeV2","CalibEnergy*4.229-105.623-4.29781e-05*CalibEnergy**2")

tcalc->SetAlias("MeV_per_z_cm","4.785/cos(theta)")
tcalc->SetAlias("EMeV","MeV_per_z_cm*(z1-z2)")

float tsec = 100.149

tcalc->Draw("EMeV>>hc(60,0,60)","rate")
hc->Scale(tsec)
hc->SetLineColor(kRed)
hc->Draw()

tdata->Draw("EkeV1/1000>>hd(60,0,60)")
hc->Draw("same")

tdata->SetAlias("EkeVhigh","EkeV1*3.36111-7083.33")
tdata->Draw("EkeVhigh/1000>>hd(57,3,60)")
hc->Draw("same")

hc->Integral(6,25) // 5-25 MeV calculated events: 187.9
tdata->GetEntries("EkeVhigh>5000&&EkeVhigh<25000") // 5-25 MeV observed events: 177
