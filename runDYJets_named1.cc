void runDYJets_named1(string strWCharge, string strLineNum,string strDoQCD,string strSystematics,string strDirection){

  cout << "We are running on : " << strLineNum << "  " << strDoQCD << "  " << strSystematics << "  " << strDirection << endl; 
  
  string srcdir = "Sources/";
  vector<string> sources;
  sources.push_back("getFilesAndHistograms");
  sources.push_back("functions");
  sources.push_back("funcReweightResp");
  sources.push_back("HistoSet");
  //sources.push_back("muresolution");   
  sources.push_back("rochcor2012wasym_hasan");   
  sources.push_back("ZJetsAndDPS");

  ////--- Load shaared libraries ---
  unsigned int nSources = sources.size();
  gSystem->AddIncludePath("-D__USE_XOPEN2K8");
  gROOT->ProcessLine(".L /cvmfs/cms.cern.ch/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
  for (unsigned int i(0); i < nSources; i++) {
      std::cout << "Compiling " << srcdir + sources[i] << ".cc" << std::endl;
      gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc+").c_str());
  }
  
  ///////////////////Assign the arguments to internal parameters/////////////////////////
  stringstream strStreamWCharge;
  stringstream strStreamLineNum;
  stringstream strStreamDOQCD;
  stringstream strStreamSystematics;
  stringstream strStreamDirection;

  int intWCharge     =   0;
  int intLineNum     =   0;
  int intDOQCD       =  -1;
  int intSystematics =  -1;
  int intDirection   = 100;

  strStreamWCharge     << strWCharge;
  strStreamWCharge     >> intWCharge;
  strStreamLineNum     << strLineNum;
  strStreamLineNum     >> intLineNum;
  strStreamDOQCD       << strDoQCD;
  strStreamDOQCD       >> intDOQCD;
  strStreamSystematics << strSystematics;
  strStreamSystematics >> intSystematics;
  strStreamDirection   << strDirection;
  strStreamDirection   >> intDirection;

  ///////////////////Check if parameters are within respective ranges///////////////////
  if(abs(intWCharge)>1){
    cout << "You entered WCharge = " << intWCharge << endl;
    cout << "It should be either 1 or -1" << endl;
    cout << "Exiting Now." << endl;
    exit(0);
  }

  //if(intLineNum > 928){
  if(intLineNum > 102){
    cout << "You entered Line Number = " << intLineNum << endl;
    cout << "It should be lesser than 35 " << endl;
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
   
  for(int j=0; j<11; j++){
    if(intSystematics == possibleSystematics[j]) {
      matchesSysteamtics = true;
    }
    else continue;
  }
  if(!matchesSysteamtics) {
    cout << "You entered systematics = " << strSystematics << endl;
    cout << "Please choose one among these values : 0  1  2  3  4  5  6  7  8  11 100" << endl;
    cout << "Exiting Now." << endl << endl;
    exit(0);
  }
  
  if(intDirection != -1 && intDirection!=0 && intDirection!=1 && intDirection!=100) {
    if(intSystematics!=100){
      cout << "You entered direction = " << intDirection << endl;
      cout << "It can be either 1 or 0 or -1 " << endl;
      cout << "Exiting Now." << endl << endl;
      exit(0);
    }
  }
  
   
  ///////////////////READ Input_DB_Summary.txt file/////////////////////////
  cout << "READING Input_DB_Summary.txt file " << endl;
  ifstream inputFile("/home/bsutar/t3store2/condor/Input_DB_Summary.txt");
  string textline;
  const char *linest;

  vector<string> filePath;
  stringstream strStreamFileName;
  char split_char = '/';

  string strDOWHAT, strCI, strTotEvents, strEvents, strPartition, strCurretPar, strFileName;
  string strDOWHATofInterest, strCIofInterest, strTotEventsofInterest, strEventsofInterest, strPartitionofInterest, strCurretParofInterest, strFileNameofInterest;

  //for(int k=1; k<=928; k++){
  for(int k=1; k<=102; k++){
    inputFile >> strDOWHAT >> strCI >> strTotEvents >> strEvents >> strPartition >> strCurretPar >> strFileName;
    if(k==intLineNum){
      strDOWHATofInterest    = strDOWHAT;
      strCIofInterest        = strCI;
      strTotEventsofInterest = strTotEvents;
      strEventsofInterest    = strEvents; 
      strPartitionofInterest = strPartition;
      strCurretParofInterest = strCurretPar;
      strFileNameofInterest  = strFileName;
      cout << "Parameters read : " << strDOWHATofInterest << "  " << strCIofInterest << "  " << strTotEventsofInterest << "  " << strEvents << "  " << strPartitionofInterest << "  " << strCurretParofInterest << "  " << strFileNameofInterest << endl;
      strStreamFileName << strFileNameofInterest;
      for(string each; getline(strStreamFileName, each, split_char); filePath.push_back(each));
    }
  }

  //for(vector<string>::iterator it=filePath.begin(); it!=filePath.end(); ++it){
    //cout << *it << endl;
  //}

  
  int counterFileName = 0;
  int counterDOWHAT = 0;
  
  stringstream strStreamDOWHAT;
  strStreamDOWHAT << strDOWHATofInterest;
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
                           // 100 - Both the respective directions 
  int doBJets      = -1;
                          // 0 - no infor on B-jets will be used ;
                          // 1, 2 .. require at least 1, 2, .. ; use 2 for ttbar systmatics;
                          // -1, -2, .. veto the event if you have 1 or more, 2 or more .. b-jets ;
                          // 101 - require exactly 1 b-jet
 
      
  //--- Internal configuration (no need to change this)---
      
  string lepSelection = "SMu"; // default lumi is set for double muon dataset
  double muLumi(19.549); // DoubleMu number with pixelCalc
  double eleLumi(19.602); // DoubleEle number with pixelCalc
  if      (lepSelection == "DE")   muLumi = 19.602;
  else if (lepSelection == "SMuE") muLumi = 19.673;
  else if (lepSelection == "SMu")  muLumi = 19.244;
  else if (lepSelection == "SE")   muLumi = 19.174;
      
  bool doRoch       =       0;
  bool doFlat       =       0;
  int  doPUStudy    =     -10;  // default int the ZJets -10
  bool doSSign      =       0;  // contribution of QCD to emu in TTbar 0
  bool doInvMassCut =       0;
  bool doDataEff    =       0;  
      
  int doMETcut      =       0;  // if you want to apply MET cut  0
  int jetPtMin      =      30;  //30
  int jetPtMax      =       0;  // 0 - for no jetPtMax cut
  int ZPtMin        =       0;  //0
  int ZEtaMin       = -999999;  //default value -999999  !!!!!!!  factor 100 to keep things integer .... eta 2.4  = eta Cut 240
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
      
  short dySyst[NSystMC]      = {0, 1, 1,    3,    3,  5, 5, 6, 6, 7, 7, 8};
  short dyDir[NSystMC]       = {0,-1, 1,   -1,    1, -1, 1,-1, 1,-1, 1, 1};
  float dyScale[NSystMC]     = {1, 1, 1, 0.04, 0.04,  1, 1, 1, 1, 1, 1, 1};
      
  
  // Data
  if ( doWhat == 0 || doWhat == 100) {
    for (unsigned int i(0); i < NSystData; i++) {
      if (dataSyst[i] != doSysRunning && doSysRunning != 100) continue;
      if (dataDir[i]  != doDirection  && doDirection  != 100) continue;   
      ZJetsAndDPS SMuData(trimmedFileName, intWCharge, 1., 1, 1, doDataEff, dataSyst[i], dataDir[i], 1, jetPtMin, jetPtMax, ZPtMin, ZEtaMin, ZEtaMax);
      SMuData.Loop(1, 0, intDOQCD, doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
    }
  }
  
  // Background
  if ( doWhat == 1 || doWhat == 100 ){
    for (unsigned int i(0); i < NSystMC; i++){
      if (bgSyst[i] != doSysRunning && doSysRunning != 100) continue;
      if (bgDir[i]  != doDirection  && doDirection  != 100) continue;
      if(trimmedFileName.find("_TTJets_")!=string::npos){
        ZJetsAndDPS SMuTT(trimmedFileName, intWCharge, muLumi*245.           *1000/6923652., 1, 1, !doDataEff, ttSyst[i], ttDir[i], ttScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuTT.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_ZZ_")!=string::npos){
        ZJetsAndDPS SMuZZInc(trimmedFileName, intWCharge, muLumi*17.654        *1000/9799908.,  1, 1, !doDataEff, zzSyst[i], zzDir[i], zzScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuZZInc.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_WZ_")!=string::npos){
        ZJetsAndDPS SMuWZInc(trimmedFileName, intWCharge, muLumi*33.21         *1000/10000283., 1, 1, !doDataEff, wzSyst[i], wzDir[i], wzScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuWZInc.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_WW_")!=string::npos){
        ZJetsAndDPS SMuWWInc(trimmedFileName, intWCharge, muLumi*54.838        *1000/10000431., 1, 1, !doDataEff, wwSyst[i], wwDir[i], wwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuWWInc.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_T_s_channel_")!=string::npos){
        ZJetsAndDPS SMuT(trimmedFileName, intWCharge, muLumi*3.79           *1000/259961.,  1, 1, !doDataEff, tcsSyst[i], tcsDir[i], tcsScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuT.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_T_t_channel_")!=string::npos){
        ZJetsAndDPS SMuT1(trimmedFileName, intWCharge, muLumi*56.4           *1000/3758227., 1, 1, !doDataEff, tctSyst[i], tctDir[i], tctScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuT1.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_T_tW_channel_")!=string::npos){
        ZJetsAndDPS SMuT2(trimmedFileName, intWCharge, muLumi*11.1           *1000/497658.,  1, 1, !doDataEff, tcwSyst[i], tcwDir[i], tcwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuT2.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_Tbar_s_channel_")!=string::npos){
        ZJetsAndDPS SMuTbar(trimmedFileName, intWCharge, muLumi*1.76           *1000/139974.,  1, 1, !doDataEff, tcsSyst[i], tcsDir[i], tcsScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuTbar.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_Tbar_t_channel_")!=string::npos){
        ZJetsAndDPS SMuTbar1(trimmedFileName, intWCharge, muLumi*30.7           *1000/1903681., 1, 1, !doDataEff, tctSyst[i], tctDir[i], tctScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuTbar1.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_Tbar_tW_channel_")!=string::npos){
        ZJetsAndDPS SMuTbar2(trimmedFileName, intWCharge, muLumi*11.1           *1000/493460.,  1, 1, !doDataEff, tcwSyst[i], tcwDir[i], tcwScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuTbar2.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_DYJets10to50_")!=string::npos){
        ZJetsAndDPS SMuDY10(trimmedFileName,  intWCharge, muLumi*860.5          *1000/11707222., 1, 1, !doDataEff, bgSyst[i], bgDir[i], bgScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax);
        SMuDY10.Loop(1, 0, intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch);
      }
      if(trimmedFileName.find("_DYJets_MIX_UNFOLDING_")!=string::npos){
        ZJetsAndDPS SMuDYMix(trimmedFileName, intWCharge, muLumi*3531.8*1000/30459503., 1., 1, !doDataEff, dySyst[i], dyDir[i], dyScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
        SMuDYMix.Loop(1, 0,  intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch );
      }
    }
  }
  
  // This is unfolding part for W+jets 
  if ( doWhat == 3 || doWhat == 100 ){
    int doGen = 0 ;
    if ( (lepSelection.find("SE") == 0 || lepSelection.find("SMu") == 0 ) && lepSelection.find("SMuE") == -1 )  doGen = 1 ;
    for (unsigned int i(0); i < NSystWJets; i++){
      if (!doGen ) continue;
      if ( ( lepSelection.find("SMu") == 0 || lepSelection.find("SE") == 0 ) && wjSyst[i] == 3) continue; // xsec -- not done for SMu ---
      if (wjSyst[i] != doSysRunning && doSysRunning != 100) continue;
      if (wjDir[i]  != doDirection  && doDirection  != 100) continue;    
      ZJetsAndDPS SMuWJMix(trimmedFileName, intWCharge, muLumi*36703. *1000/76102995., 1., 1, !doDataEff, wjSyst[i], wjDir[i], wjScale[i], jetPtMin, jetPtMax, ZPtMin , ZEtaMin,    ZEtaMax, 0);
      SMuWJMix.Loop(1, doGen,  intDOQCD,  doSSign, doInvMassCut, doBJets, doPUStudy, doFlat, doRoch );
    }
  }
   
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
      
}
