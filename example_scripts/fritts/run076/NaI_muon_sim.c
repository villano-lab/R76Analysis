
void calculateRates(){
	// What we want for each muon path: z1, z2, theta, rate
	//
	// vertical stack. approximation: no separation between detectors assumed. 
	// Detector width and thickness and height and number
	float W = 40.4;
	float T = 10.2; //float T = 10.2*3; 
	float Hdet = 5.7;
	float Ndet = 20; //float Ndet = 1;
	float H = Hdet*Ndet;
	//
	float pi = 2*asin(1);
	float C = 0.0083; // /cm2/sec/SR
	// variables for scanning
	float x, y, z, theta, phi, theta_d, phi_d;
	float dlnom = 1; // cm
	float dlx, dly, dlz;
	float da_d = 1; // degrees
	float da = da_d/180.*pi;
	float dR;


	// FACE HIT
	// scan x, z, theta, phi
	float r1a=0; float r1b=0; float r2a=0; float r2b=0; 
	dlx = W/int(W/dlnom); dlz = H/int(H/dlnom); 
	for(x=dlx/2;x<W;x+=dlx)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
//		if(gRandom->Rndm()<1e-6)cout<<x/W<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dlz*da**2*(cos(theta)*sin(theta))**2*sin(phi);
		// CASE 1: heading toward left(right) side
		//    note: situation is symmetrical for left versus right side, so we can just double the rates for this left-side calculation.
		if(phi<atan(T/x)){
			//CASE 1A: exits left(right) side
			if(theta>atan(x/z/cos(phi))){
				r1a+=dR;
			}
			//CASE 1B: exits bottom
			if(theta<atan(x/z/cos(phi))){
				r1b+=dR;
			}
		}
		// CASE 2: heading toward opposite side
		if(phi>atan(T/x)&&phi<pi-atan(T/(W-x))){
			//CASE 2a: exits opposite side
			if(theta>atan(T/z/sin(phi))){
				r2a+=dR;
			}
			//CASE 2b: exits bottom
			if(theta<atan(T/z/sin(phi))){
				r2b+=dR;
			}
		}
	}
	cout<<endl<<endl;
	cout<<"for muons entering either wide face:"<<endl;
	cout<<endl;
	cout<<4*r1a<<" Hz exit a narrow face"<<endl;
	cout<<2*r2a<<" Hz exit opposite face"<<endl;
	cout<<r1b*4+r2b*2<<" Hz exit bottom"<<endl;
	cout<<4*r1a+2*r2a+r1b*4+r2b*2<<" Hz total"<<endl;
	cout<<endl;


	// EDGE HIT
	// scan x, z, theta, phi
	float r3a=0; float r3b=0; float r4a=0; float r4b=0; 
	dlx = T/int(T/dlnom); dlz = H/int(H/dlnom); 
	for(x=dlx/2;x<T;x+=dlx)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dlz*da**2*(cos(theta)*sin(theta))**2*sin(phi);
		// CASE 3: heading toward left(right) side
		//    note: situation is symmetrical for left versus right side, so we can just double the rates for this left-side calculation.
		if(phi<atan(W/x)){
			//CASE 3A: exits left(right) side
			if(theta>atan(x/z/cos(phi))){
				r3a+=dR;
			}
			//CASE 3B: exits bottom
			if(theta<atan(x/z/cos(phi))){
				r3b+=dR;
			}
		}
		// CASE 4: heading toward opposite side
		if(phi>atan(W/x)&&phi<pi-atan(W/(T-x))){
			//CASE 4a: exits opposite side
			if(theta>atan(W/z/sin(phi))){
				r4a+=dR;
			}
			//CASE 4b: exits bottom
			if(theta<atan(W/z/sin(phi))){
				r4b+=dR;
			}
		}
	}
	cout<<"for muons entering either narrow face:"<<endl;
	cout<<endl;
	cout<<4*r3a<<" Hz exit a wide face"<<endl;
	cout<<2*r4a<<" Hz exit opposite face"<<endl;
	cout<<r3b*4+r4b*2<<" Hz exit bottom"<<endl;
	cout<<4*r3a+2*r4a+r3b*4+r4b*2<<" Hz total"<<endl;
	cout<<endl;


	// TOP HIT
	// scan x, y, theta, phi
	float r5a=0; float r5b=0; float r6a=0; float r6b=0; float r7a=0; float r7b=0; float r8a=0; float r8b=0; 
	dlx = W/int(W/dlnom); dly = T/int(T/dlnom); 
	// note: we'll scan from the corner to the center and have 4 cases - left side, right side, front side, back side. 
	// So the scan is only over 1/4 of the top surface area. Multiply rates accordingly. 
	for(x=dlx/2;x<W/2;x+=dlx)for(y=dly/2;y<T/2;y+=dly)for(phi_d=da_d/2;phi_d<360;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dly*da**2*(cos(theta))**3*sin(theta);
		// CASE 5: heading toward right side
		if(phi<atan((T-y)/(W-x))||phi>2*pi-atan((y)/(W-x))){
			//CASE 5A: exits right side
			if(theta>atan((W-x)/H/cos(phi))){
				r5a+=dR;
			}
			//CASE 5B: exits bottom
			else{
				r5b+=dR;
			}
		}
		// CASE 6: heading toward back side
		if(phi>atan((T-y)/(W-x))&&phi<pi-atan((T-y)/x)){
			//CASE 6A: exits back side
			if(theta>atan((T-y)/H/sin(phi))){
				r6a+=dR;
			}
			//CASE 6B: exits bottom
			else{
				r6b+=dR;
			}
		}
		// CASE 7: heading toward left side
		if(phi>pi-atan((T-y)/x)&&phi<pi+atan(y/x)){
			//CASE 7A: exits right side
			if(theta>atan(-x/H/cos(phi))){
				r7a+=dR;
			}
			//CASE 7B: exits bottom
			else{
				r7b+=dR;
			}
		}
		// CASE 8: heading toward front side
		if(phi>pi+atan(y/x)&&phi<2*pi-atan((y)/(W-x))){
			//CASE 8A: exits front side
			if(theta>atan(-y/H/sin(phi))){
				r8a+=dR;
			}
			//CASE 8B: exits bottom
			else{
				r8b+=dR;
			}
		}
	}
	cout<<"for muons entering top:"<<endl;
	cout<<endl;
	cout<<4*r5a+4*r7a<<" Hz exit a narrow side"<<endl;
	cout<<4*r6a+4*r8a<<" Hz exit a wide side"<<endl;
	cout<<4*(r5b+r6b+r7b+r8b)<<" Hz exit bottom"<<endl;
	cout<<4*r5a+4*r7a+4*r6a+4*r8a+4*(r5b+r6b+r7b+r8b)<<" Hz total"<<endl;
	cout<<endl;

	cout<<4*r1a+2*r2a+r1b*4+r2b*2+4*r3a+2*r4a+r3b*4+r4b*2+4*r5a+4*r7a+4*r6a+4*r8a+4*(r5b+r6b+r7b+r8b)<<" Hz grand total"<<endl;



}

void writeTree(){
	TTree * t = makeTree();
	TFile f("muontree.root","RECREATE");
	t->Write();
	f.Close();
}

TTree* makeTree() {
	// What we want for each muon path: z1, z2, theta, rate

	// Set up tree
	TTree * muontree = new TTree("muontree","muon rate calculations in NaI array");
	//
	float Z1, Z2, THETA, RATE;
float X1, Y1, PHI;
	//
	muontree->Branch("z1",&Z1,"z1/F");
	muontree->Branch("z2",&Z2,"z2/F");
	muontree->Branch("theta",&THETA,"theta/F");
	muontree->Branch("rate",&RATE,"rate/F");
muontree->Branch("x1",&X1,"x1/F");
muontree->Branch("y1",&Y1,"y1/F");
muontree->Branch("phi",&PHI,"phi/F");


	// vertical stack. approximation: no separation between detectors assumed. 
	// Detector width and thickness and height and number
	float W = 40.4;
	float T = 10.2; //float T = 10.2*3; 
	float Hdet = 5.7;
	float Ndet = 1; //float Ndet = 20;
	float H = Hdet*Ndet;
	//
	float pi = 2*asin(1);
	float C = 0.0083; // /cm2/sec/SR
	// variables for scanning
	float x, y, z, theta, phi, theta_d, phi_d;
	//for both dl and da_d, 5 is fast, 2 is a bit slower (~25 sec), 1 is default (slow, big file)
	float dlnom = 0.5; //cm
	float dlx, dly, dlz;
	float da_d = 1; // degrees
	float da = da_d/180.*pi;
	float dR;

	// FACE HIT
	// scan x, z, theta, phi
	dlx = W/int(W/dlnom); dlz = H/int(H/dlnom); 
	for(x=dlx/2;x<W;x+=dlx)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
if(gRandom->Rndm()<0.00001)cout<<"Face hit "<<x/W<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dlz*da**2*(cos(theta)*sin(theta))**2*sin(phi);
if(gRandom->Rndm()<0.00001)cout<<dR<<endl;
		// CASE 1: heading toward left(right) side
		//    note: situation is symmetrical for left versus right side, so we can just double the rates for this left-side calculation.
		if(phi<atan(T/x)){
			//CASE 1A: exits left(right) side
			if(theta>atan(x/z/cos(phi))){
				//multiplicity: 2 wide faces X 2 sides = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = z;
				Z2 = Z1 - x/cos(phi)/tan(theta);
X1=x; Y1=0; PHI=phi;
				muontree->Fill();
			}
			//CASE 1B: exits bottom
			if(theta<atan(x/z/cos(phi))){
				//multiplicity: 2 wide faces * 2 sides = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = z;
				Z2 = 0;
X1=x; Y1=0; PHI=phi;
				muontree->Fill();
			}
		}
		// CASE 2: heading toward opposite side
		if(phi>atan(T/x)&&phi<pi-atan(T/(W-x))){
			//CASE 2a: exits opposite side
			if(theta>atan(T/z/sin(phi))){
				//multiplicity: 2 wide faces = multiplicity 2
				RATE = 2*dR;
				THETA = theta;
				Z1 = z;
				Z2 = Z1 - T/sin(phi)/tan(theta);
X1=x; Y1=0; PHI=phi;
				muontree->Fill();
			}
			//CASE 2b: exits bottom
			if(theta<atan(T/z/sin(phi))){
				//multiplicity: 2 wide faces = multiplicity 2
				RATE = 2*dR;
				THETA = theta;
				Z1 = z;
				Z2 = 0;
X1=x; Y1=0; PHI=phi;
				muontree->Fill();
			}
		}
	}



	// EDGE HIT
	// scan x, z, theta, phi
	dlx = T/int(T/dlnom); dlz = H/int(H/dlnom); 
	for(x=dlx/2;x<T;x+=dlx)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
if(gRandom->Rndm()<0.00001)cout<<"Edge hit "<<x/T<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dlz*da**2*(cos(theta)*sin(theta))**2*sin(phi);
if(gRandom->Rndm()<0.00001)cout<<dR<<endl;
		// CASE 3: heading toward left(right) side
		//    note: situation is symmetrical for left versus right side, so we can just double the rates for this left-side calculation.
		if(phi<atan(W/x)){
			//CASE 3A: exits left(right) side
			if(theta>atan(x/z/cos(phi))){
				//multiplicity: 2 narrow faces X 2 sides = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = z;
				Z2 = Z1 - x/cos(phi)/tan(theta);
X1=0; Y1=x; PHI=phi;
				muontree->Fill();
			}
			//CASE 3B: exits bottom
			if(theta<atan(x/z/cos(phi))){
				//multiplicity: 2 narrow faces X 2 sides = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = z;
				Z2 = 0;
X1=0; Y1=x; PHI=phi;
				muontree->Fill();
			}
		}
		// CASE 4: heading toward opposite side
		if(phi>atan(W/x)&&phi<pi-atan(W/(T-x))){
			//CASE 4a: exits opposite side
			if(theta>atan(W/z/sin(phi))){
				//multiplicity: 2 narrow faces = multiplicity 2
				RATE = 2*dR;
				THETA = theta;
				Z1 = z;
				Z2 = Z1 - W/sin(phi)/tan(theta);
X1=0; Y1=x; PHI=phi;
				muontree->Fill();
			}
			//CASE 4b: exits bottom
			if(theta<atan(W/z/sin(phi))){
				//multiplicity: 2 narrow faces = multiplicity 2
				RATE = 2*dR;
				THETA = theta;
				Z1 = z;
				Z2 = 0;
X1=0; Y1=x; PHI=phi;
				muontree->Fill();
			}
		}
	}


	// TOP HIT
	// scan x, y, theta, phi
	// note: we'll scan from the corner to the center and have 4 cases - left side, right side, front side, back side. 
	// So the scan is only over 1/4 of the top surface area. Multiply rates accordingly. 
	dlx = W/int(W/dlnom); dly = T/int(T/dlnom); 
	for(x=dlx/2;x<W/2;x+=dlx)for(y=dly/2;y<T/2;y+=dly)for(phi_d=da_d/2;phi_d<360;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
if(gRandom->Rndm()<0.00001)cout<<"Top hit "<<x/W<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dly*da**2*(cos(theta))**3*sin(theta);
if(gRandom->Rndm()<0.00001)cout<<dR<<endl;
		// CASE 5: heading toward right side
		if(phi<atan((T-y)/(W-x))||phi>2*pi-atan((y)/(W-x))){
			//CASE 5A: exits right side
			if(theta>atan((W-x)/H/cos(phi))){
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = H-(W-x)/cos(phi)/tan(theta);
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
			//CASE 5B: exits bottom
			else{
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = 0;
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
		}
		// CASE 6: heading toward back side
		if(phi>atan((T-y)/(W-x))&&phi<pi-atan((T-y)/x)){
			//CASE 6A: exits back side
			if(theta>atan((T-y)/H/sin(phi))){
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = H-(T-y)/sin(phi)/tan(theta);
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
			//CASE 6B: exits bottom
			else{
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = 0;
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
		}
		// CASE 7: heading toward left side
		if(phi>pi-atan((T-y)/x)&&phi<pi+atan(y/x)){
			//CASE 7A: exits right side
			if(theta>atan(-x/H/cos(phi))){
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = H+x/cos(phi)/tan(theta);
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
			//CASE 7B: exits bottom
			else{
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = 0;
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
		}
		// CASE 8: heading toward front side
		if(phi>pi+atan(y/x)&&phi<2*pi-atan((y)/(W-x))){
			//CASE 8A: exits front side
			if(theta>atan(-y/H/sin(phi))){
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = H+y/sin(phi)/tan(theta);
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
			//CASE 8B: exits bottom
			else{
				//multiplicity: 4 quadrants = multiplicity 4
				RATE = 4*dR;
				THETA = theta;
				Z1 = H;
				Z2 = 0;
X1=x; Y1=x; PHI=phi;
				muontree->Fill();
			}
		}
	}
	return muontree;
}	

/*
muontree->SetAlias("d1","int((z1-1e-9)/5.7)+1")
muontree->SetAlias("d2","int((z2-1e-9)/5.7)+1")
// NaI: 4.785 MeV/cm
// min for through-going muon in 5.7cm thick paddle: 27.27 MeV
muontree->SetAlias("MeV_per_z_cm","4.785/cos(theta)")
muontree->SetAlias("h1","z1-5.7*(d1-1)")
muontree->SetAlias("h2","5.7*d2-z2")
muontree->SetAlias("e1","h1*MeV_per_z_cm")
muontree->SetAlias("e2","h2*MeV_per_z_cm")

muontree->Draw("d1-d2+1>>h(20,0,20)","rate")

muontree->Draw("e2>>he2(50,0,50)","rate")
muontree->Draw("e1","rate","same")

TCut cthresh = "d1-d2+1<3&&e1<27.27&&e2<27.27"

muontree->Draw("0","rate*(d1-d2+1<3&&e1<27.27&&e2<27.27)")
htemp->Integral() // (const Double_t)2.02626085281372070e+00

muontree->Draw("0","rate*(e1<27.27||e2<27.27)")


*/

void writeBotTree(){
	TTree * t = bot_makeTree();
	TFile f("muontree_bot.root","RECREATE");
	t->Write();
	f.Close();
}


float test(float x, float y, float z, float phi_d, float theta_d, int face) {
	// For the bottom array of 3 detectors
	// What we want for each muon path: x1, x2, y1, y2, z1, z2, rate

	// Set up tree
	//
	float X1, X2, Y1, Y2, Z1, Z2;

	// horizontal stack. 
	// Detector width and thickness and height and number
	float W = 10.2*3;
	float L = 40.4; 
	float H = 5.7;
	//
	float pi = 2*asin(1);
	float C = 0.0083; // /cm2/sec/SR
	// variables for scanning
	float theta, phi;
	//for both dl and da_d, 5 is fast, 2 is a bit slower (~25 sec), 1 is default (slow, big file)
	float dlnom = 0.5; //cm
	float dlx, dly, dlz;
	float da_d = 3; // degrees
	float da = da_d/180.*pi;
	float dR;

	// HIT ALONG WIDTH 
	// scan x, z, theta, phi
	dlx = W/int(W/dlnom); dlz = H/int(H/dlnom); 
	if(face==1){
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		// CASE 1: heading toward left side
		//    note: situation is symmetrical for left versus right side, and front and back.
		if(phi<atan(L/x)){
			//CASE 1A: exits left side
			if(theta>atan(x/z/cos(phi))){
				Z1 = z; Z2 = Z1 - x/cos(phi)/tan(theta);
				X1 = x; X2 = 0; Y1=0; Y2 = x*tan(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl; // left front
				X1 = W-x; X2 = W; Y1=0; Y2 = x*tan(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl; // right front
				X1 = x; X2 = 0; Y1=L; Y2 = L-x*tan(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl; // left back
				X1 = W-x; X2 = W; Y1=L; Y2 = L-x*tan(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl; // right back
			}
			//CASE 1B: exits bottom
			if(theta<atan(x/z/cos(phi))){
				Z1 = z; Z2 = 0;
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = 0; Y2 = z*tan(theta)*sin(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x+z*tan(theta)*cos(phi); Y1 = 0; Y2 = z*tan(theta)*sin(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = L; Y2 = L-z*tan(theta)*sin(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x+z*tan(theta)*cos(phi); Y1 = L; Y2 = L-z*tan(theta)*sin(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
			}
		}
		// CASE 2: heading toward opposite side
		if(phi>atan(L/x)&&phi<pi-atan(L/(W-x))){
			//CASE 2a: exits opposite side
			//   note: situation is symmetrical for front and back
			if(theta>atan(L/z/sin(phi))){
				Z1 = z; Z2 = Z1 - L/sin(phi)/tan(theta);
				X1 = x; X2 = x-L/tan(phi); Y1 = 0; Y2 = L; cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
				X1 = x; X2 = x-L/tan(phi); Y1 = L; Y2 = 0; cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
			}
			//CASE 2b: exits bottom
			if(theta<atan(L/z/sin(phi))){
				Z1 = z; Z2 = 0;
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = 0; Y2 = z*tan(theta)*sin(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = L; Y2 = L-z*tan(theta)*sin(phi); cout<<Form("%0.4f",X1)<<" "<<Form("%0.4f",Y1)<<" "<<Form("%0.4f",Z1)<<" "<<Form("%0.4f",X2)<<" "<<Form("%0.4f",Y2)<<" "<<Form("%0.4f",Z2)<<" "<<endl;
			}
		}
	}

return X2;

	// HIT ALONG LENGTH
	// scan y, z, theta, phi
	dly = L/int(L/dlnom); dlz = H/int(H/dlnom); 
	if(face==2){
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		// CASE 1: heading toward left side
		//    note: situation is symmetrical for left versus right side, and front and back.
		if(phi<atan(W/x)){
			//CASE 1A: exits left side
			if(theta>atan(y/z/cos(phi))){
				Z1 = z; Z2 = Z1 - y/cos(phi)/tan(theta);
				Y1 = y; Y2 = 0; X1=0; X2 = y*tan(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl; // left front
				Y1 = L-y; Y2 = L; X1=0; X2 = y*tan(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl; // right front
				Y1 = y; Y2 = 0; X1=W; X2 = W-y*tan(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl; // left back
				Y1 = L-y; Y2 = L; X1=W; X2 = W-y*tan(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl; // right back
			}
			//CASE 1B: exits bottom
			if(theta<atan(y/z/cos(phi))){
				Z1 = z; Z2 = 0;
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = 0; X2 = z*tan(theta)*sin(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				Y1 = L-y; Y2 = L-y+z*tan(theta)*cos(phi); X1 = 0; X2 = z*tan(theta)*sin(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = W; X2 = W-z*tan(theta)*sin(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				Y1 = L-y; Y2 = L-x+z*tan(theta)*cos(phi); X1 = W; X2 = W-z*tan(theta)*sin(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
		}
		// CASE 2: heading toward opposite side
		if(phi>atan(W/y)&&phi<pi-atan(W/(L-y))){
			//CASE 2a: exits opposite side
			//   note: situation is symmetrical for front and back
			if(theta>atan(W/z/sin(phi))){
				Z1 = z; Z2 = Z1 - W/sin(phi)/tan(theta);
				Y1 = y; Y2 = y-W/tan(phi); X1 = 0; X2 = W; cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				Y1 = y; Y2 = y-W/tan(phi); X1 = W; X2 = 0; cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
			//CASE 2b: exits bottom
			if(theta<atan(W/z/sin(phi))){
				Z1 = z; Z2 = 0;
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = 0; X2 = z*tan(theta)*sin(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = W; X2 = W-z*tan(theta)*sin(phi); cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
		}
	}


	// TOP HIT
	// scan x, y, theta, phi
	// note: we'll scan from the corner to the center and have 4 cases - left side, right side, front side, back side. 
	// So the scan is only over 1/4 of the top surface area.  
	dlx = W/2/int(W/2/dlnom); dly = L/2/int(L/2/dlnom); 
	if(face==3){
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		// CASE 5: heading toward right side
		if(phi<atan((L-y)/(W-x))||phi>2*pi-atan((y)/(W-x))){
			//CASE 5A: exits right side
			if(theta>atan((W-x)/H/cos(phi))){
				Z1 = H;	Z2 = H-(W-x)/cos(phi)/tan(theta);
				X1 = x; X2 = W; Y1 = y; Y2 = y+(W-x)*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = W; Y1 = L-y; Y2 = L-y-(W-x)*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = 0; Y1 = y; Y2 = y+(W-x)*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = 0; Y1 = L-y; Y2 = L-y-(W-x)*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
			//CASE 5B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
		}
		// CASE 6: heading toward back side
		if(phi>atan((L-y)/(W-x))&&phi<pi-atan((L-y)/x)){
			//CASE 6A: exits back side
			if(theta>atan((L-y)/H/sin(phi))){
				Z1 = H; Z2 = H-(L-y)/sin(phi)/tan(theta);
				X1 = x; X2 = x+(L-y)/tan(phi); Y1 = y; Y2 = L;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = x+(L-y)/tan(phi); Y1 = L-y; Y2 = 0;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-(L-y)/tan(phi); Y1 = y; Y2 = L;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-(L-y)/tan(phi); Y1 = L-y; Y2 = 0;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
			//CASE 6B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
		}
		// CASE 7: heading toward left side
		if(phi>pi-atan((L-y)/x)&&phi<pi+atan(y/x)){
			//CASE 7A: exits left side
			if(theta>atan(-x/H/cos(phi))){
				Z1 = H; Z2 = H+x/cos(phi)/tan(theta);
				X1 = x; X2 = 0; Y1 = y; Y2 = y-x*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = 0; Y1 = L-y; Y2 = L-y+x*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W; Y1 = y; Y2 = y-x*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W; Y1 = L-y; Y2 = L-y+x*tan(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
			//CASE 7B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
		}
		// CASE 8: heading toward front side
		if(phi>pi+atan(y/x)&&phi<2*pi-atan((y)/(W-x))){
			//CASE 8A: exits front side
			if(theta>atan(-y/H/sin(phi))){
				Z1 = H; Z2 = H+y/sin(phi)/tan(theta);
				X1 = x; X2 = x-y/tan(phi); Y1 = y; Y2 = 0;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = x-y/tan(phi); Y1 = L-y; Y2 = L;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x+y/tan(phi); Y1 = y; Y2 = 0;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x+y/tan(phi); Y1 = L-y; Y2 = L;	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
			//CASE 8B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	cout<<Form("%0.2f",X1)<<" "<<Form("%0.2f",Y1)<<" "<<Form("%0.2f",Z1)<<" "<<Form("%0.2f",X2)<<" "<<Form("%0.2f",Y2)<<" "<<Form("%0.2f",Z2)<<" "<<endl;
			}
		}
	}
}	



/*
// NaI: 4.785 MeV/cm
// min for through-going muon in 5.7cm thick paddle: 27.27 MeV
muontree->SetAlias("l","sqrt((x1-x2)**2+(y1-y2)**2+(z1-z2)**2)")
muontree->SetAlias("l1","(x1<10.2&&x2>10.2)*(10.2-x1)/(x2-x1)*l+(x2<10.2&&x1>10.2)*(10.2-x2)/(x1-x2)*l+(x2<10.2&&x1<10.2)*l")
muontree->SetAlias("l3","(x1<20.4&&x2>20.4)*(x2-20.4)/(x2-x1)*l+(x2<20.4&&x1>20.4)*(x1-20.4)/(x1-x2)*l+(x2>20.4&&x1>20.4)*l")

muontree->SetAlias("e","l*4.785")

muontree->SetAlias("e1","l1*4.785")
muontree->SetAlias("e3","l3*4.785")
muontree->SetAlias("e2","e-e1-e3")


*/


TTree* bot_makeTree() {
	// For the bottom array of 3 detectors
	// What we want for each muon path: x1, x2, y1, y2, z1, z2, rate

	// Set up tree
	TTree * muontree = new TTree("muontree","muon rate calculations in NaI array");
	//
	float X1, X2, Y1, Y2, Z1, Z2;
	double RATE;
	//
	muontree->Branch("x1",&X1,"x1/F");
	muontree->Branch("y1",&Y1,"y1/F");
	muontree->Branch("z1",&Z1,"z1/F");
	muontree->Branch("x2",&X2,"x2/F");
	muontree->Branch("y2",&Y2,"y2/F");
	muontree->Branch("z2",&Z2,"z2/F");
	muontree->Branch("rate",&RATE,"rate/D");

	// horizontal stack. 
	// Detector width and thickness and height and number
	float W = 10.2*3;
	float L = 40.4; 
	float H = 5.7;
	//
	float pi = 2*asin(1);
	float C = 0.0083; // /cm2/sec/SR
	// variables for scanning
	float x, y, z, theta, phi, theta_d, phi_d;
	//for both dl and da_d, 5 is fast, 2 is a bit slower (~25 sec), 1 is default (slow, big file)
	float dlnom = 0.5; //cm
	float dlx, dly, dlz;
	float da_d = 3; // degrees
	float da = da_d/180.*pi;
	float dR;

	// HIT ALONG WIDTH 
	// scan x, z, theta, phi
int ctr = 0;
	dlx = W/int(W/dlnom); dlz = H/int(H/dlnom); 
	for(x=dlx/2;x<W;x+=dlx)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
//	for(x=dlx/2;x<dlx;x+=dlx)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
if(z==dlz/2&&phi_d==da_d/2&&theta_d==da_d/2)cout<<"Width hit "<<x/W<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dlz*da**2*(cos(theta)*sin(theta))**2*sin(phi);
		RATE = dR;
		// CASE 1: heading toward left side
		//    note: situation is symmetrical for left versus right side, and front and back.
		if(phi<atan(L/x)){
			//CASE 1A: exits left side
			if(theta>atan(x/z/cos(phi))){
				Z1 = z; Z2 = Z1 - x/cos(phi)/tan(theta);
				X1 = x; X2 = 0; Y1=0; Y2 = x*tan(phi); muontree->Fill(); // left front
				X1 = W-x; X2 = W; Y1=0; Y2 = x*tan(phi); muontree->Fill(); // right front
				X1 = x; X2 = 0; Y1=L; Y2 = L-x*tan(phi); muontree->Fill(); // left back
				X1 = W-x; X2 = W; Y1=L; Y2 = L-x*tan(phi); muontree->Fill(); // right back
			}
			//CASE 1B: exits bottom
			if(theta<atan(x/z/cos(phi))){
				Z1 = z; Z2 = 0;
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = 0; Y2 = z*tan(theta)*sin(phi); muontree->Fill();
				X1 = W-x; X2 = W-x+z*tan(theta)*cos(phi); Y1 = 0; Y2 = z*tan(theta)*sin(phi); muontree->Fill();
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = L; Y2 = L-z*tan(theta)*sin(phi); muontree->Fill();
				X1 = W-x; X2 = W-x+z*tan(theta)*cos(phi); Y1 = L; Y2 = L-z*tan(theta)*sin(phi); muontree->Fill();
			}
		}
		// CASE 2: heading toward opposite side
		if(phi>atan(L/x)&&phi<pi-atan(L/(W-x))){
			//CASE 2a: exits opposite side
			//   note: situation is symmetrical for front and back
			if(theta>atan(L/z/sin(phi))){
				Z1 = z; Z2 = Z1 - L/sin(phi)/tan(theta);
				X1 = x; X2 = x-L/tan(phi); Y1 = 0; Y2 = L; muontree->Fill();
				X1 = x; X2 = x-L/tan(phi); Y1 = L; Y2 = 0; muontree->Fill();
			}
			//CASE 2b: exits bottom
			if(theta<atan(L/z/sin(phi))){
				Z1 = z; Z2 = 0;
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = 0; Y2 = z*tan(theta)*sin(phi); muontree->Fill();
				X1 = x; X2 = x-z*tan(theta)*cos(phi); Y1 = L; Y2 = L-z*tan(theta)*sin(phi); muontree->Fill();
			}
		}
	}

	// HIT ALONG LENGTH
	// scan y, z, theta, phi
	dly = L/int(L/dlnom); dlz = H/int(H/dlnom); 
	for(y=dly/2;y<L;y+=dly)for(z=dlz/2;z<H;z+=dlz)for(phi_d=da_d/2;phi_d<180;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
//if(gRandom->Rndm()<0.00001)cout<<"Length hit "<<y/L<<endl;
if(z==dlz/2&&phi_d==da_d/2&&theta_d==da_d/2)cout<<"Length hit "<<y/L<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dly*dlz*da**2*(cos(theta)*sin(theta))**2*sin(phi);
		RATE = dR;
		// CASE 1: heading toward left side
		//    note: situation is symmetrical for left versus right side, and front and back.
		if(phi<atan(W/y)){
			//CASE 1A: exits left side
			if(theta>atan(y/z/cos(phi))){
				Z1 = z; Z2 = Z1 - y/cos(phi)/tan(theta);
				Y1 = y; Y2 = 0; X1=0; X2 = y*tan(phi); muontree->Fill();
				Y1 = L-y; Y2 = L; X1=0; X2 = y*tan(phi); muontree->Fill();
				Y1 = y; Y2 = 0; X1=W; X2 = W-y*tan(phi); muontree->Fill();
				Y1 = L-y; Y2 = L; X1=W; X2 = W-y*tan(phi); muontree->Fill();
			}
			//CASE 1B: exits bottom
			if(theta<atan(y/z/cos(phi))){
				Z1 = z; Z2 = 0;
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = 0; X2 = z*tan(theta)*sin(phi); muontree->Fill();
				Y1 = L-y; Y2 = L-y+z*tan(theta)*cos(phi); X1 = 0; X2 = z*tan(theta)*sin(phi); muontree->Fill();
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = W; X2 = W-z*tan(theta)*sin(phi); muontree->Fill();
				Y1 = L-y; Y2 = L-y+z*tan(theta)*cos(phi); X1 = W; X2 = W-z*tan(theta)*sin(phi); muontree->Fill();
			}
		}
		// CASE 2: heading toward opposite side
		if(phi>atan(W/y)&&phi<pi-atan(W/(L-y))){
			//CASE 2a: exits opposite side
			//   note: situation is symmetrical for front and back
			if(theta>atan(W/z/sin(phi))){
				Z1 = z; Z2 = Z1 - W/sin(phi)/tan(theta);
				Y1 = y; Y2 = y-W/tan(phi); X1 = 0; X2 = W; muontree->Fill();
				Y1 = y; Y2 = y-W/tan(phi); X1 = W; X2 = 0; muontree->Fill();
			}
			//CASE 2b: exits bottom
			if(theta<atan(W/z/sin(phi))){
				Z1 = z; Z2 = 0;
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = 0; X2 = z*tan(theta)*sin(phi); muontree->Fill();
				Y1 = y; Y2 = y-z*tan(theta)*cos(phi); X1 = W; X2 = W-z*tan(theta)*sin(phi); muontree->Fill();
			}
		}
	}

	// TOP HIT
	// scan x, y, theta, phi
	// note: we'll scan from the corner to the center and have 4 cases - left side, right side, front side, back side. 
	// So the scan is only over 1/4 of the top surface area.  
	dlx = W/2/int(W/2/dlnom); dly = L/2/int(L/2/dlnom); 
	for(x=dlx/2;x<W/2;x+=dlx)for(y=dly/2;y<L/2;y+=dly)for(phi_d=da_d/2;phi_d<360;phi_d+=da_d)for(theta_d=da_d/2;theta_d<90;theta_d+=da_d){
//if(gRandom->Rndm()<0.00001)cout<<"Top hit "<<2*x/W<<endl;
if(y==dly/2&&phi_d==da_d/2&&theta_d==da_d/2)cout<<"Top hit "<<2*x/W<<endl;
		phi = phi_d/180.*pi; theta = theta_d/180.*pi;
		//Rate for this element of area and solid angle:
		dR = C*dlx*dly*da**2*(cos(theta))**3*sin(theta);
		RATE = dR;
		// CASE 5: heading toward right side
		if(phi<atan((L-y)/(W-x))||phi>2*pi-atan((y)/(W-x))){
			//CASE 5A: exits right side
			if(theta>atan((W-x)/H/cos(phi))){
				Z1 = H;	Z2 = H-(W-x)/cos(phi)/tan(theta);
				X1 = x; X2 = W; Y1 = y; Y2 = y+(W-x)*tan(phi);	muontree->Fill();
				X1 = x; X2 = W; Y1 = L-y; Y2 = L-y-(W-x)*tan(phi);	muontree->Fill();
				X1 = W-x; X2 = 0; Y1 = y; Y2 = y+(W-x)*tan(phi);	muontree->Fill();
				X1 = W-x; X2 = 0; Y1 = L-y; Y2 = L-y-(W-x)*tan(phi);	muontree->Fill();
			}
			//CASE 5B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
			}
		}
		// CASE 6: heading toward back side
		if(phi>atan((L-y)/(W-x))&&phi<pi-atan((L-y)/x)){
			//CASE 6A: exits back side
			if(theta>atan((L-y)/H/sin(phi))){
				Z1 = H; Z2 = H-(L-y)/sin(phi)/tan(theta);
				X1 = x; X2 = x+(L-y)/tan(phi); Y1 = y; Y2 = L;	muontree->Fill();
				X1 = x; X2 = x+(L-y)/tan(phi); Y1 = L-y; Y2 = 0;	muontree->Fill();
				X1 = W-x; X2 = W-x-(L-y)/tan(phi); Y1 = y; Y2 = L;	muontree->Fill();
				X1 = W-x; X2 = W-x-(L-y)/tan(phi); Y1 = L-y; Y2 = 0;	muontree->Fill();
			}
			//CASE 6B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
			}
		}
		// CASE 7: heading toward left side
		if(phi>pi-atan((L-y)/x)&&phi<pi+atan(y/x)){
			//CASE 7A: exits left side
			if(theta>atan(-x/H/cos(phi))){
				Z1 = H; Z2 = H+x/cos(phi)/tan(theta);
				X1 = x; X2 = 0; Y1 = y; Y2 = y-x*tan(phi);	muontree->Fill();
				X1 = x; X2 = 0; Y1 = L-y; Y2 = L-y+x*tan(phi);	muontree->Fill();
				X1 = W-x; X2 = W; Y1 = y; Y2 = y-x*tan(phi);	muontree->Fill();
				X1 = W-x; X2 = W; Y1 = L-y; Y2 = L-y+x*tan(phi);	muontree->Fill();
			}
			//CASE 7B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
			}
		}
		// CASE 8: heading toward front side
		if(phi>pi+atan(y/x)&&phi<2*pi-atan((y)/(W-x))){
			//CASE 8A: exits front side
			if(theta>atan(-y/H/sin(phi))){
				Z1 = H; Z2 = H+y/sin(phi)/tan(theta);
				X1 = x; X2 = x-y/tan(phi); Y1 = y; Y2 = 0;	muontree->Fill();
				X1 = x; X2 = x-y/tan(phi); Y1 = L-y; Y2 = L;	muontree->Fill();
				X1 = W-x; X2 = W-x+y/tan(phi); Y1 = y; Y2 = 0;	muontree->Fill();
				X1 = W-x; X2 = W-x+y/tan(phi); Y1 = L-y; Y2 = L;	muontree->Fill();
			}
			//CASE 8B: exits bottom
			else{
				Z1 = H; Z2 = 0;
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = x; X2 = x+H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = y; Y2 = y+H*tan(theta)*sin(phi);	muontree->Fill();
				X1 = W-x; X2 = W-x-H*tan(theta)*cos(phi); Y1 = L-y; Y2 = L-y-H*tan(theta)*sin(phi);	muontree->Fill();
			}
		}
	}

	return muontree;
}	







