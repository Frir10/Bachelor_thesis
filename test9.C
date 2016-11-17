{

  gROOT->Reset();

  Double_t v2r = 0.05; // Set v2
  Int_t e = 1000;    // Antal events i min sample
  Int_t M = 10;      // Antal partikler i hvert event

  Double_t meassuredphi[M]={0.};

  // Functions:
  // Flat distribution
  gRandom = new TRandom3(0);

  // Function to get azimuthal distribution
  TF1 *f = new TF1("pdf","1/(2*(TMath::Pi()))*(1+2*[0]*cos(2*(x-[1])))",0,2*(TMath::Pi()));
  f->SetParameter(0,v2r);


  // Make loop that adds the contributions from e events
  TH1D *h2 = new TH1D("h2","Q-vector Histogram",10000,-1.,1.);
  TH1D *h = new TH1D("h3","Nested loops Histogram",1000,-1.1,1.1);

  for (Int_t j = 0; j<e; j++) {

  // Get random event plane angle between 0 and 2 Pi
  Double_t psi = gRandom->Uniform(0.,2.*TMath::TwoPi());
  f->SetParameter(1,psi);

  // Get random multiplicity below M:
  Int_t p = gRandom->Uniform(4,M);

  Double_t Qsum[4] = {0.}; // Four-dimensional vector containing the real and imagenary
                           // parts of the Qn-vector and Q2n vector

  // Get azimuthal angles and Q-vector:

  for (Int_t i = 0; i<p; i++) {
    meassuredphi[i] = f->GetRandom();
    Qsum[0]+ = cos(2*meassuredphi[i]);     // Real part
    Qsum[1]+ = sin(2*meassuredphi[i]);     // Imagenary part
    Qsum[2]+ = cos(4*meassuredphi[i]);     // real part of Q2n
    Qsum[3]+ = sin(4*meassuredphi[i]);     // imagenary part of Q2n
  }

  Double_t Qsize = (pow(Qsum[0],2)+pow(Qsum[1],2));
  Double_t Q2size = (pow(Qsum[2],2)+pow(Qsum[3],2));

  // The 4-particle correlation is then:
  Double_t QC = (pow(Qsize,2) + Q2size - 2*(Qsum[2]*(pow(Qsum[0],2)-pow(Qsum[1],2))+2*Qsum[3]*Qsum[0]*Qsum[1]) - 4*(p-2)*Qsize + 2*p*(p-3))/(p*(p-1)*(p-2)*(p-3));
  Double_t Multiplicity = (p*(p-1)*(p-2)*(p-3));

  h2->Fill(QC,Multiplicity);

  // Nested loops:
  for (Int_t l = 0; l<p; l++) {
    for (Int_t m = 0; m<p; m++) {
      if (l == m) {
	continue;
      } else {
        for (Int_t n = 0; n<p; n++) {
      if (l == n) {
	continue;
      } else if (m == n) {
	continue;
      } else {
        for (Int_t o = 0; o<p; o++) {
      if (l == o) {
	continue;
      } else if (m == o) {
	continue;
      } else if (n == o) {
	continue;
      } else {
	  h->Fill(cos(2*(meassuredphi[l]+meassuredphi[m]-meassuredphi[n]-meassuredphi[o])));
      }
      }
      }
      }
      }
    }
  }

  } // End of j-loop

  TCanvas *MyC = new TCanvas("MyC","MyC",1);
  MyC->Divide(2,1);
  MyC->cd(1);
  h->Draw();
  MyC->cd(2);
  h2->Draw();

  Double_t Nested = h->GetMean();
  Double_t QCC = h2->GetMean();

  cout << "The QC value for the correlation is " << QCC << endl;
  cout << "while the value found with nested loops is " << Nested << endl;

  delete gRandom;
  delete f;

}
