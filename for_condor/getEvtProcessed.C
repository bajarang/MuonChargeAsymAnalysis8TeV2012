//#include "TROOT.h"    
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include <cmath>
#include <math.h>
#include<string>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <fstream>
#include <map>
#include <iomanip>
#include <iostream>

void getEvtProcessed(){
  
  bool isMC = true;
  bool isMuData = true;
  bool isEleData = false;

  std::map<std::string, double> mcEvtDBS;
  mcEvtDBS["DY1JetsToLL_Merged.root"]       =  62079400;
  mcEvtDBS["DY2JetsToLL_Merged.root"]       =  19970551;
  mcEvtDBS["DY3JetsToLL_Merged.root"]       =  5856110;
  mcEvtDBS["DY4JetsToLL_Merged.root"]       =  4197868;
  mcEvtDBS["DYJetsToLL_Merged.root"]        =  48103700;
  mcEvtDBS["HplusM100_Merged.root"]         =  996170; 
  mcEvtDBS["HplusM120_Merged.root"]         =  994498; 
  mcEvtDBS["HplusM140_Merged.root"]         =  987730; 
  mcEvtDBS["HplusM150_Merged.root"]         =  990645;
  mcEvtDBS["HplusM155_Merged.root"]         =  952984;
  mcEvtDBS["HplusM160_Merged.root"]         =  992264;
  mcEvtDBS["HplusM80_Merged.root"]          =  976710;
  mcEvtDBS["HplusM90_Merged.root"]          =  988480;
  mcEvtDBS["QCD_Pt-15to20_Mu_Merged.root"]  =  4141251;
  mcEvtDBS["QCD_Pt-20to30_Mu_Merged.root"]  =  31475157;
  mcEvtDBS["QCD_Pt-30to50_Mu_Merged.root"]  =  29954815;
  mcEvtDBS["QCD_Pt-50to80_Mu_Merged.root"]  =  19806915;
  mcEvtDBS["QCD_Pt-80to120_Mu_Merged.root"] =  13786971;
  mcEvtDBS["QCD_Pt-120to170_Mu_Merged.root"]=  8042721;
  mcEvtDBS["QCD_Pt-170to300_Mu_Merged.root"]=  7947159;
  mcEvtDBS["QCD_Pt-300to470_Mu_Merged.root"]=  7937590;
  mcEvtDBS["ST_s_Merged.root"]              =  2989199;
  mcEvtDBS["ST_t__Merged.root"]              =  38811017;
  mcEvtDBS["ST_tW_Merged.root"]             =  6933094;
  mcEvtDBS["TTJetsM_Merged.root"]           =  10139950;   
  mcEvtDBS["TTJetsP_Merged.root"]           =  77081156;   
  mcEvtDBS["W1JetsToLNu_Merged.root"]       =  44813600;
  mcEvtDBS["W2JetsToLNu_Merged.root"]       =  29878415;
  mcEvtDBS["W3JetsToLNu_Merged.root"]       =  19798117;
  mcEvtDBS["W4JetsToLNu_Merged.root"]       =  9170576;
  mcEvtDBS["WJetsToLNu_Merged.root"]        =  29181900;
  mcEvtDBS["WW_Merged.root"]                =  994012;
  mcEvtDBS["WZ_Merged.root"]                =  1000000;
  mcEvtDBS["ZZ_Merged.root"]                =  990064; 
  std::map<std::string, double>::iterator itr_mc;
  if(isMC){ 
    cout<<"=============================="<<endl;
    cout<<"        ALL MC SAMPLES        "<<endl;
    cout<<"=============================="<<endl;
    for(itr_mc = mcEvtDBS.begin(); itr_mc != mcEvtDBS.end(); ++itr_mc){
      TString inFile(itr_mc->first);
      TFile* ttbar= new TFile(inFile);
      TString path= "base/totalEvents";
      TH1F* hist= (TH1F*)(ttbar->Get(path));
      int entries= hist->GetBinContent(1);
      double mean= hist->GetMean();
      double event_cond = entries*mean;//events from condor submission
      double event_dbs = itr_mc->second;//events at data base server
      double ratio = event_dbs/event_cond;
      cout<<setw(30)<<inFile<<setw(15)<<event_dbs<<setw(15)<<event_cond<<setw(15)<<ratio<<endl;
    }
  }
        
  std::map<std::string, double> muDataEvtDBS;
  muDataEvtDBS["MuRunB2v2_Merged.root"] =147530808 +5586576 ;  
  muDataEvtDBS["MuRunCv1_Merged.root"]  =63880331 +762991 ;
  muDataEvtDBS["MuRunDv1_Merged.root"]  =95873038 +734074 ;
  muDataEvtDBS["MuRunEv1_Merged.root"]  =86439384 +1723910 ;
  muDataEvtDBS["MuRunFv1_Merged.root"]  =65047318   ;
  muDataEvtDBS["MuRunGv1_Merged.root"]  =146183276 +2545265 ;
  muDataEvtDBS["MuRunH2v1_Merged.root"] =161032083 +5367903 ; 
  muDataEvtDBS["MuRunH3v1_Merged.root"] =4389914  ;
  
  std::map<std::string, double>::iterator itr_mudata;
  if(isMuData){ 
    cout<<"=============================="<<endl;
    cout<<"     ALL Muon Data SAMPLES    "<<endl;
    cout<<"=============================="<<endl;
    for(itr_mudata = muDataEvtDBS.begin(); itr_mudata != muDataEvtDBS.end(); ++itr_mudata){
      TString inFile(itr_mudata->first);
      TFile* ttbar= new TFile(inFile);
      TString path= "base/totalEvents";
      TH1F* hist= (TH1F*)(ttbar->Get(path));
      int entries= hist->GetBinContent(1);
      double mean= hist->GetMean();
      double event_cond = entries*mean;//events from condor submission
      double event_dbs = itr_mudata->second;//events at data base server
      double ratio = event_dbs/event_cond;
      cout<<setw(30)<<inFile<<setw(15)<<event_dbs<<setw(15)<<event_cond<<setw(15)<<ratio<<endl;
    }
  }

 /* 
  TString ele_data_file[9] = {
  " EleRunBver2v2_Merged.root", 
  " EleRunCv1_Merged.root    ", 
  " EleRunDv1_Merged.root    ", 
  " EleRunEv1_Merged.root    ", 
  " EleRunFv1_Merged.root    ", 
  " EleRunGv1_Merged.root    ", 
  " EleRunHver2v1_Merged.root", 
  " EleRunHver3v1_Merged.root",
  " all_EleData.root"
  };
 
  if(isEleData){ 
    cout<<endl;
    cout<<"=============================="<<endl;
    cout<<" ALL ELECTRON DATA SAMPLES    "<<endl;
    cout<<"=============================="<<endl;
    for(int i= 0; i<9; i++){
      TString inFile(ele_data_file[i]);
      TFile* ttbar= new TFile(inFile);
      TString path= "base/totalEvents";
      TH1F* hist= (TH1F*)(ttbar->Get(path));
      int entries= hist->GetBinContent(1);
      double mean= hist->GetMean();
      double events = entries*mean;
      cout<<events<<" : "<<"\t"<<inFile<<endl;
      //cout<<" Total Events= "<<events<<endl;
      //hist->Draw();
    }
  }
  */
}

