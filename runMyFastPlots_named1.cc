void runMyFastPlots_named1(string strWCharge, string strVariable, string strRange, string strClosureTest, string strSystematics, string strDirection){

  string srcdir = "Sources/";

  vector<string> sources;
  sources.push_back("getFilesAndHistograms");
  sources.push_back("functions");
  sources.push_back("writeFastPlotsTex");
  sources.push_back("unfoldingFunctions");
  sources.push_back("myFastPlots");

  //--- Load shaared libraries ---
  unsigned int nSources = sources.size();
  gROOT->ProcessLine(".L /cvmfs/cms.cern.ch/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
  for (unsigned int i(0); i < nSources; i++){
      cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
      gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
      //gROOT->LoadMacro(string(srcdir + sources[i] + ".cc++").c_str());
  }

  myFastPlots(strWCharge, strVariable, strRange, strClosureTest, strSystematics, strDirection);   
 
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
