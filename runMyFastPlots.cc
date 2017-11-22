{
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

    myFastPlots("SMu","lepEta_Zexc0jet_leppt_25_30",true);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_25_30",false);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_30_35",true);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_30_35",false);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_35_40",true);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_35_40",false);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_40_45",true);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_40_45",false);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_45_above",true);      
    myFastPlots("SMu","lepEta_Zexc0jet_leppt_45_above",false);      
    
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
