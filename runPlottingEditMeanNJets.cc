{
    string srcdir = "Sources/";
    
    vector<string> sources;
    sources.push_back("getFilesAndHistograms");
    sources.push_back("PlottingEditMeanNJets");
    
    
    //--- Load shaared libraries ---
    unsigned int nSources = sources.size();
    gSystem->AddIncludePath("-D__USE_XOPEN2K8");
    gROOT->ProcessLine(".L /afs/cern.ch/cms/slc5_amd64_gcc434/external/lhapdf/5.8.5/lib/libLHAPDF.so");
    for (unsigned int i(0); i < nSources; i++){
        cout <<"Compiling " << srcdir + sources[i] << ".cc" << endl;
        gROOT->ProcessLine(string(".L " + srcdir + sources[i] + ".cc++").c_str());
        }
        
        //PlottingEditMeanNJets(48, 49);
        PlottingEditMeanNJets(48, 52);
        
        
        
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
