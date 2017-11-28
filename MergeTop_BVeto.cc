#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>

//declaration
void runMergeTop_BVeto(string lepSelection="SMu", int charge=0, int systematics=0, int direction=0, int jetPtCutMin = 30, int doBJets=0, int doQCD = 0, double MET=0, double mT=0, string type="Merge");

//instance
void MergeTop_BVeto(){
  //w+ 
  runMergeTop_BVeto("SMu",  1, 0, 0, 30, -1, 0, 15, 50, "Merge");  //qcd=0
  runMergeTop_BVeto("SMu",  1, 0, 0, 30, -1, 1, 15, 50, "Merge");  //qcd=1
  runMergeTop_BVeto("SMu",  1, 0, 0, 30, -1, 2, 15, 50, "Merge");  //qcd=2
  runMergeTop_BVeto("SMu",  1, 0, 0, 30, -1, 3, 15, 50, "Merge");  //qcd=3
  //w-
  runMergeTop_BVeto("SMu", -1, 0, 0, 30, -1, 0, 15, 50, "Merge");  //qcd=0 
  runMergeTop_BVeto("SMu", -1, 0, 0, 30, -1, 1, 15, 50, "Merge");  //qcd=1
  runMergeTop_BVeto("SMu", -1, 0, 0, 30, -1, 2, 15, 50, "Merge");  //qcd=2
  runMergeTop_BVeto("SMu", -1, 0, 0, 30, -1, 3, 15, 50, "Merge");  //qcd=3
}

//definition
void runMergeTop_BVeto(string lepSelection, int charge, int systematics, int direction, int jetPtCutMin, int doBJets, int doQCD, double MET, double mT, string type){

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  //charge
  ostringstream strCharge;
  if(charge == 1){
    strCharge << "WP";
  }
  else {
    strCharge << "WM";
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
  string strDirection
  if (direction == -1)    strDirection = "CN";
  else if(direction == 1) strDirection = "UP";
  else                    strDirection = "DN";

  //jetPtCutMin
  ostringstream strJetPtCutMin; 
  strJetPtCutMin << jetPtCutMin;

  //doBJets
  string strDoBJets = "";
  if(doBJets == -1)  strDoBJets = "_BVeto";

  //doQCD
  ostringstream strStreamDoQCD;     
  strStreamDoQCD << doQCD ;
  string strDoQCD;
  strDoQCD = strStreamDoQCD.str();

  string directory = "/home/bsutar/t3store2/MuonChargeAsymAnalysis8TeV2012/Results/HistoFiles/Condor/";
  
  // 
  cout << "T\t" << syst << "\tQCD:" << doQCD << "\tBJets:" << doBJets << endl;

  if(doQCD==0){
    string str1 = directory + lepSelection +  "_8TeV_T_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    string str2 = directory + lepSelection +  "_8TeV_T_t_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    string str3 = directory + lepSelection +  "_8TeV_T_tW_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    string str4 = directory + lepSelection +  "_8TeV_Tbar_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    string str5 = directory + lepSelection +  "_8TeV_Tbar_t_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    string str6 = directory + lepSelection +  "_8TeV_Tbar_tW_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    string strf = directory + lepSelection +  "_8TeV_Top_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
  }
  else {
    string str1 = directory + lepSelection +  "_8TeV_T_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    string str2 = directory + lepSelection +  "_8TeV_T_t_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    string str3 = directory + lepSelection +  "_8TeV_T_tW_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    string str4 = directory + lepSelection +  "_8TeV_Tbar_s_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    string str5 = directory + lepSelection +  "_8TeV_Tbar_t_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    string str6 = directory + lepSelection +  "_8TeV_Tbar_tW_channel_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    string strf = directory + lepSelection +  "_8TeV_Top_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
  }
  
  TFile *f1 = new TFile(str1.c_str());
  TFile *f2 = new TFile(str2.c_str());
  TFile *f3 = new TFile(str3.c_str());
  TFile *f4 = new TFile(str4.c_str());
  TFile *f5 = new TFile(str5.c_str());
  TFile *f6 = new TFile(str6.c_str());
  TFile *ff = new TFile(strf.c_str(), "RECREATE");

  int nHist = f1->GetListOfKeys()->GetEntries();   
  cout << nHist << endl;
  
  for (int i(0); i < nHist; i++){
    string hName = f1->GetListOfKeys()->At(i)->GetName();
    TH1D *h1 = (TH1D*) f1->Get(hName.c_str()); 
    TH1D *h2 = (TH1D*) f2->Get(hName.c_str()); 
    TH1D *h3 = (TH1D*) f3->Get(hName.c_str()); 
    TH1D *h4 = (TH1D*) f4->Get(hName.c_str()); 
    TH1D *h5 = (TH1D*) f5->Get(hName.c_str()); 
    TH1D *h6 = (TH1D*) f6->Get(hName.c_str()); 

    TH1D *hSum = (TH1D*) h1->Clone();
    hSum->Add(h2);
    hSum->Add(h3);
    hSum->Add(h4);
    hSum->Add(h5);
    hSum->Add(h6);
    ff->cd();
    hSum->Write();
  }

  f1->Close();
  f2->Close();
  f3->Close();
  f4->Close();
  f5->Close();
  f6->Close();
  ff->Close();
  
  // DY
  cout << "DY\t" << syst << "\t" << doQCD << endl;
  if(doQCD==0){
    sstrDY1 = directory+ lepSelection +  "_8TeV_DYJets10to50_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    sstrDY2 = directory+ lepSelection +  "_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
    sstrDYf = directory+ lepSelection +  "_8TeV_DYJets10toInf3_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_MET15.root";
  }
  else {
    sstrDY1 = directory+ lepSelection +  "_8TeV_DYJets10to50_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    sstrDY2 = directory+ lepSelection +  "_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
    sstrDYf = directory+ lepSelection +  "_8TeV_DYJets10toInf3_dR_5311_EffiCorr_1_TrigCorr_1_" + syst + "JetPtMin_" + strJetPtCutMin.str() + str_rochcorr + "_VarWidth" + str_dobjets + "_QCD" + doQCDStr.str() + "_MET15.root";
  }

  TFile *fDY1 = new TFile(sstrDY1.c_str());
  TFile *fDY2 = new TFile(sstrDY2.c_str());
  TFile *fDYf = new TFile(sstrDYf.c_str(), "RECREATE");

  int nDYHist = fDY1->GetListOfKeys()->GetEntries();   
  cout << nDYHist << endl;
  for (int i(0); i < nDYHist; i++){
    string hDYName = fDY1->GetListOfKeys()->At(i)->GetName();
    TH1D *hDY1 = (TH1D*) fDY1->Get(hDYName.c_str()); 
    TH1D *hDY2 = (TH1D*) fDY2->Get(hDYName.c_str()); 

    TH1D *hDYSum = (TH1D*) hDY1->Clone();
    hDYSum->Add(hDY2);
    fDYf->cd();
    hDYSum->Write();
  }
  fDY1->Close();
  fDY2->Close();
  fDYf->Close();
}
