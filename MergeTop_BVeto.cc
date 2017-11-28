#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>

//declaration
void runMergeTop_BVeto(string lepSelection="SMu_", int charge=0, int systematics=0, int direction=0, int jetPtCutMin = 30, int doBJets=0, int doQCD = 0, int MET=0, int mT=0, string type="Merge");

//instance
void MergeTop_BVeto(){
  ////w+ 
  runMergeTop_BVeto("SMu_",  1, 0, 0, 30, -1, 0, 15, 50, "Merge");  //qcd=0
  runMergeTop_BVeto("SMu_",  1, 0, 0, 30, -1, 1, 15, 50, "Merge");  //qcd=1
  runMergeTop_BVeto("SMu_",  1, 0, 0, 30, -1, 2, 15, 50, "Merge");  //qcd=2
  runMergeTop_BVeto("SMu_",  1, 0, 0, 30, -1, 3, 15, 50, "Merge");  //qcd=3
  ////w-
  runMergeTop_BVeto("SMu_", -1, 0, 0, 30, -1, 0, 15, 50, "Merge");  //qcd=0 
  runMergeTop_BVeto("SMu_", -1, 0, 0, 30, -1, 1, 15, 50, "Merge");  //qcd=1
  runMergeTop_BVeto("SMu_", -1, 0, 0, 30, -1, 2, 15, 50, "Merge");  //qcd=2
  runMergeTop_BVeto("SMu_", -1, 0, 0, 30, -1, 3, 15, 50, "Merge");  //qcd=3
}

//definition
void runMergeTop_BVeto(string lepSelection, int charge, int systematics, int direction, int jetPtCutMin, int doBJets, int doQCD, int MET, int mT, string type){

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  //charge
  ostringstream strCharge;
  if(charge == 1){
    strCharge << "WP_";
  }
  else {
    strCharge << "WM_";
  }

  //systematics
  string strSystematics;
  if      (systematics ==  0)  strSystematics =    "Syst_0_";
  else if (systematics ==  1)  strSystematics =    "Syst_1_"; 
  else if (systematics ==  2)  strSystematics =    "Syst_2_"; 
  else if (systematics ==  3)  strSystematics =    "Syst_3_"; 
  else if (systematics ==  4)  strSystematics =    "Syst_4_"; 
  else if (systematics ==  5)  strSystematics =    "Syst_5_"; 
  else if (systematics ==  6)  strSystematics =    "Syst_6_"; 
  else if (systematics ==  7)  strSystematics =    "Syst_7_"; 
  else if (systematics ==  8)  strSystematics =    "Syst_8_"; 
  else if (systematics ==  9)  strSystematics =    "Syst_9_"; 
  else if (systematics == 10)  strSystematics =   "Syst_10_"; 
  else if (systematics == 11)  strSystematics =   "Syst_11_"; 

  //direction 
  string strDirection;
  if     (direction == 0)    strDirection = "CN_";
  else if(direction == 1)    strDirection = "UP_";
  else                       strDirection = "DN_";

  //jetPtCutMin
  ostringstream strJetPtCutMin; 
  strJetPtCutMin << jetPtCutMin;

  //doBJets
  string strDoBJets;
  if(doBJets == -1)  strDoBJets = "BVeto_";

  //doQCD
  ostringstream strStreamDoQCD;     
  strStreamDoQCD << doQCD ;
  string strDoQCD;
  strDoQCD = strStreamDoQCD.str();

  //MET
  ostringstream strStreamMET;
  strStreamMET << MET;
  string strMET;
  strMET = strStreamMET.str();  

  //mT
  ostringstream strStreammT;
  strStreammT << mT;
  string strmT;
  strmT = strStreammT.str();  

  string directory = "/home/bsutar/t3store2/MuonChargeAsymAnalysis8TeV2012/Results/HistoFiles/Condor/";
  
  //create filenames
  string energy = "8TeV_";
  string whichTopChannel[6] = {"T_s_channel_","T_t_channel_","T_tW_channel_","Tbar_s_channel_","Tbar_t_channel_","Tbar_tW_channel_"};
  string fileNameTopChannel[6];
  string fileNameFinalTop;
  for(int ii=0; ii<6; ii++){
    fileNameTopChannel[ii] = directory + lepSelection + energy + whichTopChannel[ii] + "dR_5311_" + strCharge.str() + "EffiCorr_1_TrigCorr_1_" + strSystematics + strDirection + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_" + strDoBJets + "QCD" + strDoQCD + "_MET" + strMET + "_mT" + strmT + "_" + type + ".root";
    cout << "Merging following files : " << endl;
    cout << ii+1 << " - " << fileNameTopChannel[ii] << endl;
  }
  cout << "into this file : " << endl; 
  fileNameFinalTop         = directory + lepSelection + energy + "Top_" + "dR_5311_" + strCharge.str() + "EffiCorr_1_TrigCorr_1_" + strSystematics + strDirection + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_" + strDoBJets + "QCD" + strDoQCD + "_MET" + strMET + "_mT" + strmT + "_" + type + ".root";
  cout << fileNameFinalTop << endl << endl;
 
  //open TFiles
  TFile *topTFile[6];
  for(int jj=0; jj<6; jj++){
    topTFile[jj] = new TFile(fileNameTopChannel[jj].c_str(),"READ");
  }
  TFile *finalTopTFile = new TFile(fileNameFinalTop.c_str(), "RECREATE");

  int nTopHist = topTFile[0]->GetListOfKeys()->GetEntries();   
  
  for (int i(0); i < nTopHist; i++){
    string hName = topTFile[0]->GetListOfKeys()->At(i)->GetName();
    TH1D     *h1 = (TH1D*) topTFile[0]->Get(hName.c_str()); 
    TH1D     *h2 = (TH1D*) topTFile[1]->Get(hName.c_str()); 
    TH1D     *h3 = (TH1D*) topTFile[2]->Get(hName.c_str()); 
    TH1D     *h4 = (TH1D*) topTFile[3]->Get(hName.c_str()); 
    TH1D     *h5 = (TH1D*) topTFile[4]->Get(hName.c_str()); 
    TH1D     *h6 = (TH1D*) topTFile[5]->Get(hName.c_str()); 

    TH1D *hSum = (TH1D*) h1->Clone();
    hSum->Add(h2);
    hSum->Add(h3);
    hSum->Add(h4);
    hSum->Add(h5);
    hSum->Add(h6);
    finalTopTFile->cd();
    hSum->Write();
  }

  for(int kk=0; kk<6; kk++){
    topTFile[kk]->Close();
  }
  
  finalTopTFile->Close();
 
  /////////////////////////////////////////// 
  // DY //
  string whichDY[2] ={"DYJets10to50_","DYJets_MIX_UNFOLDING_"};
  string fileNameDY[2];
  string fileNameFinalDY;

  for(int ii=0; ii<2; ii++){
    fileNameDY[ii] = directory + lepSelection + energy + whichDY[ii] + "dR_5311_" + strCharge.str() + "EffiCorr_1_TrigCorr_1_" + strSystematics + strDirection + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_" + strDoBJets + "QCD" + strDoQCD + "_MET" + strMET + "_mT" + strmT + "_" + type + ".root";
    cout << "Also merging following files : " << endl;
    cout << ii+1 << " - " << fileNameDY[ii] << endl;
  }
  
  cout << "into this file : " << endl; 
  fileNameFinalDY         = directory + lepSelection + energy + "DYJets10toInf3_" + "dR_5311_" + strCharge.str() + "EffiCorr_1_TrigCorr_1_" + strSystematics + strDirection + "JetPtMin_" + strJetPtCutMin.str() + "_VarWidth_" + strDoBJets + "QCD" + strDoQCD + "_MET" + strMET + "_mT" + strmT + "_" + type + ".root";
  cout << fileNameFinalDY << endl << endl;

  //open TFiles
  TFile *dyTFile[2];
  for(int jj=0; jj<2; jj++){
    dyTFile[jj] = new TFile(fileNameDY[jj].c_str(),"READ");
  }
  TFile *finalDYTFile = new TFile(fileNameFinalDY.c_str(), "RECREATE");

  int nDYHist = dyTFile[0]->GetListOfKeys()->GetEntries();   

  for (int i(0); i < nDYHist; i++){
    string hDYName = dyTFile[0]->GetListOfKeys()->At(i)->GetName();
    TH1D *hDY1 = (TH1D*) dyTFile[0]->Get(hDYName.c_str()); 
    TH1D *hDY2 = (TH1D*) dyTFile[1]->Get(hDYName.c_str()); 

    TH1D *hDYSum = (TH1D*) hDY1->Clone();
    hDYSum->Add(hDY2);
    finalDYTFile->cd();
    hDYSum->Write();
  }

  for(int kk=0; kk<2; kk++){
    dyTFile[kk]->Close();
  }
  
  finalDYTFile->Close();

}
