/*
 * We pass either of three/four arguments to this script
 * based on those arguments, this executable will be run
 * the corresponding output histogram file will be created
 * with the name of our choice..
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <TSystem.h>
#include "TROOT.h"
#include "TError.h"
#include "LHAPDF/LHAPDF.h"
using namespace std;


int main(int argc, char* argv[]) {
  ///////////////////Load shared libraries/////////////////////////
  string srcdir = "/home/bsutar/t3store/Asym2016-8TeV/CMSSW_5_3_11/src/TreeAnalysis2012/WJetsp_CENTRAL_QCD0/Sources/";
  vector<string> sources;
  sources.push_back("getFilesAndHistograms");
  sources.push_back("functions");
  sources.push_back("funcReweightResp");
  sources.push_back("HistoSet");
  sources.push_back("rochcor2012wasym_hasan");   
  sources.push_back("ZJetsAndDPS");

  unsigned int nSources = sources.size();
  gSystem->AddIncludePath("-D__USE_XOPEN2K8");
  gROOT->ProcessLine(".L /cvmfs/cms.cern.ch/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
  for (unsigned int i(0); i < nSources; i++) {
    std::cout << "Compiling " << srcdir + sources[i] << ".cc" << std::endl;
    gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
  }
  /*
  string currentWorkingDir = "/home/bsutar/t3store/Asym2016-8TeV/CMSSW_5_3_11/src/TreeAnalysis2012/WJetsp_CENTRAL_QCD0/";
  gErrorIgnoreLevel = kError;
  string srcdir       = currentWorkingDir + "Sources/";
  string incdir       = currentWorkingDir + "Includes/";
  string roounfolddir = currentWorkingDir + "RooUnfold-1.1.1/";
  string lhapdfdir    = "/cvmfs/cms.cern.ch/slc5_amd64_gcc434/external/lhapdf/5.8.5/include/";

  cout << "--------------------------------------------------------------------------------\n" << endl;
  cout << "Adding " << incdir << " to includes directories..." << endl;
  gSystem->AddIncludePath(string("-I" + incdir).c_str());
  cout << "Adding " << lhapdfdir << " to includes directories..." << endl;
  gSystem->AddIncludePath(string("-I" + lhapdfdir).c_str());
  cout << "Adding " << roounfolddir << "src" << " to includes directories..." << endl;
  gSystem->AddIncludePath(string("-I" + roounfolddir + "src").c_str());
  cout << "Include Path -D__USE_XOPEN2K8 to fix lxplus6 compatibility" << endl;
  gSystem->AddIncludePath("-D__USE_XOPEN2K8");
  cout << "Loading RooUnfold libraries..." << endl;
  gSystem->Load(string(roounfolddir + "libRooUnfold").c_str());
  cout << "\n";
  cout << "--------------------------------------------------------------------------------\n" << endl;
  */  

  ///////////////////Check consistency in arguments/////////////////////////
  cout << "Entered number of arguements : " << argc-1 << endl;
  if(argc!=4 && argc!=5 && argc!=6){
    cout << "Please provide either 2 or 3 or 4 options like below" << endl;
    cout << "Usage : \t 1]  ./exec_runDYJets LineNumber doQCD                       #For Full Systematics" << endl;
    cout << "      : \t 2]  ./exec_runDYJets LineNumber doQCD systematics           #For a particular systematics in either direction" << endl;
    cout << "      : \t 3]  ./exec_runDYJets LineNumber doQCD systematics direction #For a particular systematics in a particular direction" << endl;
    cout << "Eg. \t 1]  ./exec_runDYJets 2 3      #Obtain full systematics for LineNumber 2 and doQCD 3" << endl;
    cout << "    \t 2]  ./exec_runDYJets 2 3 4    #Obtain full systematics for LineNumber 2 and doQCD 3 and systematics 4" << endl;
    cout << "    \t 2]  ./exec_runDYJets 2 3 4 -1 #Obtain full systematics for LineNumber 2 and doQCD 3 and systematics 4 and direction -1" << endl;
    cout << "Exiting Now." << endl;
    exit(0);
  }

  ///////////////////Assign the arguments to internal parameters/////////////////////////
  stringstream strLineNum;
  stringstream strDOQCD;
  stringstream strSystematics;
  stringstream strDirection;

  int intLineNum     =   0;
  int intDOQCD       =  -1;
  int intSystematics =  -1;
  int intDirection   = 100;

  if(argc==4){
    strLineNum << argv[1];
    strLineNum >> intLineNum;
    strDOQCD << argv[2];
    strDOQCD >> intDOQCD;
    strSystematics << argv[3];
    strSystematics >> intSystematics;
    cout << __LINE__ << "  Executing for LineNumber : " << intLineNum << "  doQCD : " << intDOQCD << "  systematics : " << intSystematics << endl; 
  }

  if(argc==5){
    strLineNum << argv[1];
    strLineNum >> intLineNum;
    strDOQCD << argv[2];
    strDOQCD >> intDOQCD;
    strSystematics << argv[3];
    strSystematics >> intSystematics;
    strDirection << argv[4];
    strDirection >> intDirection;
    cout << __LINE__ << "  Executing for LineNumber : " << intLineNum << "  doQCD : " << intDOQCD << "  systematics : " << intSystematics << "  direction : " << intDirection << endl; 
  }

  ///////////////////Check if parameters are within respective ranges/////////////////////////
  if(intLineNum > 34){
    cout << "You entered Line Number = " << intLineNum << endl;
    cout << "It should be lesser than 34 " << endl;
    cout << "Exiting Now." << endl;
    exit(0);
  }

  if(intDOQCD>3) {
    cout << "You entered doQCD = " << argv[2] << endl;
    cout << "It should be lesser than or equal to 3 " << endl << endl;
    exit(0);
  }
  
  int possibleSystematics[11] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 100};
  bool matchesSysteamtics = false;

  for(int i=0; i<11; i++){
    if(intSystematics == possibleSystematics[i]) {
      cout << i << "  " << intSystematics << "  " << possibleSystematics[i] << "   matched" << endl;
      matchesSysteamtics = true;
    }
    else continue;
  }
  if(!matchesSysteamtics) {
    cout << "You entered systematics = " << argv[3] << endl;
    cout << "Please choose one among these values : 0  1  2  3  4  5  6  7  8  11 100" << endl;
    cout << "Exiting Now." << endl << endl;
    exit(0);
  }

  if(intDirection != -1 && intDirection!=0 && intDirection!=1) {
    if(intSystematics!=100){
      cout << "Inside " << endl;
      cout << "You entered direction = " << intDirection << endl;
      cout << "It can be either 1 or 0 or -1 " << endl;
      cout << "Exiting Now." << endl << endl;
      exit(0);
    }
  }

  ///////////////////READ Input_DB_Summary.txt file/////////////////////////
  cout << "READING Input_DB_Summary.txt file " << endl;
  ifstream inputFile("Input_DB_Summary.txt");
  string textline;
  const char *linest;
  
  vector<string> filePath;

  stringstream strStreamFileName;

  string strDOWHAT, strCI, strEvents, strPartition, strCurretPar, strFileName;

  for(int i=1; i<=34; i++){
    getline(inputFile,textline);
    linest=textline.c_str();
    if(i==intLineNum){
      inputFile >> strDOWHAT >> strCI >> strEvents >> strPartition >> strCurretPar >> strFileName;
      strStreamFileName << strFileName;
      while(getline(strStreamFileName,strFileName,'/')){
        filePath.push_back(strFileName);
      }
    }
  }

  for(vector<string>::iterator it=filePath.begin(); it!=filePath.end(); ++it){
    cout << *it << endl;
  } 
  

  int counterFileName = 0;
  int counterDOWHAT = 0;

  stringstream strStreamDOWHAT;
  strStreamDOWHAT << strDOWHAT;
  int intDOWHAT = -10;
  strStreamDOWHAT >> intDOWHAT;
   
  counterFileName = filePath.size()-1;
  string fileName = "";
  fileName = filePath.at(counterFileName);
  
  cout << "--> Reading Input Ntuple file : " << fileName << endl << endl;

  size_t found;
  found = fileName.find(".root");

  string trimmedFileName;
  trimmedFileName = fileName.substr(0,found);

  stringstream appendString;
  
  //////////////////////////////////////////////////////////////////

  if(trimmedFileName.find("_Data_")!=string::npos) {
    if(intSystematics != 0 && intSystematics != 2) {
      cout << "We do not carry out systematics = " << intSystematics  << " for Data" << endl;
      cout << "Exiting Now." << endl;
      exit(0);
    }
    else {
      appendString << "_EffiCorr_0";
      appendString << "_TrigCorr_0";
    }
  }
  
  //////////////////////////////////////////////////////////////////
  
  int doWhat       = intDOWHAT;
                            // 100 - all ; 0 - data, 1 - background , 2 - tau ?, 3 - DY, 4 - W+jets,
                            // 51 - MC gen, 90 - PDF Syst., 1001 - do pull DY samples
      
  int doQCD        = intDOQCD;

                           // 0-3 : 4 combination between isolation/anti-isolation and MT cuts
      
  int doSysRunning = intSystematics;
                           // 0 - no syst running, 100 - all systematic runnings,
                           // 1 - PU, 2 - JES, 3 - XSEC, 4 - JER, 5 - LepSF,
                           // 6 - BtagSF, 7 - MES, 8 - MER, 9 - WB, 10 - RESP, 11 - LumiUncer
  
  int doDirection  = intDirection;
                           // 1  - Up direction
                           // 0  - No direction
                           // -1 - Down direction

  int doBJets      = -1;
                          // 0 - no infor on B-jets will be used ;
                          // 1, 2 .. require at least 1, 2, .. ; use 2 for ttbar systmatics;
                          // -1, -2, .. veto the event if you have 1 or more, 2 or more .. b-jets ;
                          // 101 - require exactly 1 b-jet
  cout << intDOWHAT << "  " << intDOQCD << "  " << intSystematics << "  " << intDirection << endl;
  
  
  //--- Internal configuration (no need to change this)---
      
  string lepSelection = "SMu"; // default lumi is set for double muon dataset
  double muLumi(19.549); // DoubleMu number with pixelCalc
  double eleLumi(19.602); // DoubleEle number with pixelCalc
  if      (lepSelection == "DE")   muLumi = 19.602;
  else if (lepSelection == "SMuE") muLumi = 19.673;
  else if (lepSelection == "SMu")  muLumi = 19.244;
  else if (lepSelection == "SE")   muLumi = 19.174;

  bool doRoch       =    0;
  bool doFlat       =    0;
  int  doPUStudy    =  -10;  // default int the ZJets -10
  bool doSSign      =    0;     // contribution of QCD to emu in TTbar 0
  bool doInvMassCut =    0;
  bool doDataEff    =    0;  
      
  int doMETcut      =    0;     // if you want to apply MET cut  0
  int jetPtMin      =   30;           //30
  int jetPtMax      =    0;      // 0 - for no jetPtMax cut
  int ZPtMin        =    0;   //0
  int ZEtaMin       = -999999; //default value -999999  !!!!!!!  factor 100 to keep things integer .... eta 2.4  = eta Cut 240
  int ZEtaMax       =  999999; //default value  999999

  const int NSystData(3), NSystMC(12); // set NSystMC to 5 if you want to do only PU, XSEC
  //const int NSystWJets(16), NSystDYJets(14);
  const int NSystWJets(18), NSystDYJets(14);
  
  short dataSyst[NSystData]  = {0, 2, 2};
  short dataDir[NSystData]   = {0,-1, 1};

  short ttSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short ttDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float ttScale[NSystMC]     = {1, 1, 1, 0.07, 0.07,  1, 1, 1, 1, 1, 1, 1};

  short tauSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short tauDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float tauScale[NSystMC]    = {1, 1, 1, 0.05, 0.05,  1, 1, 1, 1, 1, 1, 1};

  short bgSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short bgDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float bgScale[NSystMC]     = {1, 1, 1, 0.15, 0.15,  1, 1, 1, 1, 1, 1, 1};
  
  short zzSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short zzDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float zzScale[NSystMC]     = {1, 1, 1, 0.06, 0.06,  1, 1, 1, 1, 1, 1, 1};
      
  short wzSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short wzDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float wzScale[NSystMC]     = {1, 1, 1, 0.06, 0.06,  1, 1, 1, 1, 1, 1, 1};
  
  short wwSyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short wwDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float wwScale[NSystMC]     = {1, 1, 1, 0.08, 0.08,  1, 1, 1, 1, 1, 1, 1};
      
  short tcsSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short tcsDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float tcsScale[NSystMC]    = {1, 1, 1, 0.06, 0.06,  1, 1, 1, 1, 1, 1, 1};
  
  short tctSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short tctDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float tctScale[NSystMC]    = {1, 1, 1, 0.06, 0.06,  1, 1, 1, 1, 1, 1, 1};
  
  short tcwSyst[NSystMC]     = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short tcwDir[NSystMC]      = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float tcwScale[NSystMC]    = {1, 1, 1, 0.09, 0.09,  1, 1, 1, 1, 1, 1, 1};
      
  short wjSyst[NSystWJets]   = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 4, 4, 8, 9, 10, 11, 11};
  short wjDir[NSystWJets]    = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1,-1, 1, 1, 1,  1,  1, -1};
  float wjScale[NSystWJets]  = {1, 1, 1, 0.04, 0.04,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1,  1,  1};
      
  short dySyst[NSystDYJets]  = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 4, 4, 8};
  short dyDir[NSystDYJets]   = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1,-1, 1, 1};
  float dyScale[NSystDYJets] = {1, 1, 1, 0.04, 0.04,  1, 1, 1, 1, 1, 1, 1, 1, 1};
   
  // Data
  if ( doWhat == 0 || doWhat == 100) {
    for (unsigned int i(0); i < NSystData; i++) {
      if (dataSyst[i] != doSysRunning && doSysRunning != 100) continue;
      if (dataDir[i]  != intDirection && intDirection != 100) continue; 

      ZJetsAndDPS DMudata(lepSelection+"_8TeV_Data_dR_5311", 1., 1, 1, doDataEff, dataSyst[i], dataDir[i], 1, jetPtMin, jetPtMax, ZPtMin, ZEtaMin, ZEtaMax);
      ////DMudata.Loop(1, 0, doQCD, doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
    }
  }
    
    /*
    // Background
    if ( doWhat == 1 || doWhat == 100 ){
        for (unsigned int i(0); i < NSystMC; i++){
            if (bgSyst[i] != doSysRunning && doSysRunning != 100) continue;

            ZJetsAndDPS DMuTT(lepSelection+"_8TeV_TTJets_dR_5311",             muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTT.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);

            ZJetsAndDPS DMuZZInc(lepSelection+"_8TeV_ZZ_dR_5311",              muLumi*17.654        *1000/9799908.,  1, 1, !doDataEff, zzSyst[i], zzDir[i], zzScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuZZInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuWZInc(lepSelection+"_8TeV_WZ_dR_5311",              muLumi*33.21         *1000/10000283., 1, 1, !doDataEff, wzSyst[i], wzDir[i], wzScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWZInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuWWInc(lepSelection+"_8TeV_WW_dR_5311",              muLumi*54.838        *1000/10000431., 1, 1, !doDataEff, wwSyst[i], wwDir[i], wwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuWWInc.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuT(lepSelection+"_8TeV_T_s_channel_dR_5311",         muLumi*3.79           *1000/259961.,  1, 1, !doDataEff, tcsSyst[i], tcsDir[i], tcsScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuT1(lepSelection+"_8TeV_T_t_channel_dR_5311",        muLumi*56.4           *1000/3758227., 1, 1, !doDataEff, tctSyst[i], tctDir[i], tctScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuT2(lepSelection+"_8TeV_T_tW_channel_dR_5311",       muLumi*11.1           *1000/497658.,  1, 1, !doDataEff, tcwSyst[i], tcwDir[i], tcwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuT2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuTbar(lepSelection+"_8TeV_Tbar_s_channel_dR_5311",   muLumi*1.76           *1000/139974.,  1, 1, !doDataEff, tcsSyst[i], tcsDir[i], tcsScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTbar.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuTbar1(lepSelection+"_8TeV_Tbar_t_channel_dR_5311",  muLumi*30.7           *1000/1903681., 1, 1, !doDataEff, tctSyst[i], tctDir[i], tctScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTbar1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuTbar2(lepSelection+"_8TeV_Tbar_tW_channel_dR_5311", muLumi*11.1           *1000/493460.,  1, 1, !doDataEff, tcwSyst[i], tcwDir[i], tcwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuTbar2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            ZJetsAndDPS DMuDY10(lepSelection+"_8TeV_DYJets10to50_dR_5311",     muLumi*860.5          *1000/11707222., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDY10.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
            
            if ( (lepSelection.find("SE") == -1 && lepSelection.find("SMu") == -1 )  ) {
                // for Z plus jets: WJets is background
                ZJetsAndDPS DMuWJ(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311",           muLumi*36703.   *1000/76102995.,1, 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWJ.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                
                ZJetsAndDPS DMuWJmix(lepSelection+"_8TeV_WJetsALL_MIX_UNFOLDING_dR_5311",    muLumi*36703.   *1000/76102995.,1, 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWJmix.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            }
            if ( (lepSelection.find("SE") == -1 && lepSelection.find("SMu") == -1 )  ) {
                //--- the following Bg are not used for WJets analysis ---//
                ZJetsAndDPS DMuTTrew(lepSelection+"_8TeV_TTJets_dR_5311_TopReweighting",             muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuTTrew.Loop(1, 1, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                ZJetsAndDPS DMuZZ(lepSelection+"_8TeV_ZZJets2L2Nu_dR_5311",        muLumi*17.654*0.04039 *1000/954911.,  1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuZZ.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                ZJetsAndDPS DMuWW(lepSelection+"_8TeV_WWJets2L2Nu_dR_5311",        muLumi*54.838*0.10608 *1000/1933235., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWW.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                ZJetsAndDPS DMuZZ1(lepSelection+"_8TeV_ZZJets2L2Q_dR_5311",        muLumi*17.654*0.14118 *1000/1936727., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuZZ1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                ZJetsAndDPS DMuZZ2(lepSelection+"_8TeV_ZZJets4L_dR_5311",          muLumi*17.654*0.010196*1000/4807893., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuZZ2.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                ZJetsAndDPS DMuWZ(lepSelection+"_8TeV_WZJets3LNu_dR_5311",         muLumi*33.21 *0.032887*1000/1995334., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWZ.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
                ZJetsAndDPS DMuWZ1(lepSelection+"_8TeV_WZJets2L2Q_dR_5311",        muLumi*33.21 *0.068258*1000/3215990., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
                DMuWZ1.Loop(1, 0, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            }
        }
    }
    */

    /*    
    // DYJets_MIX_UNFOLDING_dR_5311_Inf3
    if ( doWhat == 3 || doWhat == 100 ){
        int doGen = 0 ;
        if ( lepSelection.find("DMu") == 0 || lepSelection.find("DE") == 0 )  doGen = 1 ;
        
        for (unsigned int i(0); i < NSystDYJets; i++){
            if ( ( lepSelection.find("SMu") == 0 || lepSelection.find("SE") == 0 ) && dySyst[i] == 4 ) continue; // jet smearing part -- not done for SMu ---
            if ( ( lepSelection.find("DMu") == 0 || lepSelection.find("DE") == 0 ) && dySyst[i] == 3 ) continue; // xsec -- not done for Z+jets ---
            if (dySyst[i] != doSysRunning && doSysRunning != 100) continue;
            
            ZJetsAndDPS DMuDYMix(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, dySyst[i], dyDir[i], dyScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuDYMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch );
        }
        
        if ( (lepSelection.find("SE") == -1 && lepSelection.find("SMu") == -1 )  ) {
            // these files are not needed for W+jets
            ZJetsAndDPS DMuDYTauS(lepSelection+"_8TeV_DYJets_FromTau_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1, 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDYTauS.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuDY(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDY.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            
            // scale files
            ZJetsAndDPS DMuDYscaleUp(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleUp",  muLumi*3531.8*1000/2170270., 1.,  1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDYscaleUp.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
            ZJetsAndDPS DMuDYscaleDown(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleUp",  muLumi*3531.8*1000/1934901.,    1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            DMuDYscaleDown.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
    */
       
    /* 
    // This is unfolding part for W+jets 
    if ( doWhat == 4 || doWhat == 100 ){
        int doGen = 0 ;
        if ( (lepSelection.find("SE") == 0 || lepSelection.find("SMu") == 0 ) && lepSelection.find("SMuE") == -1 )  doGen = 1 ;

        for (unsigned int i(0); i < NSystWJets; i++){
            if (!doGen ) continue;
            if ( ( lepSelection.find("SMu") == 0 || lepSelection.find("SE") == 0 ) && wjSyst[i] == 3) continue; // xsec -- not done for SMu ---
            if (wjSyst[i] != doSysRunning && doSysRunning != 100) continue;
            
            ZJetsAndDPS DMuWJMix(lepSelection+"_8TeV_WJetsALL_MIX_UNFOLDING_dR_5311", muLumi*36703. *1000/76102995., 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
            DMuWJMix.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch );
            
            //ZJetsAndDPS DMuWJ(lepSelection+"_8TeV_WJetsALL_UNFOLDING_dR_5311",  muLumi*36703.       *1000/76102995., 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            //DMuWJ.Loop(1, doGen,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
    */
   
    /*
    // Sherpa2
    if ( doWhat == 51){
        
        // this is setup for sherpa NLO
        ZJetsAndDPS DESherpaTest2NLO("SMu_8TeV_WToLNu_Sherpa2jNLO4jLO_v6_List",  muLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        DESherpaTest2NLO.Loop(0, 1, 0, 0, 0);
        
        //ZJetsAndDPS DESherpaTest2NLO("SE_8TeV_HEJ_v1",  muLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //DESherpaTest2NLO.Loop(0, 1, 0, 0, 0);
        
        // this is setup for sherpa NLO
        //ZJetsAndDPS DESherpaTest2NLO("DE_8TeV_DY_Sherpa_2NLO4_HepMC_dR_Full_List",  eleLumi         * 1000.          , 1.,    0,   0,     0,    0,     1.,  jetPtMin,  jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax );
        //DESherpaTest2NLO.Loop(0, 1, 0, 0, 0);
        
    }
    */   
     
    /*
    // skip this part
    if ( doWhat == 2 || doWhat == 100 ){
        for (unsigned int i(3); i < 5; i++){
            ZJetsAndDPS DMuDYTau(lepSelection+"_8TeV_DYJets_FromTau_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/3045950, 1., 1,  !doDataEff, tauSyst[i], tauDir[i], tauScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
            //	  DMuDYTau.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        }
    }
    if (doWhat == 222){ // for individual production
        ZJetsAndDPS DMuDY("DMu_8TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1,          jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        //       DMuDY.Loop(1, 1,  doQCD,  doSSign, doInvMassCut,  doBJets, doPUStudy );
        //         DMuDY.Loop(1, 0,  1,  1, 1, 22, doBJets, 9 );
        ZJetsAndDPS DEDY("DE_8TeV_DYJets_UNFOLDING_dR_5311_Inf3",  muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1,          jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        ///          DEDY.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        ZJetsAndDPS DMuTTrew(lepSelection+"_8TeV_TTJets_dR_5311_TopReweighting",             muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        DMuTTrew.Loop(1, 1, doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        
    }
    if (doWhat == -3) { // this is a testing flag
        ZJetsAndDPS DMuDYMixPDF(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, -30, 1);
        //DMuDYMixPDF.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 1, 0, "CT10nlo.LHgrid", 0);
        //DMuDYMixPDF.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 0, 0);
        
        //ZJetsAndDPS DMuDYscaleUp(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleUp",  muLumi*3531.8*1000/(2*2170270.), 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        //DMuDYscaleUp.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        //ZJetsAndDPS DMuDYscaleDown(lepSelection+"_8TeV_DYJets_UNFOLDING_dR_5311_Inf3_scaleDown",  muLumi*3531.8*1000/(2*1934901.), 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        //DMuDYscaleDown.Loop(1, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy );
        
    }
    if (doWhat == 90) {
        for (int pdfMember(1); pdfMember <= 5; pdfMember++) {
            ZJetsAndDPS DMuDYMixPDF(lepSelection+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1, jetPtMin, jetPtMax, ZEtaMin,    ZEtaMax, -30);
            DMuDYMixPDF.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 1, 0, "NNPDF20_as_0118_100.LHgrid", pdfMember);
            //DMuDYMixPDF.Loop(0, 1,  doQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, 0, 0, 1, 0);
        }
    }
    // now produce files for pulls
    if ( doWhat == 1001 ){
        int doGen = 1 ;
        doDataEff = 1 ;
        int NPulls = 25 ;
        for (int loopPull = 0 ; loopPull < NPulls ; loopPull++){
            if ( lepSelection.find("DMu") == 0 ||  lepSelection.find("DE") == 0 ) {
                ZJetsAndDPS DMuDYMix(lepSel+"_8TeV_DYJets_MIX_UNFOLDING_dR_5311_Inf3", muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, 0, 0, 1,   jetPtMin,         jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
                DMuDYMix.Loop(1, 1,  0,  0, 0, 0, -10, 0, 0, 1, 0, "", 0, loopPull, NPulls  );

            }
        }


    }
    */ 
        
    //--- clean the *_cc.d and *_cc.so files ---
    string cmd = "if ls *_cc.d &> .ls_tmp.list; then rm *_cc.d; fi";
    system(cmd.c_str());
    cmd = "if ls *_cc.so &> .ls_tmp.list; then rm *_cc.so; fi";
    system(cmd.c_str());
    cmd = "if ls " + srcdir + "*_cc.d &> .ls_tmp.list; then rm " + srcdir + "*_cc.d; fi";
    system(cmd.c_str());
    cmd = "if ls " + srcdir + "*_cc.so &> .ls_tmp.list; then rm " + srcdir + "*_cc.so; fi";
    system(cmd.c_str());
    system("rm .ls_tmp.list");

    return 0;
}
