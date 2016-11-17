{
  gROOT->Reset();

  // Input arrays:

  Int_t nEntries = 3;
  Double_t v2BinPosition[(nEntries*50)] = {0};
  Double_t v2BinContent[(nEntries*50)] = {0};
  Double_t v2BinError[(nEntries*50)] = {0};


  gSystem->Load("libPWGflowBase");
  //gSystem->Load("libPWGFlowTasks");

// Set how many analyses in total you want to access:
const Int_t nAnalyses = nEntries;


// Set absolute path to directory containing analyses files:
TString absolutePath[nAnalyses] = { 
"/home/lkc538",
"/home/lkc538",
"/home/lkc538"
}; 

 
// Set common output file name:
 TString commonOutputFileName[nAnalyses] = {"AnalysisResults.root","AnalysisResults.root","AnalysisResults.root"};

// Set common TDirectoryFile name:
TString commonTDirectoryFileName[nAnalyses] = {"outputQCanalysis","outputQCanalysis","outputQCanalysis"};

// Set common TList name:
TString commonTListName[nAnalyses] = {
"TaskQC <= v2 <= 10.0-20.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 20.0-30.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 30.0-40.0_eventCuts_kV0_cutsRP_cutsPOI"
};



for(Int_t n=0;n<nAnalyses;n++) // loop over all analyses
 {
  TFile *commonOutputFile = NULL; 
  if(!(gSystem->AccessPathName(Form("%s/%s",absolutePath[n].Data(),commonOutputFileName[n].Data()),kFileExists)))
  {
   commonOutputFile = TFile::Open(Form("%s/%s",absolutePath[n].Data(),commonOutputFileName[n].Data()),"READ"); 
  } else 
    {
     cout<<endl;
     cout<<" WARNING: Couldn't find the file "<<Form("%s/%s",absolutePath[n].Data(),commonOutputFileName[n].Data())<<" !!!!"<<endl;
     cout<<endl;
    }
  TDirectoryFile *dirFile = NULL; 
  if(commonOutputFile){dirFile = (TDirectoryFile*)commonOutputFile->FindObjectAny(commonTDirectoryFileName[n].Data());} 
  if(commonOutputFile && !dirFile){cout<<"dirFile is NULL"<<endl;exit(0);}
  if(dirFile) // still needed, despite above check!
  {
   TList *listOfKeys = dirFile->GetListOfKeys();
   if(!listOfKeys){cout<<"listOfKeys is NULL"<<endl;exit(0);}
   for(Int_t l=0;l<listOfKeys->GetEntries();l++)
   {

     //cout<<listOfKeys->At(l)->GetName()<<endl;
     //cout<<commonTListName[n].Data()<<endl;

    if(TString(listOfKeys->At(l)->GetName()).EqualTo(commonTListName[n].Data()))
    {
     TList *commonList = NULL;
     TList *listIntFlow = NULL;
     TList *listResults = NULL;
     TString listName = listOfKeys->At(l)->GetName();
     dirFile->GetObject(listName.Data(),commonList);
     if(!commonList){cout<<"commonList is NULL"<<endl;exit(0);}
     cout<<endl;
     cout<<" Accessing objects of TList "<<commonList->GetName()<<endl;
     cout<<"in the file "<<commonOutputFile->GetName()<<endl;
     listIntFlow = dynamic_cast<TList*> commonList->FindObject("Differential Flow");
     if(!listIntFlow){cout<<"listIntFlow is NULL"<<endl;exit(0);}
     listResults = dynamic_cast<TList*> listIntFlow->FindObject("Results");
     if(!listResults){cout<<"listResults is NULL"<<endl;}
     listDifferential = dynamic_cast<TList*> listResults->FindObject("Differential flow (RP, p_{T})");
     if(!listDifferential){cout<<"listDifferential is NULL"<<endl;}

      cout << "nej" << endl;
      TH1D *fIntFlow = dynamic_cast<TH1D*> listDifferential->FindObject("fDiffFlow, RP, p_{T}, v'{4}");
      cout << "hej" << endl;

      for (Int_t i = 0; i<50; i++) {

        v2BinPosition[(i+50*n)] = fIntFlow->GetBinCenter(3+i);	//v2BinPosition[n] = fIntFlow->GetBinCenter(3);
        v2BinContent[(i+50*n)] = fIntFlow->GetBinContent(3+i);
        v2BinError[(i+50*n)] = fIntFlow->GetBinError(3+i);
      }
      fIntFlow->Draw();

    }
   }
  }
 }

 Double_t v2NewContent[(20*nEntries)] = {0.};
 Double_t v2NewPosition[(20*nEntries)] = {0.};
 Double_t v2NewError[(20*nEntries)] = {0.};

for (Int_t m = 0; m<nEntries; m++) {
  // First the 8 unchanged bins below 1.0:
  for (Int_t o = 0; o<8; o++) {
    v2NewContent[((m*20)+o)] = v2BinContent[(o+(50*m))];
    v2NewPosition[((m*20)+o)] = v2BinPosition[(o+(50*m))];
    v2NewError[((m*20)+o)] = v2BinError[(o+(50*m))];
    }

  // 5 bins from 1.0 to 2.0:
  for (Int_t p = 0; p<5; p++) {
    v2NewContent[8+20*m+p] = ((v2BinContent[(8+(2*p)+(50*m))]/pow(v2BinError[(8+(2*p)+(50*m))],2))+(v2BinContent[(9+(2*p)+(50*m))]/pow(v2BinError[(9+(2*p)+(50*m))],2)))/(1./pow(v2BinError[(8+(2*p)+(50*m))],2)+1./pow(v2BinError[(9+(2*p)+(50*m))],2));
    v2NewPosition[8+20*m+p] = (v2BinPosition[(8+(2*p)+(50*m))]+v2BinPosition[(9+(2*p)+(50*m))])/2.;
    v2NewError[8+20*m+p] = sqrt(1./(1./pow(v2BinError[(8+(2*p)+(50*m))],2)+1./pow(v2BinError[(9+(2*p)+(50*m))],2)));
    }

  // 4 bins from 2.0 to 4.0:
  for (Int_t q = 0; q<4; q++) {
    v2NewContent[13+20*m+q] = ((v2BinContent[(18+(5*q)+(50*m))]/pow(v2BinError[(18+(5*q)+(50*m))],2))+(v2BinContent[(19+(5*q)+(50*m))]/pow(v2BinError[(19+(5*q)+(50*m))],2))+(v2BinContent[(20+(5*q)+(50*m))]/pow(v2BinError[(20+(5*q)+(50*m))],2))+(v2BinContent[(21+(5*q)+(50*m))]/pow(v2BinError[(21+(5*q)+(50*m))],2))+(v2BinContent[(22+(5*q)+(50*m))]/pow(v2BinError[(22+(5*q)+(50*m))],2)))/(1./pow(v2BinError[(18+(5*q)+(50*m))],2)+1./pow(v2BinError[(19+(5*q)+(50*m))],2)+1./pow(v2BinError[(20+(5*q)+(50*m))],2)+1./pow(v2BinError[(21+(5*q)+(50*m))],2)+1./pow(v2BinError[(22+(5*q)+(50*m))],2));
    v2NewPosition[13+20*m+q] = (v2BinPosition[(18+(5*q)+(50*m))]+v2BinPosition[(19+(5*q)+(50*m))]+v2BinPosition[(20+(5*q)+(50*m))]+v2BinPosition[(21+(5*q)+(50*m))]+v2BinPosition[(22+(5*q)+(50*m))])/5.;
    v2NewError[13+20*m+q] = sqrt(1./(1./pow(v2BinError[(18+(5*q)+(50*m))],2)+1./pow(v2BinError[(19+(5*q)+(50*m))],2)+1./pow(v2BinError[(20+(5*q)+(50*m))],2)+1./pow(v2BinError[(21+(5*q)+(50*m))],2)+1./pow(v2BinError[(22+(5*q)+(50*m))],2)));
    }

  // 1 bin for all the values from 4.0 to 5.0:
  v2NewContent[13+20*m+q] = ((v2BinContent[(38+(50*m))]/pow(v2BinError[(38+(50*m))],2))+(v2BinContent[(39+(50*m))]/pow(v2BinError[(39+(50*m))],2))+(v2BinContent[(40+(50*m))]/pow(v2BinError[(40+(50*m))],2))+(v2BinContent[(41+(50*m))]/pow(v2BinError[(41+(50*m))],2))+(v2BinContent[(42+(50*m))]/pow(v2BinError[(42+(50*m))],2))+(v2BinContent[(43+(50*m))]/pow(v2BinError[(43+(50*m))],2))+(v2BinContent[(44+(50*m))]/pow(v2BinError[(44+(50*m))],2))+(v2BinContent[(45+(50*m))]/pow(v2BinError[(45+(50*m))],2))+(v2BinContent[(46+(50*m))]/pow(v2BinError[(46+(50*m))],2))+(v2BinContent[(47+(50*m))]/pow(v2BinError[(47+(50*m))],2)))/(1./pow(v2BinError[(38+(50*m))],2)+1./pow(v2BinError[(39+(50*m))],2)+1./pow(v2BinError[(40+(50*m))],2)+1./pow(v2BinError[(41+(50*m))],2)+1./pow(v2BinError[(42+(50*m))],2)+1./pow(v2BinError[(43+(50*m))],2)+1./pow(v2BinError[(44+(50*m))],2)+1./pow(v2BinError[(45+(50*m))],2)+1./pow(v2BinError[(46+(50*m))],2)+1./pow(v2BinError[(47+(50*m))],2));
  v2NewPosition[17+20*m] = (v2BinPosition[(38+(50*m))]+v2BinPosition[(39+(50*m))]+v2BinPosition[(40+(50*m))]+v2BinPosition[(41+(50*m))]+v2BinPosition[(42+(50*m))]+v2BinPosition[(43+(50*m))]+v2BinPosition[(44+(50*m))]+v2BinPosition[(45+(50*m))]+v2BinPosition[(46+(50*m))]+v2BinPosition[(47+(50*m))])/10.;
  v2NewError[17+20*m] = sqrt(1./(1./pow(v2BinError[(38+(50*m))],2)+1./pow(v2BinError[(39+(50*m))],2)+1./pow(v2BinError[(40+(50*m))],2)+1./pow(v2BinError[(41+(50*m))],2)+1./pow(v2BinError[(42+(50*m))],2)+1./pow(v2BinError[(43+(50*m))],2)+1./pow(v2BinError[(44+(50*m))],2)+1./pow(v2BinError[(45+(50*m))],2)+1./pow(v2BinError[(46+(50*m))],2)+1./pow(v2BinError[(47+(50*m))],2)));

 }

 TH1D *histo1020 = new TH1D("histo1020","v2 as a function of transverse momentum",100,0,5);
 TH1D *histo2030 = new TH1D("histo2030","v2 as a function of transverse momentum",100,0,5);
 TH1D *histo3040 = new TH1D("histo3040","v_{2}{4} as a function of transverse momentum",100,0,5);

 Int_t BinNumber[18] = {6,8,10,12,14,16,18,20,23,27,31,35,39,46,56,66,76,91};
 Int_t BinNumber2[18] = {7,9,11,13,15,17,19,21,24,28,32,36,40,47,57,67,77,92};

 for (Int_t j = 0; j<18; j++) {

   //Fyld alle tre her
   histo1020->Fill(v2NewPosition[j],v2NewContent[j]);
   histo2030->SetBinContent(BinNumber2[j],v2NewContent[j+20]);
   histo3040->Fill(v2NewPosition[j+40],v2NewContent[j+40]);
   histo1020->SetBinError(BinNumber[j],v2NewError[j]);
   histo2030->SetBinError(BinNumber2[j],v2NewError[j+20]);
   histo3040->SetBinError(BinNumber[j],v2NewError[j+40]);

 }
 histo1020->SetMarkerStyle(20);
 histo1020->SetMarkerColor(4);
 histo1020->SetMarkerSize(2);
 histo2030->SetMarkerStyle(21);
 histo2030->SetMarkerColor(2);
 histo2030->SetMarkerSize(2);
 histo3040->SetMarkerStyle(22);
 histo3040->SetMarkerColor(kGreen+1);
 histo3040->SetMarkerSize(2);

 histo3040->SetXTitle("p_{t} GeV/c");
 histo3040->SetYTitle("v_{2}{4}");

 histo3040->Draw("P,E1");
 histo2030->Draw("same,P,E1");
 histo1020->Draw("same,P,E1");

 TLegend *legend = new TLegend(0.65,0.25,0.9,0.48);
 legend->AddEntry(histo1020,"10-20%","P");
 legend->AddEntry(histo2030,"20-30%","P");
 legend->AddEntry(histo3040,"30-40%","P");
 legend->Draw();

 for (Int_t k = 0; k<60; k++) {
 cout << k << " " << v2NewContent[k] << " " << v2NewError[k] << endl;

 }
 

}
