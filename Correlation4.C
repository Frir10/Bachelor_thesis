{

  gROOT->Reset();

  Double_t v2r = 0.05; // Set v2
  Int_t e = 10000;    // Antal events i min sample
  Int_t p = 500;      // Antal partikler i hvert event

  Double_t meassuredphi[p]={0.};

  // Functions:
  // Flat distribution
  gRandom = new TRandom3(0);

  // Function to get azimuthal distribution
  TF1 *f = new TF1("pdf","1/(2*(TMath::Pi()))*(1+2*[0]*cos(2*(x-[1])))",0,2*(TMath::Pi()));
  f->SetParameter(0,v2r);


  // Make loop that adds the contributions from e events
  TH1D *h2 = new TH1D("h2","MultipleeventsHistogram",1000,-0.02,0.08);

  for (Int_t j = 0; j<e; j++) {

  // Get random event plane angle between 0 and 2 Pi
  Double_t psi = gRandom->Uniform(0.,2.*TMath::TwoPi());
  f->SetParameter(1,psi);

  Double_t Qsum[3] = {0.}; // Two-dimensional vector containing the real and imagenary
                          // parts of the Q-vector

  // Get azimuthal angles and Q-vector:
  for (Int_t i = 0; i<p; i++) {
    meassuredphi[i] = f->GetRandom();
    Qsum[0]+ = cos(2*meassuredphi[i]);     // Real part
    Qsum[1]+ = sin(2*meassuredphi[i]);     // Imagenary part
    Qsum[2]+ = i;
  }

  // The 2-particle correlation is then:

  //Double_t correlation = ((Qsum[0]*Qsum[0])+(Qsum[1]*Qsum[1])-p)/(p*(p-1));

  // Fill the correlations into the histogram, to get the average:

  h2->Fill(((Qsum[0]*Qsum[0])+(Qsum[1]*Qsum[1])-p)/(p*(p-1)));

  }  // End of j-loop

  delete gRandom;
  delete f;

  h2->Draw();
  Double_t v2 = sqrt(h2->GetMean());
  cout << "The meassured value of v2 is " << v2 << endl;
  cout << "while the real value was " << v2r << endl;

}
