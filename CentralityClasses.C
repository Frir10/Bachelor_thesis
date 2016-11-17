{
  gROOT->Reset();

  // Input arrays:

  Int_t nEntries = 20;
  Double_t v2[nEntries] = {0.};
  Double_t v2Error[nEntries] = {0.};
  Double_t v4[nEntries] = {0.};
  Double_t v4Error[nEntries] = {0.};

  gSystem->Load("libPWGflowBase");
  //gSystem->Load("libPWGFlowTasks");

// Set how many analyses in total you want to access:
const Int_t nAnalyses = nEntries;


// Set absolute path to directory containing analyses files:
TString absolutePath[nAnalyses] = { 
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538",
"/home/lkc538"
}; 

 
// Set common output file name:
 TString commonOutputFileName[nAnalyses] = {"AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root","AnalysisResults.root"};

// Set common TDirectoryFile name:
 TString commonTDirectoryFileName[nAnalyses] = {"outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis","outputQCanalysis"};

// Set common TList name:
TString commonTListName[nAnalyses] = {
"TaskQC <= v2 <= 0.0-5.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 5.0-10.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 10.0-20.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 20.0-30.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 30.0-40.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 40.0-50.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 50.0-60.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 60.0-70.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 70.0-80.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 80.0-100.0_eventCuts_kV0_cutsRP_cutsPOI",
"TaskQC <= v2 <= 0.0-5.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 5.0-10.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 10.0-20.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 20.0-30.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 30.0-40.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 40.0-50.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 50.0-60.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 60.0-70.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 70.0-80.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
"TaskQC <= v2 <= 80.0-100.0_eventCuts_kV0_cutsRP_onlyPositive_cutsPOI",
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
     listIntFlow = dynamic_cast<TList*> commonList->FindObject("Integrated Flow");
     if(!listIntFlow){cout<<"listIntFlow is NULL"<<endl;exit(0);}
     listResults = dynamic_cast<TList*> listIntFlow->FindObject("Results");
     if(!listResults){cout<<"listResults is NULL"<<endl;}

     TH1D *fIntFlow = dynamic_cast<TH1D*> listResults->FindObject("fIntFlow");

     v2[n] = fIntFlow->GetBinContent(1);
     v2Error[n] = fIntFlow->GetBinError(1);
     v4[n] = fIntFlow->GetBinContent(2);
     v4Error[n] = fIntFlow->GetBinError(2);

 
    }
   }
  }
 }
 TH1D *histov2 = new TH1D("histov2","v2 as a function of centrality",100,0,60);
 TH1D *histov4 = new TH1D("histov4","v2 as a function of centrality",100,0,60);
 TH1D *histov2positive = new TH1D("histov2positive","v2 as a function of centrality",100,0,60);
 TH1D *histov4positive = new TH1D("histov4positive","v2 as a function of centrality",100,0,60);

 Double_t Position[10] = {2.5,7.5,15.,25.,35.,45.,55.,65.,75.,90.};
 Int_t Bins[10] = {5,13,26,42,59,76,92,0,0,0};
 Int_t BinsShifted[10] = {7,15,28,44,61,78,94,0,0,0};
 cout << "hej" << endl;

 for (Int_t j = 0; j<10; j++) {
   histov2->Fill(Position[j],v2[j]);
   histov2->SetBinError(Bins[j],v2Error[j]);
   histov4->Fill(Position[j],v4[j]);
   histov4->SetBinError(Bins[j],v4Error[j]);
   histov2positive->SetBinContent(BinsShifted[j],v2[(j+10)]);
   histov2positive->SetBinError(BinsShifted[j],v2Error[(j+10)]);
   histov4positive->SetBinContent(BinsShifted[j],v4[(j+10)]);
   histov4positive->SetBinError(BinsShifted[j],v4Error[(j+10)]);

   cout << j << " " << v2[j] << " " << v4[j] << endl;

 }

 histov2->SetMarkerStyle(20);
 histov2->SetMarkerColor(4);
 histov2->Draw("P");
 histov4->SetMarkerStyle(21);
 histov4->SetMarkerColor(2);
 histov4->Draw("same,P");
 histov2positive->SetMarkerStyle(24);
 histov2positive->SetMarkerColor(4);
 histov2positive->Draw("same,P");
 histov4positive->SetMarkerStyle(25);
 histov4positive->SetMarkerColor(2);
 histov4positive->Draw("same,P");


}
