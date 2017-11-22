
#define PI 3.14159265359
#define BARREDEPROGRESSION 0
#define DEBUG 0
#define PRINTEVENT 1
#define whichW             1   // 0:both Ws, 1:plusW, -1:minusW

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>
#include <TDatime.h>
#include <TMath.h>
#include <TRandom3.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "TProfile.h"
#include "LHAPDF/LHAPDF.h"

//#include "muresolution.h"
#include "rochcor2012wasym_hasan.h"
#include "functions.h"
#include "getFilesAndHistograms.h"
#include "standalone_LumiReWeighting.h"
#include "funcReweightResp.h"
#include "HistoSet.h"
#include "ZJetsAndDPS.h"

using namespace std;


ClassImp(ZJetsAndDPS);

void ZJetsAndDPS::Loop(bool hasRecoInfo, bool hasGenInfo, int doQCD, bool doSSign, bool doInvMassCut, 
        int doBJets, int doPUStudy, bool doFlat, bool useRoch, bool doVarWidth,  bool hasPartonInfo, string pdfSet, int pdfMember)
{

    ifstream infile("KinematicVariables.txt");
    float colval1, colval2, colval3, colval4, colval5, colval6;
    
    //---    
    float GenMuon1PtMin, GenMuon1PtMax, GenMuon1EtaMin, GenMuon1EtaMax;
    float GenMuon2PtMin, GenMuon2PtMax, GenMuon2EtaMin, GenMuon2EtaMax;

    float RecMuon1PtMin, RecMuon1PtMax, RecMuon1EtaMin, RecMuon1EtaMax, RecMuon1Iso;
    float RecMuon2PtMin, RecMuon2PtMax, RecMuon2EtaMin, RecMuon2EtaMax, RecMuon2Iso;

    float GenJetPtMin,   GenJetPtMax,   GenJetEtaMin,   GenJetEtaMax;
    float RecJetPtMin,   RecJetPtMax,   RecJetEtaMin,   RecJetEtaMax;

    float GenV_MassMin,  GenV_MassMax,  GenV_PtMin,     GenV_PtMax,     GenV_EtaMin,  GenV_EtaMax;
    float RecV_MassMin,  RecV_MassMax,  RecV_PtMin,     RecV_PtMax,     RecV_EtaMin,  RecV_EtaMax;

    string textline;
    const char *linest;

    for(int i=0; i<22; i++){
      getline(infile,textline); linest=textline.c_str();
      if(textline.find("//")  != string::npos)continue;
      if(textline.find("GenMuon1")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f", &colval1, &colval2, &colval3, &colval4);
        GenMuon1PtMin=colval1; GenMuon1PtMax=colval2; GenMuon1EtaMin=colval3; GenMuon1EtaMax=colval4;
        cout << "GenMuon1  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4 << endl;
      }

      if(textline.find("GenMuon2")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f", &colval1, &colval2, &colval3, &colval4);
        GenMuon2PtMin=colval1; GenMuon2PtMax=colval2; GenMuon2EtaMin=colval3; GenMuon2EtaMax=colval4;
        cout << "GenMuon2  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4 << endl;
      }

      if(textline.find("RecMuon1")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f %f", &colval1, &colval2, &colval3, &colval4, &colval5);
        RecMuon1PtMin=colval1;  RecMuon1PtMax=colval2;  RecMuon1EtaMin=colval3;
        RecMuon1EtaMax=colval4; RecMuon1Iso=colval5;
        cout << "RecMuon1  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4 << "   " << colval5 << endl;
      }      

      if(textline.find("RecMuon2")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f %f", &colval1, &colval2, &colval3, &colval4, &colval5);
        RecMuon2PtMin=colval1;  RecMuon2PtMax=colval2;  RecMuon2EtaMin=colval3;
        RecMuon2EtaMax=colval4; RecMuon2Iso=colval5;
        cout << "RecMuon2  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4 << "   " << colval5 << endl;
      }

      if(textline.find("GenJet")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f", &colval1, &colval2, &colval3, &colval4);
        GenJetPtMin=colval1; GenJetPtMax=colval2; GenJetEtaMin=colval3; GenJetEtaMax=colval4;
        cout << "GenJet  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4 << endl;
      }

      if(textline.find("RecJet")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f", &colval1, &colval2, &colval3, &colval4);
        RecJetPtMin=colval1; RecJetPtMax=colval2; RecJetEtaMin=colval3; RecJetEtaMax=colval4;
        cout << "RecJet  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4 << endl;
      }

      if(textline.find("GenV")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f %f %f", &colval1, &colval2, &colval3, &colval4, &colval5, &colval6);
        GenV_MassMin=colval1; GenV_MassMax=colval2; GenV_PtMin=colval3;
        GenV_PtMax=colval4;   GenV_EtaMin=colval5;  GenV_EtaMax=colval6;
        cout << "GenV  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4;
        cout << "   " << colval5 << "   " << colval6 << endl;
      }

      if(textline.find("RecV")!=string::npos){
        sscanf(linest, "%*s %f %f %f %f %f %f", &colval1, &colval2, &colval3, &colval4, &colval5, &colval6);
        RecV_MassMin=colval1; RecV_MassMax=colval2;  RecV_PtMin=colval3;
        RecV_PtMax=colval4;   RecV_EtaMin=colval5;   RecV_EtaMax=colval6;
        cout << "RecV  " << colval1 << "   " << colval2 << "  " << colval3 << "  " << colval4;
        cout << "   " << colval5 << "   " << colval6 << endl;
      }
    }
    infile.close(); 

    //--- Initialize PDF from LHAPDF if needed ---
    if (pdfSet != "") {
        LHAPDF::initPDFSet(1, pdfSet.c_str(), pdfMember);
        LHAPDF::initPDFSet(2, "CT10.LHgrid");
        const int numberPDFS(LHAPDF::numberPDF() + 1);
        if (pdfMember > numberPDFS) {
            std::cout << "Warning pdfMember to high" << std::endl;
            return;
        }
    }
    //--------------------------------------------

    //--- Check weither it is 7 or 8 TeV ---
    string energy = getEnergy();
    //--------------------------------------

    //--- Counters to check the yields ---
    unsigned int nEvents(0), nEventsIncl0Jets(0), nEventsUNFOLDIncl0Jets(0);
    unsigned int nEventsWithTwoGoodLeptonsNoChargeNoMass(0), nEventsWithTwoGoodLeptonsNoMass(0), nEventsWithTwoGoodLeptons(0);
    unsigned int nEventsExcl0Jets(0), nEventsExcl1Jets(0), nEventsExcl2Jets(0), nEventsExcl3Jets(0),nEventsIncBJets(0);
    unsigned int GENnEventsIncl0Jets(0), GENnEventsIncl1Jets(0), GENnEventsIncl2Jets(0), GENnEventsIncl3Jets(0);
    double TotalGenWeight(0.), TotalGenWeightPassGEN(0.), TotalGenWeightPassGENPU(0.), TotalGenWeightPassRECO(0.), TotalRecoWeightPassRECO(0.);
    //------------------------------------
    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    double MTCut = RecV_MassMin;
    double ZMCutLow(71), ZMCutHigh(111);
    //------------------------------------
    bool doZ(true), doW(false), doTT(false), doDR(false), doTTreweighting(false);
    if (leptonFlavor == "SingleElectron" || leptonFlavor == "SingleMuon"){
        doW = true; 
        doDR = true;
    }
    if (leptonFlavor == "TTMuE") doTT = true; 
    if (doW || doTT) doZ = false;
    if (fileName.find("_dR_") != string::npos) doDR = true;
    if (fileName.find("TopReweighting") != string::npos) { hasGenInfo = true ; doTTreweighting = true;} // we don't want to use gen plots for ttbar, we just need to load the lepton branch to read the t qurak pt 
    if ( doZ ) METcut = 0; // no need for MET cut on Z+jets analysis 
    if ( doW ) METcut = RecMuon2PtMin; // remove this if no MET cut (default value is 0)
    
    // additional muons variables
    double leptonMass(0.00051);
    int LeptonID(11);
    if (leptonFlavor == "Muons" || leptonFlavor == "SingleMuon"){
        leptonMass = 0.105658;
        LeptonID = 13;
    }
    //------------------------------------
    cout << " begin: "<< hasRecoInfo <<"  " << hasGenInfo <<"  " << doQCD<<"  " << doSSign<<"  " << doInvMassCut << "  " << METcut << "  " <<doBJets <<"  " <<doPUStudy << endl;
    
    
    //==========================================================================================================//
    //         Output file name           //
    //===================================//
    string command = "mkdir -p " + outputDirectory;
    system(command.c_str());
    string outputFileName = CreateOutputFileName(useRoch, doFlat, doPUStudy, doVarWidth, doBJets, doQCD, doSSign , doInvMassCut, pdfSet, pdfMember);
    TFile *outputFile = new TFile(outputFileName.c_str(), "RECREATE");
    //==========================================================================================================//

    
    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    //       Load efficiency tables        //
    //====================================//
    table LeptIso, LeptID, LeptTrig, Ele_Rec;
    table TableJESunc("EfficiencyTables/JESUnce_FT_53_V21_AN4_Uncertainty_AK5PFchs.txt");
    if (energy == "8TeV"){
        if (leptonFlavor == "Electrons" || leptonFlavor == "SingleElectron"){
            /// electron SF
            table Ele_Rec_8TeV("EfficiencyTables/Ele_SF_Reconstruction_2012.txt");
            table SC_Ele_2012EA("EfficiencyTables/Ele_SF_EA2012.txt");
            
            Ele_Rec = Ele_Rec_8TeV ;
            LeptID = SC_Ele_2012EA;
        }
        if (leptonFlavor == "SingleMuon")  {
            // Single Muon SF for 2012 data (Rereco on Jan2013)
            table SF_Muon_IDTight_Rereco("EfficiencyTables2012/scale_factors_reco_id_iso_muplus_hogul.txt");
            //table SF_Muon_IDTight_Rereco("EfficiencyTables2012/Muon_IDTight_Efficiencies_ReReco2012_Eta_Pt.txt");
            table SF_Muon_ISOTight_ReReco("EfficiencyTables2012/Muon_ISOTight_forTight_Efficiencies_ReReco2012_Eta_Pt.txt");
            table SF_TrigIsoMu24eta2p1_ReReco("EfficiencyTables2012/scale_factors_trig_muplus_hogul.txt");
            //table SF_TrigIsoMu24eta2p1_ReReco("EfficiencyTables2012/Efficiency_SF_ReReco2012_IsoMu24_eta2p1.txt");
            
            LeptID = SF_Muon_IDTight_Rereco;
            LeptIso = SF_Muon_ISOTight_ReReco;
            LeptTrig = SF_TrigIsoMu24eta2p1_ReReco;
        }
    }
    //==========================================================================================================//
    cout << "Phase space cuts -- jet pt:" << RecJetPtMin <<"  " << RecJetPtMax<<"  -- jet eta : " << RecJetEtaMin<< "  " << RecJetEtaMax<< "  " << "  -- Z eta: " << ZEtaCutMin<<"   " << ZEtaCutMax<< "  -- MET cut: " << METcut << "    "   << endl;
    cout << " other selections:  " <<endl;
    cout << " doQCD: " << doQCD <<"  do SS: " << doSSign <<" inv. mass cut: " << doInvMassCut <<"  use MET cut: " << METcut<<"  use B jets: " << doBJets <<" do PU study: " << doPUStudy << endl;


    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    //     Systematics: jec, pu, xsec     //
    //====================================//
    cout << "Lepton Flavor: " << leptonFlavor << endl;
    int puYear(2011); 
    if (energy == "8TeV") puYear = 2013;
    cout << "Pile Up Distribution: " << puYear << endl;
    standalone_LumiReWeighting puWeight(leptonFlavor, puYear), puUp(leptonFlavor, puYear, 1), puDown(leptonFlavor, puYear, -1);
    cout << "systematics: " << systematics << "  direction: " << direction << endl;
    if (systematics == 1 && direction ==  1) puWeight = puUp;
    if (systematics == 1 && direction == -1) puWeight = puDown;

    int scale(0);//0,+1,-1; (keep 0 for noJEC shift study)
    if (systematics == 2 && direction ==  1) scale =  1;
    if (systematics == 2 && direction == -1) scale = -1;

    double xsec(1.);
    if (systematics == 3 && direction ==  1) xsec = 1. + xsecfactor;
    if (systematics == 3 && direction == -1) xsec = 1. - xsecfactor;

    int smearJet(0);
    if (systematics == 4 && direction ==  1) smearJet =  1;
    if (systematics == 4 && direction == -1) smearJet = -1;
    
    int sysLepSF(0);
    if (systematics == 5 && direction ==  1) sysLepSF =  1;
    if (systematics == 5 && direction == -1) sysLepSF = -1;
    
    int sysBtagSF(0);
    if (systematics == 6 && direction ==  1) sysBtagSF =  1;
    if (systematics == 6 && direction == -1) sysBtagSF = -1;
    
    double muScale(1.0);
    if (systematics == 7 && direction ==  1) muScale = 1.002;
    if (systematics == 7 && direction == -1) muScale = 0.998;
    
    bool doMer(false); // the number used for MER : 0.006
    double merUncer(0);
    if (systematics == 8 && direction ==  1) doMer = true;
    
    // Wb study
    bool doWbsyst(false);
    double WbSystSF(1.3);
    if (systematics == 9 && direction ==  1) doWbsyst = true;
    
    bool doRespSyst(false);
    if (systematics == 10 && direction ==  1) doRespSyst = true;
    
    //int smearLepSF(0);
    //if ((systematics == 5 || systematics == 6) && direction ==  1) smearLepSF = 1;
    //if ((systematics == 5 || systematics == 6) && direction == -1) smearLepSF = -1;
    
    TRandom3* RandGen = new TRandom3();
    RandGen->SetSeed(22346);
    if (sysBtagSF != 0) RandGen->SetSeed(333);
    
    TRandom3* Rand_MER_Gen = new TRandom3();
    //Rand_MER_Gen->SetSeed(0); // set random seed to random

    //==========================================================================================================//


    // initialize rochester corrrection
    rochcor2012 *rmcor = new rochcor2012(); // make the pointer of rochcor class
    //REMARK : Need to call "rochcor(seed)" to assign the systematic error
    //rochcor2012 *rmcor = new rochcor2012(seed); //where "seed" is the random seed number

    //---  Retreive the NVtx comparison histogram to have the exact weight to re-weight for pile-up
    // and have a flat NVtx distribution
    if (doFlat){
        string nVtxFileName = "Includes/";
        if (leptonFlavor == "Muons") nVtxFileName += "DMu_NVtx.root";
        else if (leptonFlavor == "Electrons") nVtxFileName += "DE_NVtx.root";
        TFile *DataMCRawComparison = new TFile(nVtxFileName.c_str());
        TCanvas *can = (TCanvas*) DataMCRawComparison->Get("NVtx");
        TPad *pad = (TPad*) can->FindObject("pad2");
        FlatNVtxWeight = (TH1D*) pad->FindObject("NVtx");
    }

    int NZtotal = 0 ; // for Z counting
    double sumSherpaW = 0. ;
    double sumSherpaW0 = 0. ;
    cout << " Sherpa initial weight :  " << sumSherpaW <<endl;

    // setting weight when running on MIX of exclusive DY/WJets files to match number of parton events
    double mixingWeightsDY[4] = {0.1926862,  0.07180968,  0.04943502,  0.03603373 }; // here we match all partons, and combine electron and muon side
    double mixingWeightsWJ_SMu[4] = {0.366713,  0.1119323,  0.07641136,  0.03803325};
    double mixingWeightsWJ_SE[4]  = {0.3667127984048746, 0.111932213229137, 0.076411344088767, 0.0380331330318}; // this need to be updated
    
    //==========================================================================================================//
    // Start looping over all the events //
    //===================================//
    cout << endl;
    printf("\nProcessing : %s    -->   %s \n", fileName.c_str(), outputFileName.c_str());

    //--- Initialize the tree branches ---
    Init(hasRecoInfo, hasGenInfo, hasPartonInfo);
    if (fChain == 0) return;
    Long64_t nentries = fChain->GetEntries();
    //Long64_t nentries = fChain->GetEntriesFast();
    if (nEvents_10000) {
        nentries = 10000;
        std::cout << "We plane to run on 100000 events" << std::endl;
    }
    //------------------------------------
    
    //--- get weight and bin edges for systematic for reweighted response
    vector<double> vecFwHT1; vector<double> binEdgeHT1;
    vector<double> vecFwHT2; vector<double> binEdgeHT2;
    vector<double> vecFwRap12; vector<double> binEdgeRap12;
    vector<double> vecFwRapFB; vector<double> binEdgeRapFB;
    if (doRespSyst){
        vecFwHT1     = getFW("MeanNJetsHT_Zinc1jet");
        binEdgeHT1   = getXBin("MeanNJetsHT_Zinc1jet");
        vecFwHT2     = getFW("MeanNJetsHT_Zinc2jet");
        binEdgeHT2   = getXBin("MeanNJetsHT_Zinc2jet");
        vecFwRap12   = getFW("MeanNJetsdRapidity_Zinc2jet");
        binEdgeRap12 = getXBin("MeanNJetsdRapidity_Zinc2jet");
        vecFwRapFB   = getFW("MeanNJetsdRapidityFB_Zinc2jet");
        binEdgeRapFB = getXBin("MeanNJetsdRapidityFB_Zinc2jet");
    }
    //------------------------------------
    
    std::cout << "We will run on " << nentries << " events" << std::endl;

    //--- Begin Loop All Entries --
    //for (Long64_t jentry(0); jentry < nentries; jentry++){
    for (Long64_t jentry(0); jentry < 10; jentry++){
        Long64_t ientry = LoadTree(jentry);
        //cout << "ev:\t\t*******\t\t" << jentry << endl;
        if (ientry < 0) break;
        if (jentry % 100000 == 0) std::cout << jentry << std::endl;
        fChain->GetEntry(jentry);
        nEvents++;
        //=======================================================================================================//

        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //         Computing weight           //
        //====================================//
        //--- weight variable ---
        double weight(1.);
        double genWeight(1.);
        double reweighting(1);
        //-----------------------

        //--- line below is to see distributions as provided with default MC PU distribution
        //--- for data PU_npT == -2
        if (hasRecoInfo && !isData){
            weight *= (double)puWeight.weight(int(PU_npT));
            //-- reweight again to IMPOSE FLAT #VTX DATA/MC RATIO
            if (doFlat){
                reweighting = FlatNVtxWeight->GetBinContent(EvtInfo_NumVtx + 1);
                //-- for safety check the value of the weight...
                if (reweighting <= 0 || reweighting > 1000) reweighting = 1;
                weight *= reweighting;
            }
        }
        if (weight > 10000 || weight < 0) weight = 1;
        weight = weight * lumiScale * xsec;
        
        //--- apply mixing weight
        if (fileName.find("MIX") != string::npos && nup_ > 5) {
            if      (fileName.find("DYJets") != string::npos) weight *= mixingWeightsDY[nup_ - 6];
            else if (fileName.find("WJets" ) != string::npos && fileName.find("SMu_") != string::npos) weight *= mixingWeightsWJ_SMu[nup_ - 6];
            else if (fileName.find("WJets" ) != string::npos && fileName.find("SE_" ) != string::npos) weight *= mixingWeightsWJ_SE[nup_ - 6];
            else weight *= mixingWeightsDY[nup_ - 6];
        }
        
        //--- apply weight for Sherpa2 and other generators
        //        if (fileName.find("mcEveWeight") != string::npos || fileName.find("MiNLO") != string::npos) {
        //            weight *= mcEveWeight_;
        //        }
        //        if (fileName.find("HepMC") != string::npos) {
        //            weight *= mcEveWeight_;
        //            //sumSherpaW += mcSherpaSumWeight3_ ;
        //        }
        
        if (fileName.find("HEJ") != string::npos){
            weight *= mcSherpaWeights_->at(0);
        }
        if (fileName.find("Sherpa2") != string::npos){
            weight *= mcSherpaWeights_->at(0);
            sumSherpaW += mcSherpaWeights_->at(4);
            sumSherpaW0 += mcSherpaWeights_->at(0);
        }

        //==========================================================================================================//
        // Compute the weight for PDF syst   //
        //===================================//
        //-- get the pdgId of the two colliding partons 
        double wPdf(1);
        if (pdfSet != "") {
            int id1 = pdfInfo_->at(2);
            int id2 = pdfInfo_->at(3);
            if (id1 == 21) id1 = 0;
            if (id2 == 21) id2 = 0;

            LHAPDF::usePDFMember(2, 0);
            double pdf1 = LHAPDF::xfx(1, pdfInfo_->at(2), pdfInfo_->at(4), id1);
            double pdf2 = LHAPDF::xfx(1, pdfInfo_->at(3), pdfInfo_->at(4), id2);
            double pdf01 = LHAPDF::xfx(2, pdfInfo_->at(2), pdfInfo_->at(4), id1);
            double pdf02 = LHAPDF::xfx(2, pdfInfo_->at(3), pdfInfo_->at(4), id2);

            if (pdfInfo_->at(2) * pdfInfo_->at(3) > 0) {
                wPdf = pdf1 * pdf2;
                if (pdf01*pdf02 <= 0 || pdf1*pdf2 <= 0) {
                    wPdf = 1;
                }
                else {
                    wPdf /= (pdf01 * pdf02);
                }
            }
        }
        //==========================================================================================================//

        //--- There is no pile-up so no need to reweight for that ---
        genWeight = weight * wPdf;
        double genWeightBackup(genWeight);
        TotalGenWeight += genWeightBackup;
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //         Retrieving leptons           //
        //====================================//
        bool doMuons(leptonFlavor == "Muons" || doW || doTT);
        bool doElectrons(leptonFlavor == "Electrons" || doW || doTT);
        bool passesLeptonCut(0);
        bool passesWhichWCut(0);
        bool passesLeptonReq(0), passesLeptonAndMT(0), passesBtagReq(1), passesTau3Req(1);
        unsigned short nTotLeptons(0), nLeptons(0), nMuons(0), nElectrons(0);
        vector<leptonStruct> leptons, muons, electrons, selLeptons;
        TLorentzVector lep1, lep2, Z, roch_mu;
        leptonStruct lepton1 = {0, 0, 0, 0, 0, 0, 0};
        leptonStruct lepton2 = {0, 0, 0, 0, 0, 0, 0};
        double METphi(0), METpt(0), MT(0);
        int whichMet(2); //  0 - pfMETPFlow, 1 - pfMet, 2 - pfType1CorrectedMet, 3 - pfType1p2CorrectedMet
        float qter = 1.0;
        float sysdev = 0.0;

        int sumLepCharge(1000);
        if (hasRecoInfo && doTT && patMetPt_->at(whichMet) < METcut) continue; 
        if (hasRecoInfo) {

            bool eventTrigger = false;
            //--- DO MUONS ---
            if (doMuons){
                nTotLeptons = patMuonEta_->size();
                // test if any of the leptons is atached to trigger
                // if we don't really care to match both leptons to trigger
                // we also have event trigger variables --> we should at least match one of the leptons to trigger
                for (unsigned short i(0); i < nTotLeptons; i++) {
                    int whichTrigger(patMuonTrig_->at(i));
                    if (energy == "7TeV" && whichTrigger > 0) eventTrigger = true;
                    //if (energy == "8TeV" && (whichTrigger % 2) == 1 && doW) eventTrigger = true;
                    if (energy == "8TeV" && (whichTrigger & 0x1) && doW) eventTrigger = true;
                    if (energy == "8TeV" && doTT && whichTrigger >= 16) eventTrigger = true; // for TT background
                }

                for (unsigned short i(0); i < nTotLeptons; i++) {
                    if (doMer) merUncer = Rand_MER_Gen->Gaus(0, (patMuonPt_->at(i) * 0.006));
                    if(useRoch && isData){
                        roch_mu.SetPtEtaPhiE(patMuonPt_->at(i), patMuonEta_->at(i), patMuonPhi_->at(i), patMuonEn_->at(i));
                        rmcor->momcor_data(roch_mu,patMuonCharge_->at(i),0,qter);
                    }
                    if(useRoch && !isData){
                        roch_mu.SetPtEtaPhiE(patMuonPt_->at(i), patMuonEta_->at(i), patMuonPhi_->at(i), patMuonEn_->at(i));
                        rmcor->momcor_mc(roch_mu,patMuonCharge_->at(i),0,qter);
                    }
 
                    leptonStruct mu;
                    if(!useRoch){
                        mu = {(patMuonPt_->at(i) * muScale) + merUncer, patMuonEta_->at(i), patMuonPhi_->at(i), patMuonEn_->at(i), patMuonCharge_->at(i), patMuonPfIsoDbeta_->at(i), 0};
                    }
                    if(useRoch) {
                        mu = {(roch_mu.Pt() * muScale) + merUncer, roch_mu.Eta(), roch_mu.Phi(), roch_mu.E(), patMuonCharge_->at(i), patMuonPfIsoDbeta_->at(i), 0};
                    }

                    bool muPassesPtCut(( (doZ || doTT) && mu.pt >= 20.) || (doW && mu.pt >= RecMuon1PtMin));
                    
                    bool muPassesEtaLooseCut(fabs(mu.eta) <= 2.4);
                    bool muPassesEtaCut( ((doZ || doTT) && muPassesEtaLooseCut) || (doW && fabs(mu.eta) <= RecMuon1EtaMax) );
                    
                    // We use Tight muon, only for tight the patMuonCombId_ is odd
                    bool muPassesIdCut(int(patMuonCombId_->at(i)) % 2 == 1); // this is for tight ID --> odd number
                    //bool muPassesIdCut(int(patMuonCombId_->at(i)) >= 1 ); // this is for Loose ID

                    bool muPassesDxyCut(patMuonDxy_->at(i) < 0.2);
                    bool muPassesIsoCut((!doW && patMuonPfIsoDbeta_->at(i) < 0.2) || (doW && patMuonPfIsoDbeta_->at(i) < RecMuon1Iso));  
                    bool muPassesQCDIsoCut(doW && patMuonPfIsoDbeta_->at(i) >= 0.2); // use 0.12 if you want to cover full Iso space
                    
                    int whichTrigger(patMuonTrig_->at(i));
                    bool muPassesEMuAndWJetsTrig( whichTrigger == 1 || whichTrigger == 16 || whichTrigger == 17 || whichTrigger == 32 || whichTrigger == 33 || whichTrigger == 48 || whichTrigger ==  49  ) ;
                    bool muPassesAnyTrig((doZ && ((energy == "7TeV" && whichTrigger > 0) || (energy == "8TeV" && whichTrigger > 7 && !muPassesEMuAndWJetsTrig))) ||
                            (doW && whichTrigger % 2 == 1) || (doTT && whichTrigger >= 16)); // 8TeV comment: Mu17Mu8Tk = 4; Mu17Mu8 = 8 
                    /// for files obtained form bugra
                    //if (fileName.find("DYJets_Sherpa_UNFOLDING_dR_5311") != string::npos && whichTrigger > 0) muPassesAnyTrig = 1; // Bugra only keeps the double electron trigger !!!!!

                    // select the good muons only
                    //-- no Isolation Cut
                    if (!doTT && muPassesEtaLooseCut && patMuonCombId_->at(i) > 0 && mu.pt >= 15) muons.push_back(mu);
                    
                    if (doW && fabs(mu.eta) > RecMuon1EtaMax) muPassesEtaCut = false;
                    //if (doTT && fabs(mu.eta) > 2.4) muPassesEtaCut = false;
                    
                    if (muPassesPtCut && muPassesEtaCut && muPassesIdCut && muPassesDxyCut && (!useTriggerCorrection || muPassesAnyTrig || eventTrigger)){
                        // fill isolation histograms for control    
                        MuDetIsoRhoCorr->Fill(patMuonPfIsoDbeta_->at(i), weight);
                        MuPFIsoDBetaCorr->Fill(patMuonPfIsoDbeta_->at(i), weight);
                        //-- isolation Cut
                        if (doQCD > 1 && muPassesQCDIsoCut && leptonFlavor != "SingleElectron") leptons.push_back(mu);
                        if (muPassesIsoCut){  
                            if (doQCD < 2 && leptonFlavor != "SingleElectron") leptons.push_back(mu); 
                            if (doTT && fabs(mu.eta) < 2.4) muons.push_back(mu); 
                        }
                    }
                }//End of loop over all the muons
            }

            //------ DO ELECTRONS -------
            if (doElectrons) {
                nTotLeptons = 0;
                nTotLeptons = patElecEta_->size();
                // if we don't really care to match both leptons to trigger
                if (doW) eventTrigger = false;
                for (unsigned short i(0); i < nTotLeptons; i++){
                    int whichTrigger(patElecTrig_->at(i));
                    if (energy == "7TeV" && whichTrigger > 0) eventTrigger = true; ///
                    if (energy == "8TeV" && (whichTrigger & 0x1) && doW) eventTrigger = true; ///
                    if (energy == "8TeV" && doTT && whichTrigger >= 16) eventTrigger = true; // for TT background ///
                }
                
                for (unsigned short i(0); i < nTotLeptons; i++){
                    leptonStruct ele = {patElecPt_->at(i), patElecEta_->at(i), patElecPhi_->at(i), patElecEn_->at(i),  patElecCharge_->at(i), 0., patElecScEta_->at(i)};
                    int whichTrigger(patElecTrig_->at(i));
                    bool elePassesPtCut( ( !doW && ele.pt >= 20.)  || ( doW && ele.pt >= 30.));
                    
                    bool elePassesEtaLooseCut(fabs(patElecScEta_->at(i)) <= 1.4442 || (fabs(patElecScEta_->at(i)) >= 1.566 && fabs(patElecScEta_->at(i)) <= 2.4));
                    bool elePassesEtaCut( ((doZ || doTT) && elePassesEtaLooseCut) || (doW && elePassesEtaLooseCut && fabs(patElecScEta_->at(i)) <= 2.1) );
                    
                    // We use medium electron id
                    bool elePassesIdCut(int(patElecID_->at(i)) >= 4); /// >=4 is medium ID; >=2 is Loose ID
                    bool elePassesIsoCut(patElecPfIsoRho_->at(i) < 0.15 );

                    bool elePassesEMuAndWJetsTrig(whichTrigger == 1 || whichTrigger == 16 || whichTrigger == 17 || whichTrigger == 32 || whichTrigger == 33 || whichTrigger == 48 || whichTrigger == 49 ) ;
                    bool elePassesAnyTrig(  (doZ && (whichTrigger >= 2 && !elePassesEMuAndWJetsTrig )) || ( doTT && whichTrigger >= 16 ) || ( doW && whichTrigger % 2 == 1) );
                    if ( DEBUG ) cout << EvtInfo_EventNum << "  lepton loop: "<<elePassesAnyTrig <<"   " << ele.pt <<"   " << ele.eta <<"  " <<"  " << patElecEn_->at(i) <<"  " <<elePassesIdCut<<"  SIZE  " << nTotLeptons <<  endl;
                    // elePassesAnyTrig = true ;

                    // select the good electrons only
                    if (!doTT && elePassesEtaLooseCut && int(patElecID_->at(i)) >= 2 && ele.pt >= 15. && patElecPfIsoRho_->at(i) < 0.2 )  electrons.push_back(ele); /// DO I WANT THIS !!!!!!
                    
                    if (doW && fabs(patElecScEta_->at(i)) > 2.1) elePassesEtaCut = false ;
                    
                    if (elePassesPtCut && elePassesEtaCut && elePassesIdCut && (!useTriggerCorrection || elePassesAnyTrig || eventTrigger)){
                        //-- isolation Cut
                        if (doQCD > 1  && !elePassesIsoCut && leptonFlavor != "SingleMuon") leptons.push_back(ele);
                        if ( elePassesIsoCut ) {
                                if (doQCD < 2 && leptonFlavor != "SingleMuon") leptons.push_back(ele);
                                if (doTT) electrons.push_back(ele);
                            }
                    }
                }//End of loop over all the electrons
            }
            
            nMuons = muons.size();
            nElectrons = electrons.size();
            nLeptons = leptons.size();
            
            //--- sort leptons by descending pt ---
            // no need to sort if doW, exactly one leptons is required
            
            vector<leptonStruct> tempVec;
            for ( int iLep = 0 ; iLep < nLeptons ; iLep++){
                tempVec.push_back(leptons[iLep]);
            }
            selLeptons = tempVec ; // selLeptons is the same as leptons
            
            // END IF RECO FOR LEPTONS
        }
        // end has reco info


        if (DEBUG) std::cout << "Stop after line " << __LINE__ << std::endl;
        //=======================================================================================================//
        //       Retrieving gen leptons        //
        //====================================//
        bool passesGenLeptonCut(0);
        bool passesWhichWGenCut(0);
        bool passesGenLeptonReq(0);
        unsigned short nTotGenLeptons(0), nGenLeptons(0), nTotGenPhotons(0);
        vector<leptonStruct> genLeptons;
        vector<int> usedGenPho;
        TLorentzVector genLep1, genLep2, genZ;
        leptonStruct genLepton1, genLepton2;
        double genMT(0);
        int countTauS3 = 0;

        // to use the TOP PAG TTBAR reweighting recommendation
        // line below is to check contribution from mainy tau decay : use passesLeptonCut = 0  only if you want to have RECO events that originate from tau ; countTauS3 is used in passesGenLeptonCut
        // I also put top quarks in this collection
        // ...DELETED
        /// end top reweighting
        if (hasGenInfo) {
            if (hasRecoInfo) countTauS3 = 2;
            if (hasRecoInfo && doW) countTauS3 = 1;
            nTotGenPhotons = genPhoEta_->size();
            nTotGenLeptons = genLepEta_->size();
            //-- retriveing generated leptons with status 1
            for (unsigned short i(0); i < nTotGenLeptons; i++) {
                // line below is to check contribution from mainy tau decay : use passesLeptonCut = 0  only if you want to have RECO events that originate from tau ; countTauS3 is used in passesGenLeptonCut
                bool lepSelector( 
                        (doZ && abs(genLepId_->at(i)) == LeptonID) || 
                        (doW && (abs(genLepId_->at(i)) == LeptonID || abs(genLepId_->at(i)) == 12 || abs(genLepId_->at(i)) == 14)));
                
                
                // following two lines should give the same result
                if (genLepSt_->at(i) == 3 && abs(genLepId_->at(i)) != LeptonID && (abs(genLepId_->at(i)) == 15 || abs(genLepId_->at(i)) == 13 || abs(genLepId_->at(i)) == 11)) countTauS3++;
                if (genLepSt_->at(i) == 3 && abs(genLepId_->at(i)) == LeptonID ) countTauS3--;
                
                
                if (!lepSelector) continue ;
                double charge(genLepQ_->at(i)); 
                if (abs(genLepId_->at(i)) == 12 || abs(genLepId_->at(i)) == 14 || abs(genLepId_->at(i)) == 16) charge = 0.;
                leptonStruct genLep = {genLepPt_->at(i), genLepEta_->at(i), genLepPhi_->at(i), genLepE_->at(i), charge, 0., 0.};
                leptonStruct genLepNoFSR = {genLepPt_->at(i), genLepEta_->at(i), genLepPhi_->at(i), genLepE_->at(i), charge, 0., 0. };
                
                //-- dress the leptons with photon (cone size = 0.1). Only for status 1 leptons (after FSR)
                if ( ( genLepSt_->at(i) == 1 && lepSelector && abs(genLepId_->at(i)) == LeptonID) || ( doW && charge == 0 ) ){
                    // only charged lepton(s) will be dressed
                    if( fabs(genLep.charge) > 0 ){
                        TLorentzVector tmpGenLep;
                        tmpGenLep.SetPtEtaPhiM(genLep.pt, genLep.eta, genLep.phi, leptonMass);
                        // loop over all photons
                        for (unsigned short j(0); j < nTotGenPhotons; j++){
                            TLorentzVector tmpGenPho;
                            tmpGenPho.SetPtEtaPhiM(genPhoPt_->at(j), genPhoEta_->at(j), genPhoPhi_->at(j), 0.);
                            int used(0);
                            for (unsigned short k(0); k < usedGenPho.size(); k++){
                                if (j == usedGenPho[k]) used = 1;
                            }
                            if (deltaR(tmpGenPho.Phi(), tmpGenPho.Eta(), genLepNoFSR.phi, genLepNoFSR.eta) <= 0.1 && !used){
                                tmpGenLep += tmpGenPho;
                                usedGenPho.push_back(j);
                            }
                        }
                        genLep.pt = tmpGenLep.Pt();
                        genLep.eta = tmpGenLep.Eta();
                        genLep.phi = tmpGenLep.Phi();
                        genLep.energy = tmpGenLep.E();
                    }
                    
                    //-- store lepton in the collection
                    if (doZ && genLep.pt >= 20 && fabs(genLep.eta) <= 2.4 && fabs(genLep.charge) > 0){
                        //genLeptons.push_back(genLep);
                        if(whichW==0) genLeptons.push_back(genLep);
                    }

                    if (doW && ((fabs(genLep.charge) > 0 && genLep.pt >= GenMuon1PtMin && fabs(genLep.eta) <= GenMuon1EtaMax) || (fabs(genLep.charge) == 0 && genLep.pt >= GenMuon2PtMin))){
                        if(whichW==0) {
                            genLeptons.push_back(genLep); 
                            passesWhichWGenCut = true;
                        }
                        if(whichW== 1 && (genLep.charge<0 || genLep.charge==0)) {
                            genLeptons.push_back(genLep);
                            passesWhichWGenCut = true;
                        }
                        if(whichW== -1 && (genLep.charge>0 || genLep.charge==0)) {
                            genLeptons.push_back(genLep);
                            passesWhichWGenCut = true;
                        }
                
                    }
                }
            }
            nGenLeptons = genLeptons.size();
            // sort leptons by descending pt
            sort(genLeptons.begin(), genLeptons.end(), LepDescendingOrder);

            //-- determine if the event passes the leptons requirements
            if (nGenLeptons >= 2){
                
                genLepton1 = genLeptons[0];
                genLepton2 = genLeptons[1];
                
                if (doW){
                    if (abs(genLeptons[0].charge) > 0 && genLeptons[1].charge == 0){
                        genLepton1 = genLeptons[0];
                        genLepton2 = genLeptons[1];
                        genMT = sqrt(2 *genLepton2.pt * genLepton1.pt * (1 - cos(genLepton2.phi - genLepton1.phi)));
                    }
                    else if (abs(genLeptons[1].charge) > 0 && genLeptons[0].charge == 0){
                        genLepton1 = genLeptons[1];
                        genLepton2 = genLeptons[0];
                        genMT = sqrt(2 *genLepton2.pt * genLepton1.pt * (1 - cos(genLepton2.phi - genLepton1.phi)));
                    }
                    else genMT = -99.0;
                    
                    if (genMT >= 0) passesGenLeptonReq = true;
                    if (passesWhichWGenCut && (genMT >= GenV_MassMin && genLepton2.pt >= METcut)) {
                      passesGenLeptonCut = 1;
                    }
                }
                
                //----- For Z+jets -------
                // select the first two leptons with opposite charge
                if (doZ && genLepton1.charge*genLepton2.charge > 0 && nGenLeptons > 2) {
                    genLepton2 = genLeptons[2];
                }
                // build the TLorentzVectors, the Z candidate and the kinematic
                genLep1.SetPtEtaPhiE(genLepton1.pt, genLepton1.eta, genLepton1.phi, genLepton1.energy);
                genLep2.SetPtEtaPhiE(genLepton2.pt, genLepton2.eta, genLepton2.phi, genLepton2.energy);
                genZ = genLep1 + genLep2;
                // apply charge, mass and eta cut
                if (doZ && genLepton1.charge*genLepton2.charge < 0 
                        && genZ.M() > ZMCutLow && genZ.M() < ZMCutHigh 
                        && genZ.Eta()*100 > ZEtaCutMin && genZ.Eta()*100 < ZEtaCutMax && genZ.Pt()>= ZPtCutMin) {
                    
                    passesGenLeptonCut = 1;
                }
                //----- End For Z+jets -------
            }
            
            /// --- if there are taus, but we do not run on the Tau file, thus we run on the WJets file,
            //    then we don't count the event at reco.
            //if (countTauS3 > 0 && fileName.find("Tau") == string::npos) passesLeptonCut = 0 ;

            //--- if there are taus we don't want the gen level
            if (countTauS3 > 0){
                passesGenLeptonCut = 0;
                passesGenLeptonReq = 0;
            }
        }
        //end has gen info
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //          Retrieving jets           //
        //====================================//
        bool passesJetCut(1), passesEWKJetPt(0), passesEWKJetFwdEta(0);
        unsigned short nGoodJets(0), nTotJets(0), nJetsAdd(0);
        double jetsHT(0), METscale(0.);
        
        double XMETscale(0.), YMETscale(0.);            // for calculating METscale
        double TempMETpt(0.), TempMETphi(0.), XMETpt(0.), YMETpt(0.) ; // for calculating METscale
        
        vector<jetStruct> jets, jetsAdditional;
        TLorentzVector leadJ, secondJ, jet1Plus2, jet1Minus2;
        
        //*************************************** begin edit *************************************************************//
        TLorentzVector newLeadJ, newSecondJ, newThirdJ, newFourthJ, newFifthJ;
        double ForwardJetRapidity(0), BackwardJetRapidity(0);
        vector<TLorentzVector> vJetYOrdered;
        //**************************************** end edit **************************************************************//
        
        int countBJets = 0;
        int countWbBjets = 0; // Wb study
        
        if (hasRecoInfo) {
            int countNJetsVSBeta[10] = {0};
            nTotJets = patJetPfAk05Eta_->size();
            
            //--- loop over all the jets ----------
            for (unsigned short i(0); i < nTotJets; i++) {
                double jetPtTemp(0.); // for calculating METscale
                bool passBJets(0);
                if (patJetPfAk05OCSV_->at(i) >= 0.679) passBJets = true;
                
                //************************* B-tag Veto Correcction *******************************//
                float this_rand = RandGen->Rndm(); // Get a random number.
                float pt= patJetPfAk05Pt_->at(i);
                float eta= patJetPfAk05Eta_->at(i);
                float x = 0.679;     ///discrim_cut;
                // --------- MC-only
                if (isData == false){
                    
                    bool passBJets_SFB_sys_up = passBJets;     // Initialize the systematic_up as the central value
                    bool passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value
                    
                    //int jetflavour= patJetPfAk05PartonFlavour_->at(i);
                    int jetflavour = int(patJetPfAk05PartonFlavour_->at(i));
                    
                    if (abs(jetflavour)==5){
                        float effb = -1.73338329789*x*x*x*x +  1.26161794785*x*x*x +  0.784721653518*x*x +  -1.03328577451*x +  1.04305075822;
                        
                        float SFb = (0.938887+(0.00017124*pt))+(-2.76366e-07*(pt*pt));
                        if (pt < 20.) SFb = (0.938887+(0.00017124*20.))+(-2.76366e-07*(20.*20.));
                        if (pt > 800.) SFb = (0.938887+(0.00017124*800.))+(-2.76366e-07*(800.*800.));
                        
                        float SFb_error = 0.0;
                        if (pt < 20.)                SFb_error = 0.0415707*2.;
                        if (pt >= 20. && pt < 30.)   SFb_error = 0.0415707;
                        if (pt >= 30. && pt < 40.)   SFb_error = 0.0204209;
                        if (pt >= 40. && pt < 50.)   SFb_error = 0.0223227;
                        if (pt >= 50. && pt < 60.)   SFb_error = 0.0206655;
                        if (pt >= 60. && pt < 70.)   SFb_error = 0.0199325;
                        if (pt >= 70. && pt < 80.)   SFb_error = 0.0174121;
                        if (pt >= 80. && pt < 100.)  SFb_error = 0.0202332;
                        if (pt >= 100. && pt < 120.) SFb_error = 0.0182446;
                        if (pt >= 120. && pt < 160.) SFb_error = 0.0159777;
                        if (pt >= 160. && pt < 210.) SFb_error = 0.0218531;
                        if (pt >= 210. && pt < 260.) SFb_error = 0.0204688;
                        if (pt >= 260. && pt < 320.) SFb_error = 0.0265191;
                        if (pt >= 320. && pt < 400.) SFb_error = 0.0313175;
                        if (pt >= 400. && pt < 500.) SFb_error = 0.0415417;
                        if (pt >= 500. && pt < 600.) SFb_error = 0.0740446;
                        if (pt >= 600. && pt < 800.) SFb_error = 0.0596716;
                        if (pt >= 800.)              SFb_error = 0.0596716*2.;
                        
                        float SFb_up = SFb + SFb_error;
                        float SFb_down = SFb - SFb_error;
                        
                        // F values for rand comparison
                        float f = 0.0;
                        float f_up = 0.0;
                        float f_down = 0.0;
                        
                        if (SFb <1.0) f = (1.0 - SFb);
                        if (SFb_up <1.0) f_up = (1.0 - SFb_up);
                        if (SFb_down <1.0) f_down = (1.0 - SFb_down);
                        
                        if (SFb > 1.0) f = (1.0 - SFb)/(1.0 - 1.0/effb);
                        if (SFb_up > 1.0) f_up = (1.0 - SFb_up)/(1.0 - 1.0/effb);
                        if (SFb_down > 1.0) f_down = (1.0 - SFb_down)/(1.0 - 1.0/effb);
                        
                        passBJets_SFB_sys_up = passBJets;     // Initialize the systematic_up as the central value
                        passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value
                        
                        // Untag a tagged jet
                        if ((passBJets==true) && (SFb<1.0) && (this_rand < f)) passBJets = false; // for central value
                        if ((passBJets_SFB_sys_up==true)   && (SFb_up<1.0) && (this_rand < f_up))   passBJets_SFB_sys_up = false; // for systematic_up
                        if ((passBJets_SFB_sys_down==true) && (SFb_down<1.0) && (this_rand < f_down)) passBJets_SFB_sys_down = false; // for sytematic_down
                        
                        
                        // Tag an untagged jet
                        if ((passBJets==false) && (SFb>1.0) && (this_rand < f)) passBJets = true; // for central value
                        if ((passBJets_SFB_sys_up==false)   && (SFb_up>1.0) && (this_rand < f_up))   passBJets_SFB_sys_up = true; // for systematic_up
                        if ((passBJets_SFB_sys_down==false) && (SFb_down>1.0) && (this_rand < f_down)) passBJets_SFB_sys_down = true; // for sytematic_down
                        
                    }
                    // ---------------- For Real C-jets--------------- //
                    if (abs(jetflavour)==4){
                        float effc = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724;
                        
                        float SFc = (0.938887+(0.00017124*pt))+(-2.76366e-07*(pt*pt));
                        if (pt < 20.) SFc = (0.938887+(0.00017124*20.))+(-2.76366e-07*(20.*20.));
                        if (pt > 800.) SFc = (0.938887+(0.00017124*800.))+(-2.76366e-07*(800.*800.));
                        
                        float SFc_error = 0.0;
                        if (pt < 20.)                SFc_error = 0.0415707*4.;
                        if (pt >= 20. && pt < 30.)   SFc_error = 0.0415707*2.;
                        if (pt >= 30. && pt < 40.)   SFc_error = 0.0204209*2.;
                        if (pt >= 40. && pt < 50.)   SFc_error = 0.0223227*2.;
                        if (pt >= 50. && pt < 60.)   SFc_error = 0.0206655*2.;
                        if (pt >= 60. && pt < 70.)   SFc_error = 0.0199325*2.;
                        if (pt >= 70. && pt < 80.)   SFc_error = 0.0174121*2.;
                        if (pt >= 80. && pt < 100.)  SFc_error = 0.0202332*2.;
                        if (pt >= 100. && pt < 120.) SFc_error = 0.0182446*2.;
                        if (pt >= 120. && pt < 160.) SFc_error = 0.0159777*2.;
                        if (pt >= 160. && pt < 210.) SFc_error = 0.0218531*2.;
                        if (pt >= 210. && pt < 260.) SFc_error = 0.0204688*2.;
                        if (pt >= 260. && pt < 320.) SFc_error = 0.0265191*2.;
                        if (pt >= 320. && pt < 400.) SFc_error = 0.0313175*2.;
                        if (pt >= 400. && pt < 500.) SFc_error = 0.0415417*2.;
                        if (pt >= 500. && pt < 600.) SFc_error = 0.0740446*2.;
                        if (pt >= 600. && pt < 800.) SFc_error = 0.0596716*2.;
                        if (pt >= 800.)              SFc_error = 0.0596716*4.;
                        
                        float SFc_up = SFc + SFc_error;
                        float SFc_down = SFc - SFc_error;
                        
                        // F values for rand comparison
                        float f = 0.0;
                        float f_up = 0.0;
                        float f_down = 0.0;
                        
                        if (SFc <1.0) f = (1.0 - SFc);
                        if (SFc_up <1.0) f_up = (1.0 - SFc_up);
                        if (SFc_down <1.0) f_down = (1.0 - SFc_down);
                        
                        if (SFc > 1.0) f = (1.0 - SFc)/(1.0 - 1.0/effc);
                        if (SFc_up > 1.0) f_up = (1.0 - SFc_up)/(1.0 - 1.0/effc);
                        if (SFc_down > 1.0) f_down = (1.0 - SFc_down)/(1.0 - 1.0/effc);
                        
                        passBJets_SFB_sys_up = passBJets;     // Initialize the systematic_up as the central value
                        passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value
                        
                        // Untag a tagged jet
                        if ((passBJets==true) && (SFc<1.0) && (this_rand < f)) passBJets = false; // for central value
                        if ((passBJets_SFB_sys_up==true)   && (SFc_up<1.0) && (this_rand < f_up))   passBJets_SFB_sys_up = false; // for systematic_up
                        if ((passBJets_SFB_sys_down==true) && (SFc_down<1.0) && (this_rand < f_down)) passBJets_SFB_sys_down = false; // for sytematic_down
                        
                        // Tag an untagged jet
                        if ((passBJets==false) && (SFc>1.0) && (this_rand < f)) passBJets = true; // for central value
                        if ((passBJets_SFB_sys_up==false)   && (SFc_up>1.0) && (this_rand < f_up))   passBJets_SFB_sys_up = true; // for systematic_up
                        if ((passBJets_SFB_sys_down==false) && (SFc_down>1.0) && (this_rand < f_down)) passBJets_SFB_sys_down = true; // for sytematic_down
                        
                    }
                    // ---------------- For REAL Light-jets --------------- //
                    if (abs(jetflavour)<4){
                        float SFlight=1.0;
                        float SFlight_up=1.0;
                        float SFlight_down=1.0;
                        float eff_l = 0.0;
                        float pt_temp(0.0), xpt(0.0);
                        
                        xpt = pt;
                        if (pt < 20. ) xpt = 20.;
                        if (pt > 670.) xpt = 670.;
                        
                        if ((fabs(eta)>=0.0) && (fabs(eta)<=0.8)){
                            eff_l = ((0.00967751+(2.54564e-05*xpt))+(-6.92256e-10*(xpt*xpt)));
                            
                            pt_temp = pt;
                            if (pt < 20.) pt = 20.;
                            if (pt > 1000.) pt = 1000.;
                            
                            SFlight = (((1.07541+(0.00231827*pt))+(-4.74249e-06*(pt*pt)))+(2.70862e-09*(pt*(pt*pt))));
                            SFlight_up = (((1.18638+(0.00314148*pt))+(-6.68993e-06*(pt*pt)))+(3.89288e-09*(pt*(pt*pt))));
                            SFlight_down = (((0.964527+(0.00149055*pt))+(-2.78338e-06*(pt*pt)))+(1.51771e-09*(pt*(pt*pt))));
                            
                            if (pt_temp < 20.|| pt_temp > 1000.) {
                                SFlight_up =  SFlight + 2.0*fabs(SFlight_up - SFlight);
                                SFlight_down = SFlight - 2.0*fabs(SFlight - SFlight_down);
                            }
                            pt = pt_temp;
                        }
                        if ((fabs(eta)>0.8) && (fabs(eta)<=1.6)){
                            eff_l = ((0.00974141+(5.09503e-05*xpt))+(2.0641e-08*(xpt*xpt)));
                            
                            pt_temp = pt;
                            if (pt < 20.) pt = 20.;
                            if (pt > 1000.) pt = 1000.;
                            
                            SFlight = (((1.05613+(0.00114031*pt))+(-2.56066e-06*(pt*pt)))+(1.67792e-09*(pt*(pt*pt))));
                            SFlight_up = (((1.16624+(0.00151884*pt))+(-3.59041e-06*(pt*pt)))+(2.38681e-09*(pt*(pt*pt))));
                            SFlight_down = (((0.946051+(0.000759584*pt))+(-1.52491e-06*(pt*pt)))+(9.65822e-10*(pt*(pt*pt))));
                            
                            if (pt_temp < 20.|| pt_temp > 1000.) {
                                SFlight_up =  SFlight + 2.0*fabs(SFlight_up - SFlight);
                                SFlight_down = SFlight - 2.0*fabs(SFlight - SFlight_down);
                            }
                            pt = pt_temp;
                        }
                        if ((fabs(eta)>1.6) && (fabs(eta)<=2.4)){
                            eff_l = ((0.013595+(0.000104538*xpt))+(-1.36087e-08*(xpt*xpt)));
                            
                            pt_temp = pt;
                            if (pt < 20.) pt = 20.;
                            if (pt > 850.) pt = 850.;
                            
                            SFlight = (((1.05625+(0.000487231*pt))+(-2.22792e-06*(pt*pt)))+(1.70262e-09*(pt*(pt*pt))));
                            SFlight_up = (((1.15575+(0.000693344*pt))+(-3.02661e-06*(pt*pt)))+(2.39752e-09*(pt*(pt*pt))));
                            SFlight_down = (((0.956736+(0.000280197*pt))+(-1.42739e-06*(pt*pt)))+(1.0085e-09*(pt*(pt*pt))));
                            
                            if (pt_temp < 20.|| pt_temp > 850.) {
                                SFlight_up =  SFlight + 2.0*fabs(SFlight_up - SFlight);
                                SFlight_down = SFlight - 2.0*fabs(SFlight - SFlight_down);
                            }
                            pt = pt_temp;
                        }
                        if (fabs(eta)>2.4){
                            eff_l = ((0.013595+(0.000104538*xpt))+(-1.36087e-08*(xpt*xpt)));
                        }
                        
                        // F values for rand comparison
                        float f = 0.0;
                        float f_up = 0.0;
                        float f_down = 0.0;
                        
                        if (SFlight <1.0) f = (1.0 - SFlight);
                        if (SFlight_up <1.0) f_up = (1.0 - SFlight_up);
                        if (SFlight_down <1.0) f_down = (1.0 - SFlight_down);
                        
                        if (SFlight > 1.0) f = (1.0 - SFlight)/(1.0 - 1.0/eff_l);
                        if (SFlight_up > 1.0) f_up = (1.0 - SFlight_up)/(1.0 - 1.0/eff_l);
                        if (SFlight_down > 1.0) f_down = (1.0 - SFlight_down)/(1.0 - 1.0/eff_l);
                        
                        passBJets_SFB_sys_up = passBJets;     // Initialize the systematic_up as the central value
                        passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value
                        
                        // Untag a tagged jet
                        if ((passBJets==true) && (SFlight<1.0) && (this_rand < f)) passBJets = false; // for central value
                        if ((passBJets_SFB_sys_up==true)   && (SFlight_up<1.0) && (this_rand < f_up))   passBJets_SFB_sys_up = false; // for systematic_up
                        if ((passBJets_SFB_sys_down==true) && (SFlight_down<1.0) && (this_rand < f_down)) passBJets_SFB_sys_down = false; // for sytematic_down
                        
                        // Tag an untagged jet
                        if ((passBJets==false) && (SFlight>1.0) && (this_rand < f)) passBJets = true; // for central value
                        if ((passBJets_SFB_sys_up==false)   && (SFlight_up>1.0) && (this_rand < f_up))   passBJets_SFB_sys_up = true; // for systematic_up
                        if ((passBJets_SFB_sys_down==false) && (SFlight_down>1.0) && (this_rand < f_down)) passBJets_SFB_sys_down = true; // for sytematic_down
                    }   ////////flavour lop
                    
                    if (sysBtagSF ==  1) passBJets = passBJets_SFB_sys_up;
                    if (sysBtagSF == -1) passBJets = passBJets_SFB_sys_down;
                    
                    // Wb study
                    if (abs(jetflavour)==5) countWbBjets++ ;
                }
                // --------- End MC-only
                //************************* End B-tag Veto Correcction *******************************//                
                 
                jetStruct jet = {patJetPfAk05Pt_->at(i), patJetPfAk05Eta_->at(i), patJetPfAk05Phi_->at(i), patJetPfAk05En_->at(i), i, passBJets};

                //-- apply jet energy scale uncertainty (need to change the scale when initiating the object)
                double jetEnergyCorr = 0.; 
                bool jetPassesPtCut(jet.pt >= 10); // for MET uncertainty should the cut be before or aftes adding unc.?????
                jetEnergyCorr = TableJESunc.getEfficiency(jet.pt, jet.eta);
                
                jetPtTemp = jet.pt; // for calculating METscale
                jet.pt *= (1 + scale * jetEnergyCorr);
                jet.energy *= (1 + scale * jetEnergyCorr);

                bool jetPassesEtaCut((jet.eta >= RecJetEtaMin) && (jet.eta <= RecJetEtaMax)); 
                bool jetPassesIdCut(patJetPfAk05LooseId_->at(i) > 0);
                bool jetPassesBetaCut(patJetPfAk05jetBZ_->at(i) > 0.1 * doPUStudy);
                bool jetPassesBetaStarCut(patJetPfAk05jetBSZ_->at(i) < 1);
                double tempMVA = patJetPfAk05jetpuMVA_->at(i);
                bool jetPassesMVACut(0);
                if (energy == "7TeV") {
                    jetPassesMVACut = ((tempMVA > -0.9 && jet.pt <= 20) || (jet.pt > 20 && (
                                    (tempMVA > -0.4  && fabs(jet.eta) <= 2.5) ||
                                    (tempMVA > -0.85 && fabs(jet.eta) > 2.5  && fabs(jet.eta) <= 2.75) ||
                                    (tempMVA > -0.7  && fabs(jet.eta) > 2.75 && fabs(jet.eta) <= 3.) ||
                                    (tempMVA > -0.6  && fabs(jet.eta) > 3.   && fabs(jet.eta) <= 5.))));  
                }
                if (energy == "8TeV") {
                    jetPassesMVACut = (
                            (tempMVA > -0.89 && fabs(jet.eta) <= 2.5) || 
                            (tempMVA > -0.77 && fabs(jet.eta) > 2.5  && fabs(jet.eta) <= 2.75) ||
                            (tempMVA > -0.69 && fabs(jet.eta) > 2.75 && fabs(jet.eta) <= 3.) ||
                            (tempMVA > -0.75 && fabs(jet.eta) > 3.   && fabs(jet.eta) <= 5.)) ;  
                    //  new training does not work for 22Jan rereco, we use simple loose PU ID : -1  - does not pass, 1 passes
                    if (tempMVA > 0) jetPassesMVACut = true ;
                    else jetPassesMVACut = false ;
                }
                //bool jetPassesMVACut(patJetPfAk05jetpuMVA_->at(i) >= - 0.4); // -0.4 set for the cut was for 44x training. for 53x chs loose jet id is set to -0.89? 
                //bool jetPassesMVACut(patJetPfAk05jetpuMVA_->at(i) >= - 0.0 && patJetPfAk05jetBZ_->at(i) > 0.05 ); // 0 set for the cut was for 44x training. for 53x chs loose jet id is set to -0.89? 

                bool jetPassesdRCut(1);
                double dPtJetMuon(-100.);
                unsigned short nRemovedLep = min(int(nLeptons), doW ? 1:2);
                
                //if (jentry % 100000 == 0) cout << "--- nLeptons: " << nLeptons << "  nRemovedLep: " << nRemovedLep << " ---" << endl;
                
                for (unsigned short j(0); j < nRemovedLep; j++) {
                    // determine if passes dRCut
                    if (deltaR(jet.phi, jet.eta, selLeptons[j].phi, selLeptons[j].eta) < 0.5) {
                        if (doDR) jetPassesdRCut = 0;
                        dPtJetMuon = jet.pt-selLeptons[j].pt;
                        if (jet.pt >= RecJetPtMin && jetPassesMVACut && passesLeptonCut && jetPassesEtaCut && jetPassesIdCut) { 
                            ZMass_lowDeltaR->Fill(Z.M(), weight);
                            deltaPtjetMu->Fill(dPtJetMuon,weight);
                        }
                    }
                    
                    if (jet.pt >= RecJetPtMin && jetPassesMVACut && passesLeptonCut && jetPassesEtaCut && jetPassesIdCut){
                        deltaRjetMu->Fill(deltaR(jet.phi, jet.eta, selLeptons[j].phi, selLeptons[j].eta), weight);
                    }
                }
                
                // for MET scale
                if (fabs(scale) > 0. && jetPassesPtCut && jetPassesMVACut && jetPassesIdCut){
                    
                    //METscale -= scale * jetEnergyCorr * jetPtTemp ;
                    //-------- my calculation --------
                    XMETscale += ( scale * jetEnergyCorr * jetPtTemp * cos(jet.phi) ) ;
                    YMETscale += ( scale * jetEnergyCorr * jetPtTemp * sin(jet.phi) ) ;
                    //-------- end my calculation --------
                    
                }

                
                if ( jetPassesEtaCut && jetPassesIdCut && jetPassesdRCut) {
                    if (jetPassesPtCut){
                        if (jet.pt >= RecJetPtMin && passesLeptonCut){
                            Beta->Fill(patJetPfAk05jetBZ_->at(i), weight);
                            BetaStar->Fill(patJetPfAk05jetBSZ_->at(i), weight);
                            puMVA->Fill(patJetPfAk05jetpuMVA_->at(i), weight);
                            puMVAvsBeta->Fill(patJetPfAk05jetpuMVA_->at(i),patJetPfAk05jetBZ_->at(i), weight);
                        }
                        
                        //if ( fabs(doBJets) > 0 && patJetPfAk05OCSV_->at(i) >=  0.679 )  countBJets++ ;// count BJets, used for BVeto
                        if ( fabs(doBJets) > 0 && passBJets == true) countBJets++ ;// count BJets, used for BVeto
                        
                        // We apply only the pu MVA variable for the time being.
                        // This is the recommended one.
                        // if (jetPassesBetaCut && jetPassesBetaStarCut && jetPassesMVACut) 
                        if ( jet.pt >= 50 ) passesEWKJetPt = true ;
                        if ( fabs(jet.eta) > 2.4 ) passesEWKJetFwdEta = true ;
                        //if (doZ)	
                        if (energy == "8TeV" && doPUStudy < 0  && jetPassesMVACut) jets.push_back(jet);
                        //if (energy == "8TeV" && doPUStudy >= 0 && jetPassesBetaCut && jetPassesBetaStarCut) jets.push_back(jet);
                        //if (energy == "7TeV" && jetPassesBetaCut && jetPassesBetaStarCut) jets.push_back(jet);
                        for ( int k = 0 ; k < 10 ; k++){
                            if ( patJetPfAk05jetBZ_->at(i) >= 0.1 * k )  countNJetsVSBeta[k]++;
                        }
                        //
                        //else jets.push_back(jet);
                    }
                    if (jet.pt >=  15.){
                        jetsAdditional.push_back(jet);	
                    }
                }
            }
            //--- End of loop over all the jets ---

            for ( int k = 0 ; k < 10 ; k++){
                ZNGoodJetsBeta_Zexc->Fill(countNJetsVSBeta[k], k  , weight);
            }

            nGoodJets = jets.size();
            nJetsAdd = jetsAdditional.size();
            
            
//            // line below to test reco events that originate from TAU
//            if (fileName.find("Tau") != string::npos && countTauS3 == 0 && hasGenInfo ){
//                passesLeptonCut = 0;
//            }
//            
//            if (doBJets < 0 && countBJets >= fabs(doBJets) )  { nEventsIncBJets++; 
//                passesLeptonCut = 0  ; }
//
//            if (doBJets > 0 && doBJets < 99 && countBJets < fabs(doBJets) ) passesLeptonCut = 0  ;
//            if (doBJets == 101 && countBJets != 1) passesLeptonCut = 0  ;
            
        }  // END IF HAS RECO
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //        Print Event Information     //
        //====================================//
        if (PRINTEVENT && passesLeptonCut){
            vector<jetStruct> tmpJets;
            for (unsigned short i(0); i < nGoodJets; i++){
                if (jets[i].pt >= RecJetPtMin) tmpJets.push_back(jets[i]);
            }
            unsigned short tempnGoodJets(tmpJets.size());
            NZtotal++;
            cout << "event info: " << EvtInfo_RunNum << "  " << EvtInfo_EventNum << endl;
            cout << "Z event #" << NZtotal << "  Zmass : " << Z.M() << "  Zpt : " << Z.Pt() << " NJets : " << tempnGoodJets <<"    " <<weight << endl;
            if (nGoodJets > 0) cout << "JETS:"<< endl;
            for (unsigned short i(0); i < tempnGoodJets; i++) 
                cout << " jet #" << i + 1 << "  pt: " << tmpJets[i].pt << "  eta:"<<tmpJets[i].eta << "   " << endl;
            cout << "-----------------------------------------------------------------------------------------"<< endl;
        }
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //        Retrieving gen jets         //
        //====================================//
        bool passesGenJetCut(1), passesGenEWKJetPt(0), passesGenEWKJetFwdEta(0);
        unsigned short nGoodGenJets(0), nGenJetsAdd(0), nTotGenJets(0);
        double genJetsHT(0);
        vector<jetStruct> genJets, genJetsAdditional;
        TLorentzVector genLeadJ, genSecondJ, genJet1Plus2, genJet1Minus2;
        
        //*************************************** begin edit *************************************************************//
        TLorentzVector genNewLeadJ, genNewSecondJ, genNewThirdJ, genNewFourthJ, genNewFifthJ;
        double genForwardJetRapidity(0), genBackwardJetRapidity(0);
        vector<TLorentzVector> genvJetYOrdered;
        //**************************************** end edit **************************************************************//

        if (hasGenInfo){
            nTotGenJets = genJetEta_->size();
            //-- retrieving generated jets
            for (unsigned short i(0); i < nTotGenJets; i++){
                jetStruct genJet = {genJetPt_->at(i), genJetEta_->at(i), genJetPhi_->at(i), genJetE_->at(i), i, 0};
                bool genJetPassesdRCut(1);
                double dRmin = 9999.;
                for (unsigned short j(0); j < nGenLeptons; j++){ 
                    if (genJet.pt >=  GenJetPtMin){
                        gendeltaRjetMu->Fill(deltaR(genJet.phi, genJet.eta, genLeptons[j].phi, genLeptons[j].eta), genWeight);
                    }
                    if ( deltaR(genJet.phi, genJet.eta, genLeptons[j].phi, genLeptons[j].eta) < dRmin ) dRmin = deltaR(genJet.phi, genJet.eta, genLeptons[j].phi, genLeptons[j].eta);
                    // I need this line because for to me unknown reason I CAN NO REMOVE ELECTRONS FROM Z IN SHERPA !!!!
                    if ((genLeptons[j].charge != 0)
                        && (doDR || (leptonFlavor == "Electrons" && fileName.find("HepMC") != string::npos))
                        && (deltaR(genJet.phi, genJet.eta, genLeptons[j].phi, genLeptons[j].eta) < 0.5)){
                        genJetPassesdRCut = 0;
                    }
                }
                //if (genJet.pt >= 10 && genJet.pt < 1000. && fabs(genJet.eta) <= 4.7 && genJetPassesdRCut)
                if (genJetPassesdRCut && genJet.pt >= 10 && fabs(genJet.eta) <= 4.7){
                    genJets.push_back(genJet);
                    
                    passesGenEWKJetPt = (genJet.pt >= 50);
                    passesGenEWKJetFwdEta = (fabs(genJet.eta) > 2.4);
                    if (genJet.pt >=  15.) genJetsAdditional.push_back(genJet);
                }
            }
            nGoodGenJets = genJets.size();
            nGenJetsAdd = genJetsAdditional.size();
        }
        //=======================================================================================================//



        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //     Matching gen and reco jets     //
        //====================================//
        double XMetSmear(0), YMetSmear(0);
        vector<int> genJetsIndex(nGoodGenJets, 0);
        vector<vector<int> > matchingTable(nGoodJets, genJetsIndex);
        if (hasRecoInfo && hasGenInfo){
            for (unsigned short i(0); i < nGoodJets; i++){
                double mindR(0.5);
                int index(-1);
                double dR(9999);
                for (unsigned short j(0); j < nGoodGenJets; j++){
                    dR = deltaR(genJets[j].phi, genJets[j].eta, jets[i].phi, jets[i].eta);
                    if (dR < mindR){
                        mindR = dR;
                        index = j;
                    }
                }
                if (index > -1 ){
                    matchingTable[i][index] = 1;
                    //cout << " " << jentry << " " << i <<  " " << index << "  " << jets[i].pt << "  " << genJets[index].pt;
                    //cout << " " << jentry << " " << i <<  " " << index << "  " << jets[i].pt << "  " << jets[i].energy ;
                    //jets[i].pt = SmearJetPt(jets[i].pt, genJets[index].pt, jets[i].eta, smearJet);
                    double oldJetPt = jets[i].pt;
                    double newJetPt = SmearJetPt(oldJetPt, genJets[index].pt, jets[i].eta, smearJet);
                    jets[i].pt = newJetPt;
                    jets[i].energy = jets[i].energy * (newJetPt / oldJetPt);
                    //cout << "  new : " << jets[i].pt << "  " << jets[i].energy << endl;
                    
                    // for recalculating MET
                    XMetSmear += (newJetPt - oldJetPt) * cos(jets[i].phi);
                    YMetSmear += (newJetPt - oldJetPt) * sin(jets[i].phi);

                    puMVA_JetsMatchGenJets->Fill(patJetPfAk05jetpuMVA_->at(jets[i].patIndex), weight);
                    puBeta_JetsMatchGenJets->Fill(patJetPfAk05jetBZ_->at(jets[i].patIndex), weight);
                    puBetaStar_JetsMatchGenJets->Fill(patJetPfAk05jetBSZ_->at(jets[i].patIndex), weight);
                    jetsEta_JetsMatchGenJets->Fill(patJetPfAk05Eta_->at(jets[i].patIndex), weight);
                }
                else {
                    puMVA_JetsNoMatchGenJets->Fill(patJetPfAk05jetpuMVA_->at(jets[i].patIndex), weight);
                    puBeta_JetsNoMatchGenJets->Fill(patJetPfAk05jetBZ_->at(jets[i].patIndex), weight);
                    puBetaStar_JetsNoMatchGenJets->Fill(patJetPfAk05jetBSZ_->at(jets[i].patIndex), weight);
                    jetsEta_JetsNoMatchGenJets->Fill(patJetPfAk05Eta_->at(jets[i].patIndex), weight);
                }
            }

            //-- print the mathcing table
            //cout << "\n mathcing Table: \n" << endl; 
            //for (int i = 0; i < int(matchingTable.size()); i++)
            //  for (int j = 0; j < int(matchingTable[i].size()); j++)
            //    cout << matchingTable[i][j] << "  ";
            //  
            //  cout << endl;
            //
        }
        //end of has reco info and has gen info 
        //=======================================================================================================//
        //          Retrieving MET             //
        //====================================//
        if (hasRecoInfo){
            
            if (doW && !(patMetPt_->size() > 0)) continue;
            METphi = patMetPhi_->at(whichMet);
            METpt = patMetPt_->at(whichMet);
            
            //cout << " jentry: " << jentry << " nMuons: " << nMuons << " nLeptons: " << nLeptons << " nElectrons: " << nElectrons <<  endl;
            if (doW && ((leptonFlavor == "SingleMuon" && nMuons == 1 && nLeptons == 1 && nElectrons == 0) || (leptonFlavor == "SingleElectron" && nMuons == 0 && nLeptons == 1 && nElectrons == 1))) {
                
                passesLeptonReq = true;
                
                // recalculate METpt and METphi
                if ( (fabs(scale) > 0.) || (hasRecoInfo && hasGenInfo) ){
                    //-------- my calculation --------
                    TempMETphi = patMetPhi_->at(whichMet);
                    TempMETpt = patMetPt_->at(whichMet);
                    XMETpt = (TempMETpt * cos(TempMETphi));
                    YMETpt = (TempMETpt * sin(TempMETphi));
                    
                    // if do JES systematic variations (Data only), we recalculate MET
                    if (fabs(scale) > 0.){
                        XMETpt -= XMETscale;
                        YMETpt -= YMETscale;
                    }
                    
                    // Recalculate MET because Smearing jets
                    if (hasRecoInfo && hasGenInfo){
                        XMETpt -= XMetSmear;
                        YMETpt -= YMetSmear;
                    }
                    
                    TVector2 METvec;
                    METvec.Set(XMETpt, YMETpt);
                    
                    // assign new values for METpt and METphi
                    METpt  = METvec.Mod();
                    METphi = METvec.Phi_mpi_pi(METvec.Phi());
                    //-------- end my calculation --------
                }
                
                //lepton1 = leptons[0];
                //cout << "how many leptons \t" << leptons.size() << endl;
                if(leptons.size()>=2) {
                    cout << "DD : " << leptons[0].pt << "\t" << leptons[0].eta << "\t" << leptons[0].phi << "\t" << leptons[0]. energy << "\t"
                         << leptons[0].charge << "\t" << leptons[0].iso << "\t" << leptons[0].scEta << endl;
                }
                if(whichW==0) {
                  lepton1 = leptons[0];
                  passesWhichWCut = true;
                }
                if(whichW==1 && leptons[0].charge<0) {
                  lepton1 = leptons[0];
                  passesWhichWCut = true;
                }
                if(whichW==-1 && leptons[0].charge>0){
                  lepton1 = leptons[0];
                  passesWhichWCut = true;
                }
                //
                leptonStruct tempMet = {METpt, 0., METphi, METpt, 0, 0, 0};
                lepton2 = tempMet;
                
                MT = sqrt(2 * METpt * lepton1.pt * (1 - cos(METphi - lepton1.phi)));

                // build the TLorentzVectors, the Z candidate and the kinematic
                lep1.SetPtEtaPhiM(lepton1.pt, lepton1.eta, lepton1.phi, leptonMass);
                lep2.SetPtEtaPhiM(METpt, 0, METphi, 0);
                Z = lep1 + lep2;
                
                // correct for identification and isolation efficiencies if required by useEfficiencyCorrection
                // apply scale factors only on MC
                if (useEfficiencyCorrection) {
                    double effWeight = 1.;
                    if (leptonFlavor == "SingleMuon") {
                        //effWeight = LeptID.getEfficiency(lepton1.pt, fabs(lepton1.eta), sysLepSF);
                        effWeight = LeptID.getEfficiency(lepton1.pt, lepton1.eta, sysLepSF);
                        //effWeight *= LeptIso.getEfficiency(lepton1.pt, fabs(lepton1.eta), sysLepSF);
                        if (useTriggerCorrection) {
                            //effWeight *= LeptTrig.getEfficiency(fabs(lepton1.pt), fabs(lepton1.eta), sysLepSF);
                            effWeight *= LeptTrig.getEfficiency(fabs(lepton1.pt), lepton1.eta, sysLepSF);
                        }
                    }
                    else if (leptonFlavor == "SingleElectron") {
                        effWeight *= LeptID.getEfficiency(lepton1.pt, fabs(lepton1.eta), sysLepSF);
                        effWeight *= Ele_Rec.getEfficiency(lepton1.pt, fabs(lepton1.scEta), sysLepSF);
                        
                    }
                    //effWeight = 1;
                    if (isData) weight /= effWeight;
                    else weight *= effWeight;
                }
                
                ///--- 2D histograms for ABCD method to extract the QCD abckround ?
                fullMET_pfMETPFlow->Fill(patMetPt_->at(0), weight);
                fullMET_pfMet->Fill(patMetPt_->at(1), weight);
                fullMET_pfType1CorrectedMet->Fill(patMetPt_->at(2), weight);
                fullMET_pfType1p2CorrectedMet->Fill(patMetPt_->at(3), weight);
                ///---
                
                // apply transverse mass and MET cut
                if ((passesWhichWCut) && (METpt >= METcut && (((doQCD % 2) == 0 && MT >= RecV_MassMin) || ((doQCD % 2) == 1 && MT < RecV_MassMin)))) {
                    passesLeptonCut = true;
                    passesLeptonAndMT = true;
                    nEventsWithTwoGoodLeptons++;
                }
            }
        } // END IF RECO FOR MET
        
        if (passesGenLeptonCut) {
            TotalGenWeightPassGEN += genWeightBackup;
            TotalGenWeightPassGENPU += weight;
            partonsNAfterGenCut->Fill(nup_ - 5);
            partonsNAfterGenCutWeighted->Fill(nup_ - 5, genWeight);
        }
        //=======================================================================================================//
        
        // Re-analyze the jets collections and Cut on the Pt
        // we can do it only now since we needed to smear
        // the jet pt distribution for the MC
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        if (hasRecoInfo){     
            vector<jetStruct> tmpJets;
            for (unsigned short i(0); i < nGoodJets; i++){
                if (jets[i].pt >= RecJetPtMin) tmpJets.push_back(jets[i]);
            }
            jets.clear(); 
            jets = tmpJets; 
            tmpJets.clear(); 
            nGoodJets = jets.size();
            if (nGoodJets >= 1){
                sort(jets.begin(), jets.end(), JetDescendingOrder);
                sort(jetsAdditional.begin(), jetsAdditional.end(), JetDescendingOrder);
                leadJ.SetPtEtaPhiE(jets[0].pt, jets[0].eta, jets[0].phi, jets[0].energy);               
                //*************************************** begin edit *************************************************************//
                newLeadJ.SetPtEtaPhiE(jets[0].pt, jets[0].eta, jets[0].phi, jets[0].energy);
                vector<double> vJetRapidity;
                for (unsigned short i(0); i < nGoodJets; i++) {
                    TLorentzVector LVJet;
                    LVJet.SetPtEtaPhiE(jets[i].pt, jets[i].eta, jets[i].phi, jets[i].energy);
                    vJetRapidity.push_back(LVJet.Rapidity());
                    vJetYOrdered.push_back(LVJet);
                }
                ForwardJetRapidity = *max_element(vJetRapidity.begin(), vJetRapidity.end());
                BackwardJetRapidity = *min_element(vJetRapidity.begin(), vJetRapidity.end());
                sort(vJetYOrdered.begin(), vJetYOrdered.end(), JetYDescendingOrder);
                //**************************************** end edit ************************************************************//               
                if (RecJetPtMax > RecJetPtMin){
                    passesJetCut = jets[0].pt < RecJetPtMax;
                }
            }
            if (nGoodJets >= 2){
                secondJ.SetPtEtaPhiE(jets[1].pt, jets[1].eta, jets[1].phi, jets[1].energy);               
                //*************************************** begin edit *************************************************************//
                newSecondJ.SetPtEtaPhiE(jets[1].pt, jets[1].eta, jets[1].phi, jets[1].energy);
                //**************************************** end edit ************************************************************// 
		        jet1Plus2 = leadJ + secondJ;
                jet1Minus2 = leadJ - secondJ;
            }
            //*************************************** begin edit *************************************************************//
            if (nGoodJets >= 3){
                newThirdJ.SetPtEtaPhiE(jets[2].pt, jets[2].eta, jets[2].phi, jets[2].energy);
            }
            if (nGoodJets >= 4){
                newFourthJ.SetPtEtaPhiE(jets[3].pt, jets[3].eta, jets[3].phi, jets[3].energy);
            }
            if (nGoodJets >= 5){
                newFifthJ.SetPtEtaPhiE(jets[4].pt, jets[4].eta, jets[4].phi, jets[4].energy);
            }
            //**************************************** end edit ************************************************************//
	    jetsHT = 0;
            for (unsigned short i(0); i < nGoodJets; i++){
                jetsHT += jets[i].pt;  
            }
        }
        //end of has reco info

        if (hasGenInfo){
            vector< jetStruct> tmpJets;
            for (unsigned short i(0); i < nGoodGenJets; i++){
                if (genJets[i].pt >= GenJetPtMin && genJets[i].eta >= double (GenJetEtaMin) && genJets[i].eta <= (double )(GenJetEtaMax) ){
                    tmpJets.push_back(genJets[i]);
                }
            }
            genJets.clear();
            genJets = tmpJets; 
            tmpJets.clear(); 
            nGoodGenJets = genJets.size();
            if (nGoodGenJets >= 1){
                sort(genJets.begin(), genJets.end(), JetDescendingOrder);
                genLeadJ.SetPtEtaPhiE(genJets[0].pt, genJets[0].eta, genJets[0].phi, genJets[0].energy);
                //*************************************** begin edit ***********************************************************//
                genNewLeadJ.SetPtEtaPhiE(genJets[0].pt, genJets[0].eta, genJets[0].phi, genJets[0].energy);
                vector<double> vJetRapidity;
                for (unsigned short i(0); i < nGoodGenJets; i++) {
                    TLorentzVector LVJet;
                    LVJet.SetPtEtaPhiE(genJets[i].pt, genJets[i].eta, genJets[i].phi, genJets[i].energy);
                    vJetRapidity.push_back(LVJet.Rapidity());
                    genvJetYOrdered.push_back(LVJet);
                }
                genForwardJetRapidity = *max_element(vJetRapidity.begin(), vJetRapidity.end());
                genBackwardJetRapidity = *min_element(vJetRapidity.begin(), vJetRapidity.end());
                sort(genvJetYOrdered.begin(), genvJetYOrdered.end(), JetYDescendingOrder);
                //**************************************** end edit ************************************************************//
                if (RecJetPtMax > GenJetPtMin){
                    passesGenJetCut = genJets[0].pt < RecJetPtMax;
                }
            }
            if (nGoodGenJets >= 2){
                genSecondJ.SetPtEtaPhiE(genJets[1].pt, genJets[1].eta, genJets[1].phi, genJets[1].energy);
                //*************************************** begin edit *************************************************************//
                genNewSecondJ.SetPtEtaPhiE(genJets[1].pt, genJets[1].eta, genJets[1].phi, genJets[1].energy);
                //**************************************** end edit ************************************************************//
                genJet1Plus2 = genLeadJ + genSecondJ;
                genJet1Minus2 = genLeadJ - genSecondJ;
            }
            //*************************************** begin edit *************************************************************//
            if (nGoodGenJets >= 3){
                genNewThirdJ.SetPtEtaPhiE(genJets[2].pt, genJets[2].eta, genJets[2].phi, genJets[2].energy);
            }
            if (nGoodGenJets >= 4){
                genNewFourthJ.SetPtEtaPhiE(genJets[3].pt, genJets[3].eta, genJets[3].phi, genJets[3].energy);
            }
            if (nGoodGenJets >= 5){
                genNewFifthJ.SetPtEtaPhiE(genJets[4].pt, genJets[4].eta, genJets[4].phi, genJets[4].energy);
            }
            //**************************************** end edit ************************************************************//
            genJetsHT = 0.;
            for (unsigned short i(0); i < nGoodGenJets; i++){
                genJetsHT += genJets[i].pt;  
            }

        }
        //end of has gen info
        //=======================================================================================================//

        
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        // Select the best pair of jets for DPS  //
        //=======================================//
        pair<TLorentzVector, TLorentzVector> bestTwoJets;
        TLorentzVector bestJet1Plus2, bestJet1Minus2;
        if (hasRecoInfo){
            bestTwoJetsCandidatesPt(jets, bestTwoJets);
            //bestTwoJetsCandidatesPhi(jets, bestTwoJets);
            if (nGoodJets >= 2){
                bestJet1Plus2 = bestTwoJets.first + bestTwoJets.second;
                bestJet1Minus2 = bestTwoJets.first - bestTwoJets.second;
            }
        }

        pair<TLorentzVector, TLorentzVector> genBestTwoJets;
        TLorentzVector genBestJet1Plus2, genBestJet1Minus2;
        if (hasGenInfo){
            bestTwoJetsCandidatesPt(genJets, genBestTwoJets);
            //bestTwoJetsCandidatesPhi(genJets, genBestTwoJets);
            if (nGoodGenJets >= 2){
                genBestJet1Plus2 = genBestTwoJets.first + genBestTwoJets.second; 
                genBestJet1Minus2 = genBestTwoJets.first - genBestTwoJets.second; 
            }
        }
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //   Retrieving DPS partons and jets  //
        //====================================//
        bool passesDPSPartonCut(1); 
        unsigned short nTotDPSPartons(0);
        vector<pair<TLorentzVector, TLorentzVector> > genDPSAndItsJets;
        if (hasPartonInfo){
            nTotDPSPartons = dpsParton_Eta->size();
            short jetInd(0);
            for (unsigned short i(0); i < nTotDPSPartons; i++){
                TLorentzVector genDPSParton, genDPSJet;
                genDPSParton.SetPtEtaPhiE(dpsParton_Pt->at(i), dpsParton_Eta->at(i), dpsParton_Phi->at(i), dpsParton_E->at(i));
                jetInd = genMatchDPSpar->at(i); 
                if (jetInd >= 0){
                    genDPSJet.SetPtEtaPhiE(genJetPt_->at(jetInd), genJetEta_->at(jetInd), genJetPhi_->at(jetInd), genJetE_->at(jetInd));
                    pair<TLorentzVector, TLorentzVector> genDPSAndItsJet(genDPSParton, genDPSJet);
                    genDPSAndItsJets.push_back(genDPSAndItsJet);
                }
            }
            // remove DPS from inclusive sample
            if (fileName.find("PartonInfo") != string::npos && fileName.find("enriched") != string::npos &&
                    (nTotDPSPartons < 2 || fabs(dpsParton_Eta->at(0)) > 2.4 || fabs(dpsParton_Eta->at(1)) > 2.4)){
                passesDPSPartonCut = 0;
            }
        }
        //has parton info
        //=======================================================================================================//

        // Wb study
        if ( doWbsyst && countWbBjets == 1 && hasGenInfo) {
            weight = weight * WbSystSF;
            genWeight = genWeight * WbSystSF;
        }
        
        //======= Final Selections: =======
        if (hasRecoInfo){
            if (doBJets < 0 && countBJets >= fabs(doBJets)) {
                passesLeptonCut = 0;
                passesBtagReq = false;
                nEventsIncBJets++;
            }
            if (doBJets > 0 && doBJets < 99 && countBJets < fabs(doBJets) ){
                passesLeptonCut = 0;
                passesBtagReq = false;
            }
            if (doBJets == 101 && countBJets != 1){
                passesLeptonCut = 0;
                passesBtagReq = false;
            }
            
            // line below to test reco events that originate from TAU
            //if (fileName.find("Tau") != string::npos && countTauS3 == 0 && hasGenInfo )
            //    passesLeptonCut = 0;
            //
        }
        if (hasGenInfo){
            /// --- if there are taus, but we do not run on the Tau file, thus we run on the WJets file,
            //    then we don't count the event at reco.
            if (countTauS3 > 0 && fileName.find("Tau") == string::npos){
                passesLeptonCut = 0 ;
                passesTau3Req =false;
            }
        }
        //=================================

//        // keep the following code for reference
//        if (hasGenInfo){
//            if (countTauS3 == 0 && fileName.find("UNFOLDING") != string::npos){
//                partonsN->Fill(nup_-5);
//                partonsNWeighted->Fill(nup_-5, genWeight);
//            }
//            if ( ( fileName.find("Tau") == string::npos &&  countTauS3 > 0  ) || ( fileName.find("Tau") != string::npos &&  countTauS3 == 0) ){
//                ZMassAllPassLep->Fill(Z.M(),weight);
//                AllPassLepID->Fill(sumLepCharge,weight);
//                if (Z.M() > 50 )  AllPassWithMassCutLepID->Fill(sumLepCharge,weight);
//            }
//            if (passesLeptonCut)      AllPassWithMassCutLepIDCharge->Fill(sumLepCharge,weight);
//        }
//        //---------
        
        //---filling MT 2D Gen vs Reco without MT cut
        if(hasRecoInfo && hasGenInfo && passesLeptonReq && passesBtagReq && passesTau3Req && passesGenLeptonReq){
            full2DMT->Fill(MT, genMT, weight);
            full2DMET->Fill(METpt, genLepton2.pt, weight);
            full2DMTdiff->Fill(genMT, MT-genMT,  weight);
            
            dphiLep1Lep2Full->Fill(deltaPhi(lep1, lep2), weight);
            gendphiLep1Lep2Full->Fill(deltaPhi(genLep1, genLep2), genWeight);
            dphi2DLep1Lep2Full->Fill(deltaPhi(lep1, lep2), deltaPhi(genLep1, genLep2),  weight);
            
            genMETRatio->Fill(genLepton2.pt/METpt, weight);
            
            fullMT->Fill(MT, weight);
            fullMET->Fill(METpt, weight);
            
            fullgenMT->Fill(genMT, genWeight);
            fullgenMET->Fill(genLepton2.pt, genWeight);
            
            METvslepIso->Fill(METpt, lepton1.iso, weight);
            MTvslepIso->Fill(MT, lepton1.iso, weight);
        }
        
        if (DEBUG) cout << "Stop after line " << __LINE__ << "   " << hasGenInfo <<"    gen Wgh = " << genWeight << "  pass gen cuts = " << passesGenLeptonCut <<"  nGenJets = " << nGoodGenJets <<  endl;
        //=======================================================================================================//
        //   Filling gen and parton histos    //
        //====================================//
        if (hasGenInfo){
            if (passesGenLeptonCut && passesDPSPartonCut && passesGenJetCut){
                if (fileName.find("HepMC") == string::npos && pdfInfo_->size()>3){
                    partonX2D->Fill(pdfInfo_->at(2),pdfInfo_->at(3),genWeight);
                }
                GENnEventsIncl0Jets++;
               
                genZNGoodJets_Zexc->Fill(nGoodGenJets, genWeight);
                if(nGoodGenJets < 7){
                    genZNGoodJetsFull_Zexc->Fill(nGoodGenJets, genWeight);
                }
                genZNGoodJets_Zinc->Fill(0., genWeight);
                genZNGoodJetsFull_Zinc->Fill(0., genWeight);
                
                genMT_Zinc0jet->Fill(genMT, genWeight);
                genMET_Zinc0jet->Fill(genLepton2.pt, genWeight);
                if(lepton1.pt > 25 && lepton1.pt <= 30)   genMET_Zinc0jet_leppt_25_30->Fill(genLepton2.pt, genWeight);
                if(lepton1.pt > 30 && lepton1.pt <= 35)   genMET_Zinc0jet_leppt_30_35->Fill(genLepton2.pt, genWeight);
                if(lepton1.pt > 35 && lepton1.pt <= 40)   genMET_Zinc0jet_leppt_35_40->Fill(genLepton2.pt, genWeight);
                if(lepton1.pt > 40 && lepton1.pt <= 45)   genMET_Zinc0jet_leppt_40_45->Fill(genLepton2.pt, genWeight);
            

                genlepPtvsgenMET_Zinc0jet->Fill(genLep1.Pt(), genLepton2.pt, genWeight);
                
                genZMass_Zinc0jet->Fill(genZ.M(), genWeight);
                genZPt_Zinc0jet->Fill(genZ.Pt(), genWeight);
                genZRapidity_Zinc0jet->Fill(genZ.Rapidity(), genWeight);
                genZEta_Zinc0jet->Fill(genZ.Eta(), genWeight);
                genlepPt_Zinc0jet->Fill(genLep1.Pt(), genWeight);
                genlepEta_Zinc0jet->Fill(genLep1.Eta(), genWeight);


                if(genLep1.Pt() > 25 && genLep1.Pt() <= 30) genlepEta_Zinc0jet_leppt_25_30->Fill(genLep1.Eta(), genWeight);
                if(genLep1.Pt() > 30 && genLep1.Pt() <= 35) genlepEta_Zinc0jet_leppt_30_35->Fill(genLep1.Eta(), genWeight);
                if(genLep1.Pt() > 35 && genLep1.Pt() <= 40) genlepEta_Zinc0jet_leppt_35_40->Fill(genLep1.Eta(), genWeight);
                if(genLep1.Pt() > 40 && genLep1.Pt() <= 45) genlepEta_Zinc0jet_leppt_40_45->Fill(genLep1.Eta(), genWeight);
                if (doZ || doTT){
                    genlepPt_Zinc0jet->Fill(genLep2.Pt(), genWeight);
                    genlepEta_Zinc0jet->Fill(genLep2.Eta(), genWeight);
                }
                
                if (nGoodGenJets >= 1){
                    GENnEventsIncl1Jets++;
                    genZNGoodJets_Zinc->Fill(1., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(1., genWeight);
                    genFirstJetPt_Zinc1jet->Fill(genLeadJ.Pt(), genWeight);
                    genFirstJetPt_1_Zinc1jet->Fill(genLeadJ.Pt(), genWeight);
                    genFirstJetPt_2_Zinc1jet->Fill(genLeadJ.Pt(), genWeight);
                    genFirstJetEta_Zinc1jet->Fill(fabs(genLeadJ.Eta()), genWeight);
                    genFirstJetEta_2_Zinc1jet->Fill(fabs(genLeadJ.Eta()), genWeight);
                    genJetsHT_Zinc1jet->Fill(genJetsHT, genWeight);
                    genJetsHT_1_Zinc1jet->Fill(genJetsHT, genWeight);
                    genJetsHT_2_Zinc1jet->Fill(genJetsHT, genWeight);
                    //*************************************** begin edit *************************************************************//
                    gendPhiLepJet1_Zinc1jet->Fill(deltaPhi(genLep1, genNewLeadJ), genWeight);
                    gendPhiLepJet1_2_Zinc1jet->Fill(deltaPhi(genLep1, genNewLeadJ), genWeight);
                    
                    genMeanNJetsHT_1D_Zinc1jet->Fill(genJetsHT, genWeight*nGoodGenJets);
                    genMeanNJetsHT_Zinc1jet->Fill(genJetsHT, nGoodGenJets, genWeight);
                    //---
                    genFirstJetRapidity_Zinc1jet->Fill(fabs(genNewLeadJ.Rapidity()), genWeight);
                    genFirstJetRapidityFull_Zinc1jet->Fill(genNewLeadJ.Rapidity(), genWeight);
                    //*************************************** end edit ***************************************************************//
                    
                    genZMass_Zinc1jet->Fill(genZ.M(), genWeight);
                    genlepPt_Zinc1jet->Fill(genLep1.Pt(), genWeight);
                    genlepEta_Zinc1jet->Fill(genLep1.Eta(), genWeight);
                    if (doZ || doTT){
                        genlepPt_Zinc1jet->Fill(genLep2.Pt(), genWeight);
                        genlepEta_Zinc1jet->Fill(genLep2.Eta(), genWeight);
                    }
                    genZPt_Zinc1jet->Fill(genZ.Pt(), genWeight);
                    genZRapidity_Zinc1jet->Fill(genZ.Rapidity(), genWeight);
                    genZEta_Zinc1jet->Fill(genZ.Eta(), genWeight);
                    genFirstJetPtEta_Zinc1jet->Fill(genLeadJ.Pt(), fabs(genLeadJ.Eta()), genWeight);
                    genFirstHighestJetPt_Zinc1jet->Fill(genLeadJ.Pt(), genWeight);
                    
                    for ( int i =0 ; i < NbinsEta2D - 1 ; i++){
                        if ( fabs(genLeadJ.Eta()) >= j_Y_range[i] &&  fabs(genLeadJ.Eta()) < j_Y_range[i+1] )                                genFirstJetPt_Zinc1jet_Eta[i]->Fill(fabs(genLeadJ.Pt()), genWeight);
                    }
                    if ( doW ) gendEtaBosonJet_Zinc1jet->Fill(fabs(genLeadJ.Eta() - genLep1.Eta()), genWeight);
                    else gendEtaBosonJet_Zinc1jet->Fill(fabs(genLeadJ.Eta()-genZ.Eta()), genWeight);
                    if (nGoodGenJets == 1){
                        genFirstJetPt_Zexc1jet->Fill(genLeadJ.Pt(), genWeight);
                        //    gendEtaBosonJet_Zexc1jet->Fill(fabs(genLeadJ.Eta()), genWeight);
                        if ( doW ) gendEtaBosonJet_Zexc1jet->Fill(fabs(genLeadJ.Eta() - genLep1.Eta()), genWeight);
                        else gendEtaBosonJet_Zexc1jet->Fill(fabs(genLeadJ.Eta()-genZ.Eta()), genWeight);

                    }
                }
                if (nGoodGenJets >= 2){
                    TLorentzVector genJet1Plus2PlusZ = genJet1Plus2 + genZ;
                    GENnEventsIncl2Jets++;
                    
                    genZNGoodJets_Zinc->Fill(2., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(2., genWeight);
                    genSecondJetPt_Zinc2jet->Fill(genSecondJ.Pt(), genWeight);
                    genSecondJetPt_1_Zinc2jet->Fill(genSecondJ.Pt(), genWeight);
                    genSecondJetPt_2_Zinc2jet->Fill(genSecondJ.Pt(), genWeight);
                    genSecondJetEta_Zinc2jet->Fill(fabs(genSecondJ.Eta()), genWeight);
                    genSecondJetEta_2_Zinc2jet->Fill(fabs(genSecondJ.Eta()), genWeight);
                    genJetsHT_Zinc2jet->Fill(genJetsHT, genWeight);
                    genJetsHT_1_Zinc2jet->Fill(genJetsHT, genWeight);
                    genJetsHT_2_Zinc2jet->Fill(genJetsHT, genWeight);
                    //*************************************** begin edit *******************************************************//
                    gendRapidityJets_Zinc2jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight);
                    gendRapidityJets_2_Zinc2jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight);
                    
                    gendRapidityJetsFB_Zinc2jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight);
                    gendRapidityJetsFB_2_Zinc2jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight);
                    
                    gendPhiJets_Zinc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                    gendPhiJets_2_Zinc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                    
                    gendPhiJetsFB_Zinc2jet->Fill(deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), genWeight);
                    gendPhiJetsFB_2_Zinc2jet->Fill(deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), genWeight);
                    
                    gendiJetMass_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    gendiJetMass_2_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    
                    gendRJets_Zinc2jet->Fill(deltaRYPhi(genNewLeadJ, genNewSecondJ), genWeight);
                    gendRJets_2_Zinc2jet->Fill(deltaRYPhi(genNewLeadJ, genNewSecondJ), genWeight);
                    
                    gendiJetPt_Zinc2jet->Fill(genJet1Plus2.Pt(), genWeight);
                    gendiJetPt_2_Zinc2jet->Fill(genJet1Plus2.Pt(), genWeight);
                    
                    gendPhiLepJet2_Zinc2jet->Fill(deltaPhi(genLep1, genNewSecondJ), genWeight);
                    gendPhiLepJet2_2_Zinc2jet->Fill(deltaPhi(genLep1, genNewSecondJ), genWeight);
                    
                    genMeanNJetsHT_Zinc2jet->Fill(genJetsHT, nGoodGenJets, genWeight);
                    genMeanNJetsdRapidity_Zinc2jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), nGoodGenJets, genWeight);
                    genMeanNJetsdRapidityFB_Zinc2jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, nGoodGenJets, genWeight);
                    //---
                    genSecondJetRapidity_Zinc2jet->Fill(fabs(genNewSecondJ.Rapidity()), genWeight);
                    genSecondJetRapidityFull_Zinc2jet->Fill(genNewSecondJ.Rapidity(), genWeight);
                    genMeanNJetsHT_1D_Zinc2jet->Fill(genJetsHT, genWeight*nGoodGenJets);
                    genMeanNJetsdRapidity_1D_Zinc2jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight*nGoodGenJets);
                    genMeanNJetsdRapidityFB_1D_Zinc2jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight*nGoodGenJets);
                    //*************************************** end edit *********************************************************//
                    
                    genZMass_Zinc2jet->Fill(genZ.M(), genWeight);
                    genTwoJetsPtDiff_Zinc2jet->Fill(genJet1Minus2.Pt(), genWeight);
                    genBestTwoJetsPtDiff_Zinc2jet->Fill(genBestJet1Minus2.Pt(), genWeight);
                    genJetsMass_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    genlepPt_Zinc2jet->Fill(genLep1.Pt(), genWeight);
                    genlepEta_Zinc2jet->Fill(genLep1.Eta(), genWeight);
                    if (doZ || doTT){
                        genlepPt_Zinc2jet->Fill(genLep2.Pt(), genWeight);
                        genlepEta_Zinc2jet->Fill(genLep2.Eta(), genWeight);
                    }
                    genZPt_Zinc2jet->Fill(genZ.Pt(), genWeight);
                    genZRapidity_Zinc2jet->Fill(genZ.Rapidity(), genWeight);
                    genZEta_Zinc2jet->Fill(genZ.Eta(), genWeight);
                    genFirstHighestJetPt_Zinc2jet->Fill(genLeadJ.Pt(), genWeight);
                    genSecondHighestJetPt_Zinc2jet->Fill(genSecondJ.Pt(), genWeight);
                    genSecondJetPtEta_Zinc2jet->Fill(genSecondJ.Pt(), fabs(genSecondJ.Eta()), genWeight);
                    genRatioJetPt21_Zinc2jet->Fill(genJets[1].pt/genJets[0].pt, genWeight);
                    genptBal_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                    genBestdPhiJets_Zinc2jet->Fill(deltaPhi(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    gendEtaJets_Zinc2jet->Fill(genLeadJ.Eta() - genSecondJ.Eta(), genWeight);
                    gendEtaFirstJetZ_Zinc2jet->Fill(genLeadJ.Eta() - genZ.Eta(), genWeight);
                    gendEtaSecondJetZ_Zinc2jet->Fill(genSecondJ.Eta() - genZ.Eta(), genWeight);
                    gendEtaJet1Plus2Z_Zinc2jet->Fill(genJet1Plus2.Eta() - genZ.Eta(), genWeight);
                    genPHI_Zinc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                    genBestPHI_Zinc2jet->Fill(PHI(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genPHI_T_Zinc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                    genBestPHI_T_Zinc2jet->Fill(PHI_T(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genSpT_Zinc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                    genBestSpT_Zinc2jet->Fill(SpT(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genSpTJets_Zinc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                    genBestSpTJets_Zinc2jet->Fill(SpTsub(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genSpTLeptons_Zinc2jet->Fill(SpTsub(genLep1, genLep2), genWeight);
                    genSPhi_Zinc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                    genBestSPhi_Zinc2jet->Fill(SPhi(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    for ( int i =0 ; i < NbinsEta2D - 1 ; i++){
                        if ( fabs(genSecondJ.Eta()) >= j_Y_range[i] &&  fabs(genSecondJ.Eta()) < j_Y_range[i+1] )                                                genSecondJetPt_Zinc2jet_Eta[i]->Fill(fabs(genSecondJ.Pt()), genWeight);
                    }
                    //--- START EWK ---
                    if (passesGenEWKJetPt){
                        // met histos
                        //
                        //METEWK_Zinc2jet->Fill(METpt, weight);
                        //METphiEWK_Zinc2jet->Fill(METphi,  weight);
                        //MTEWK_Zinc2jet->Fill(MT,  weight);
                        //          
                        // jet histos
                        genZNGoodJetsEWK_Zexc->Fill(nGoodGenJets, genWeight);
                        genFirstJetPtEWK_Zinc2jet->Fill(genJets[0].pt, genWeight);
                        genFirstJetEtaEWK_Zinc2jet->Fill(genJets[0].eta, genWeight);
                        genFirstJetPhiEWK_Zinc2jet->Fill(genJets[0].phi, genWeight);


                        genSecondJetPtEWK_Zinc2jet->Fill(genJets[1].pt, genWeight);
                        genSecondJetEtaEWK_Zinc2jet->Fill(genJets[1].eta, genWeight);
                        genSecondJetPhiEWK_Zinc2jet->Fill(genJets[1].phi, genWeight);
                        int temIND(0), temIND1(1);
                        if (fabs(genJets[1].eta) > fabs(genJets[0].eta)){ 
                            temIND = 1; 
                            temIND1 = 0;
                        }
                        genForwardJetPtEWK_Zinc2jet->Fill(genJets[temIND].pt, genWeight);
                        genForwardJetEtaEWK_Zinc2jet->Fill(genJets[temIND].eta, genWeight);
                        genForwardJetPhiEWK_Zinc2jet->Fill(genJets[temIND].phi, genWeight);


                        genCentralJetPtEWK_Zinc2jet->Fill(genJets[temIND1].pt, genWeight);
                        genCentralJetEtaEWK_Zinc2jet->Fill(genJets[temIND1].eta, genWeight);
                        genCentralJetPhiEWK_Zinc2jet->Fill(genJets[temIND1].phi, genWeight);


                        genJetsHTEWK_Zinc2jet->Fill(genJetsHT, genWeight);
                        genJetsMassEWK_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);

                        // multi jet variables
                        genSumEtaJetsEWK_Zinc2jet->Fill(fabs(genLeadJ.Eta() + genSecondJ.Eta()),  genWeight);
                        genAbsSumEtaJetsEWK_Zinc2jet->Fill(fabs(genLeadJ.Eta()) + fabs(genSecondJ.Eta()),  genWeight);
                        genTwoJetsPtDiffEWK_Zinc2jet->Fill(genJet1Minus2.Pt(), genWeight);
                        genptBalEWK_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                        gendPhiJetsEWK_Zinc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                        gendEtaJetsEWK_Zinc2jet->Fill(fabs(genLeadJ.Eta() - genSecondJ.Eta()), genWeight);
                        genSpTJetsEWK_Zinc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                        gendPhiJetsEWK_Zinc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);

                        // find jet properties of the third jet that is  between the two leading jets
                        int nGoodJetsBtw(0.);
                        double jetsHTBtw(0.);
                        if (nGenJetsAdd > 2){
                            genThirdJetPtEWKadd_Zinc2jet->Fill(genJetsAdditional[2].pt, genWeight);
                            genThirdJetEtaEWKadd_Zinc2jet->Fill(genJetsAdditional[2].eta, genWeight);
                            genThirdJetPhiEWKadd_Zinc2jet->Fill(genJetsAdditional[2].phi, genWeight);
                            for (unsigned short i(2); i < nGenJetsAdd; i++) {
                                if (genJetsAdditional[i].eta < max(genJets[0].eta,genJets[1].eta) -0.5 && genJetsAdditional[i].eta > min(genJets[0].eta,genJets[1].eta) + 0.5){
                                    jetsHTBtw += genJetsAdditional[i].pt;
                                    nGoodJetsBtw++;
                                    genAllJetPtEWKbtw_Zinc2jet->Fill(genJetsAdditional[i].pt, genWeight);
                                    genAllJetEtaEWKbtw_Zinc2jet->Fill(genJetsAdditional[i].eta, genWeight);
                                    genAllJetPhiEWKbtw_Zinc2jet->Fill(genJetsAdditional[i].phi, genWeight);
                                    if (nGoodJetsBtw == 1){
                                        genThirdJetPtEWKbtw_Zinc2jet->Fill(genJetsAdditional[i].pt, genWeight);
                                        genThirdJetEtaEWKbtw_Zinc2jet->Fill(genJetsAdditional[i].eta, genWeight);
                                        genThirdJetPhiEWKbtw_Zinc2jet->Fill(genJetsAdditional[i].phi, genWeight);
                                    }
                                }
                            }
                            genJetsHTEWKbtw_Zinc2jet->Fill(jetsHTBtw, genWeight);
                            genZNGoodJetsEWKbtw_Zexc->Fill(nGoodJetsBtw, genWeight);
                        }

                    }
                    //////////////////////// STOP EWK
                    if (genZ.Pt() < 25){
                        genptBal_LowPt_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                        gendPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                        genBestdPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        gendPhiLeptons_LowPt_Zinc2jet->Fill(deltaPhi(genLep1, genLep2), genWeight);
                        genPHI_T_LowPt_Zinc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genBestPHI_T_LowPt_Zinc2jet->Fill(PHI_T(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genPHI_LowPt_Zinc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genBestPHI_LowPt_Zinc2jet->Fill(PHI(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genSpTJets_LowPt_Zinc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                        genBestSpTJets_LowPt_Zinc2jet->Fill(SpTsub(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genSpTLeptons_LowPt_Zinc2jet->Fill(SpTsub(genLep1, genLep2), genWeight);
                        genSpT_LowPt_Zinc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genBestSpT_LowPt_Zinc2jet->Fill(SpT(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genSPhi_LowPt_Zinc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genBestSPhi_LowPt_Zinc2jet->Fill(SPhi(genLep1, genLep2, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        if (SpT(genLep1, genLep2, genLeadJ, genSecondJ) < 0.5){ 
                            genPHI_LowSpT_LowPt_Zinc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genSPhi_LowSpT_LowPt_Zinc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        }
                        else {
                            genPHI_HighSpT_LowPt_Zinc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genSPhi_HighSpT_LowPt_Zinc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        }
                        if (SPhi(genLep1, genLep2, genLeadJ, genSecondJ) < 0.5){
                            genSpT_LowSPhi_LowPt_Zinc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        }
                        else {
                            genSpT_HighSPhi_LowPt_Zinc2jet ->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        }
                    }
                    else {
                        genptBal_HighPt_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                        gendPhiJets_HighPt_Zinc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                        gendPhiLeptons_HighPt_Zinc2jet->Fill(deltaPhi(genLep1, genLep2), genWeight);
                        genPHI_HighPt_Zinc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genPHI_T_HighPt_Zinc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genSpTJets_HighPt_Zinc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                        genSpTLeptons_HighPt_Zinc2jet->Fill(SpTsub(genLep1, genLep2), genWeight);
                        genSpT_HighPt_Zinc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genSPhi_HighPt_Zinc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                    }
                    if (nGoodGenJets == 2){
                        genTwoJetsPtDiff_Zexc2jet->Fill(genJet1Minus2.Pt(), genWeight);
                        genJetsMass_Zexc2jet->Fill(genJet1Plus2.M(), genWeight);
                        genSecondJetPt_Zexc2jet->Fill(genSecondJ.Pt(), genWeight);
                        gendPhiJets_Zexc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                        genPHI_Zexc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genPHI_T_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        gendEtaJets_Zexc2jet->Fill(genLeadJ.Eta() - genSecondJ.Eta(), genWeight);
                        gendEtaFirstJetZ_Zexc2jet->Fill(genLeadJ.Eta() - genZ.Eta(), genWeight);
                        gendEtaSecondJetZ_Zexc2jet->Fill(genSecondJ.Eta() - genZ.Eta(), genWeight);
                        gendEtaJet1Plus2Z_Zexc2jet->Fill(genJet1Plus2.Eta() - genZ.Eta(), genWeight);
                        genSpT_Zexc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genSpTJets_Zexc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                        genSpTLeptons_Zexc2jet->Fill(SpTsub(genLep1, genLep2), genWeight);
                        genSPhi_Zexc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        genptBal_Zexc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);

                        if (genZ.Pt() < 25){
                            genptBal_LowPt_Zexc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                            gendPhiJets_LowPt_Zexc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                            gendPhiLeptons_LowPt_Zexc2jet->Fill(deltaPhi(genLep1, genLep2), genWeight);
                            genPHI_T_LowPt_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genPHI_LowPt_Zexc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genSpTJets_LowPt_Zexc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                            genSpTLeptons_LowPt_Zexc2jet->Fill(SpTsub(genLep1, genLep2), genWeight);
                            genSpT_LowPt_Zexc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genSPhi_LowPt_Zexc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            if (SpT(genLep1, genLep2, genLeadJ, genSecondJ) < 0.5) { 
                                genPHI_LowSpT_LowPt_Zexc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                                genSPhi_LowSpT_LowPt_Zexc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            }
                            else {
                                genPHI_HighSpT_LowPt_Zexc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                                genSPhi_HighSpT_LowPt_Zexc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            }
                            if (SPhi(genLep1, genLep2, genLeadJ, genSecondJ) < 0.5) {
                                genSpT_LowSPhi_LowPt_Zexc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            }
                            else {
                                genSpT_HighSPhi_LowPt_Zexc2jet ->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            }
                        }
                        else {
                            genptBal_HighPt_Zexc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                            gendPhiJets_HighPt_Zexc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                            gendPhiLeptons_HighPt_Zexc2jet->Fill(deltaPhi(genLep1, genLep2), genWeight);
                            genPHI_HighPt_Zexc2jet->Fill(PHI(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genPHI_T_HighPt_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genSpTJets_HighPt_Zexc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                            genSpTLeptons_HighPt_Zexc2jet->Fill(SpTsub(genLep1, genLep2), genWeight);
                            genSpT_HighPt_Zexc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                            genSPhi_HighPt_Zexc2jet->Fill(SPhi(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                        }
                    }
                    if (hasPartonInfo){
                        if (genDPSAndItsJets.size() == 2) {
                            if ((deltaR(genDPSAndItsJets[0].first, genLeadJ) < 0.5 || deltaR(genDPSAndItsJets[0].first, genSecondJ) < 0.5) 
                                    && (deltaR(genDPSAndItsJets[1].first, genLeadJ) < 0.5 || deltaR(genDPSAndItsJets[1].first, genSecondJ) < 0.5)) {
                                genSpTJetsDeltaR_Zexc2jet->Fill(SpTsub(genLeadJ, genSecondJ), genWeight);
                                gendPhiJetsDeltaR_Zexc2jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                                genPHI_TDeltaR_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                                genSpTDeltaR_Zexc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ), genWeight);
                                if (hasRecoInfo) {
                                    resSpTJetsDeltaR_Zexc2jet->Fill(SpTsub(genLeadJ, genSecondJ) - SpTsub(leadJ, secondJ) , genWeight);
                                    resdPhiJetsDeltaR_Zexc2jet->Fill(deltaPhi(genLeadJ, genSecondJ) - deltaPhi(leadJ, secondJ), genWeight);
                                    resPHI_TDeltaR_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genLeadJ, genSecondJ) - PHI_T(lep1, lep2, leadJ, secondJ), genWeight);
                                    resSpTDeltaR_Zexc2jet->Fill(SpT(genLep1, genLep2, genLeadJ, genSecondJ) - SpT(lep1, lep2, leadJ, secondJ), genWeight);
                                }
                            }
                            gendPhiJetsDPS_Zexc2jet->Fill(deltaPhi(genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                            genPHI_TDPS_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                            genSpTDPSPartons_Zexc2jet->Fill(SpTsub(genDPSAndItsJets[0].first, genDPSAndItsJets[1].first), genWeight);
                            genSpTJetsDPS_Zexc2jet->Fill(SpTsub(genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                            genSpTDPS_Zexc2jet->Fill(SpT(genLep1, genLep2, genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                            if (deltaR(genDPSAndItsJets[0].first, genDPSAndItsJets[0].second) < 0.5 && deltaR(genDPSAndItsJets[1].first, genDPSAndItsJets[1].second)) {
                                gendPhiJetsDPSDeltaR_Zexc2jet->Fill(deltaPhi(genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                                genSpTJetsDPSDeltaR_Zexc2jet->Fill(SpTsub(genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                                genSpTDPSDeltaR_Zexc2jet->Fill(SpT(genLep1, genLep2, genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                                genPHI_TDPSDeltaR_Zexc2jet->Fill(PHI_T(genLep1, genLep2, genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genWeight);
                                gendPhiJetsDPSDeltaR_ZpT_Zexc2jet->Fill(deltaPhi(genDPSAndItsJets[0].second, genDPSAndItsJets[1].second), genZ.Pt(), genWeight);
                            }
                        }
                    }
                }
                if (nGoodGenJets >= 3){
                    GENnEventsIncl3Jets++;
                    genZNGoodJets_Zinc->Fill(3., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(3., genWeight);
                    genThirdJetPt_Zinc3jet->Fill(genJets[2].pt, genWeight);
                    genThirdJetPt_1_Zinc3jet->Fill(genJets[2].pt, genWeight);
                    genThirdJetPt_2_Zinc3jet->Fill(genJets[2].pt, genWeight);
                    genThirdJetEta_Zinc3jet->Fill(fabs(genJets[2].eta), genWeight);
                    genThirdJetEta_2_Zinc3jet->Fill(fabs(genJets[2].eta), genWeight);
                    genJetsHT_Zinc3jet->Fill(genJetsHT, genWeight);
                    genJetsHT_1_Zinc3jet->Fill(genJetsHT, genWeight);
                    genJetsHT_2_Zinc3jet->Fill(genJetsHT, genWeight);
                    //*************************************** begin edit *************************************************************//
                    gendRapidityJets_Zinc3jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight);
                    gendRapidityJets_2_Zinc3jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight);
                    
                    gendRapidityJets_First_Third_Zinc3jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    gendRapidityJets_2_First_Third_Zinc3jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    
                    gendRapidityJets_Second_Third_Zinc3jet->Fill(fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    gendRapidityJets_2_Second_Third_Zinc3jet->Fill(fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    
                    gendRapidityJetsFB_Zinc3jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight);
                    gendRapidityJetsFB_2_Zinc3jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight);
                    
                    gendPhiJets_Zinc3jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                    gendPhiJets_2_Zinc3jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                    
                    gendPhiJetsFB_Zinc3jet->Fill(deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), genWeight);
                    gendPhiJetsFB_2_Zinc3jet->Fill(deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), genWeight);
                    
                    gendiJetMass_Zinc3jet->Fill(genJet1Plus2.M(), genWeight);
                    gendiJetMass_2_Zinc3jet->Fill(genJet1Plus2.M(), genWeight);
                    
                    gendiJetPt_Zinc3jet->Fill(genJet1Plus2.Pt(), genWeight);
                    gendiJetPt_2_Zinc3jet->Fill(genJet1Plus2.Pt(), genWeight);
                    
                    gendPhiLepJet3_Zinc3jet->Fill(deltaPhi(genLep1, genNewThirdJ), genWeight);
                    gendPhiLepJet3_2_Zinc3jet->Fill(deltaPhi(genLep1, genNewThirdJ), genWeight);
                    //---
                    genThirdJetRapidity_Zinc3jet->Fill(fabs(genNewThirdJ.Rapidity()), genWeight);
                    genThirdJetRapidityFull_Zinc3jet->Fill(genNewThirdJ.Rapidity(), genWeight);
                    //*************************************** end edit ***************************************************************//
                    
                    genZMass_Zinc3jet->Fill(genZ.M(), genWeight);
                    genZPt_Zinc3jet->Fill(genZ.Pt(), genWeight);
                    genZRapidity_Zinc3jet->Fill(genZ.Rapidity(), genWeight);
                    genZEta_Zinc3jet->Fill(genZ.Eta(), genWeight);
                    genThirdJetPtEta_Zinc3jet->Fill(genJets[2].pt, fabs(genJets[2].eta), genWeight);
                    genRatioJetPt32_Zinc3jet->Fill(genJets[2].pt/genJets[1].pt, genWeight);
                    genFirstHighestJetPt_Zinc3jet->Fill(genJets[0].pt, genWeight);
                    genSecondHighestJetPt_Zinc3jet->Fill(genJets[1].pt, genWeight);
                    genThirdHighestJetPt_Zinc3jet->Fill(genJets[2].pt, genWeight);
                    
                    
                }
                if (nGoodGenJets >= 4){
                    genZNGoodJets_Zinc->Fill(4., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(4., genWeight);
                    genFourthJetPt_Zinc4jet->Fill(genJets[3].pt, genWeight);
                    genFourthJetPt_1_Zinc4jet->Fill(genJets[3].pt, genWeight);
                    genFourthJetPt_2_Zinc4jet->Fill(genJets[3].pt, genWeight);
                    genFourthJetEta_Zinc4jet->Fill(fabs(genJets[3].eta), genWeight);
                    genFourthJetEta_2_Zinc4jet->Fill(fabs(genJets[3].eta), genWeight);
                    genJetsHT_Zinc4jet->Fill(genJetsHT, genWeight);
                    genJetsHT_1_Zinc4jet->Fill(genJetsHT, genWeight);
                    genJetsHT_2_Zinc4jet->Fill(genJetsHT, genWeight);
                    //*************************************** begin edit *************************************************************//
                    gendRapidityJets_Zinc4jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight);
                    gendRapidityJets_2_Zinc4jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), genWeight);
                    
                    gendRapidityJets_First_Third_Zinc4jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    gendRapidityJets_2_First_Third_Zinc4jet->Fill(fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    
                    gendRapidityJets_Second_Third_Zinc4jet->Fill(fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    gendRapidityJets_2_Second_Third_Zinc4jet->Fill(fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), genWeight);
                    
                    gendRapidityJetsFB_Zinc4jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight);
                    gendRapidityJetsFB_2_Zinc4jet->Fill(genForwardJetRapidity - genBackwardJetRapidity, genWeight);
                    
                    gendPhiJets_Zinc4jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                    gendPhiJets_2_Zinc4jet->Fill(deltaPhi(genLeadJ, genSecondJ), genWeight);
                    
                    gendPhiJetsFB_Zinc4jet->Fill(deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), genWeight);
                    gendPhiJetsFB_2_Zinc4jet->Fill(deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), genWeight);
                    
                    gendiJetMass_Zinc4jet->Fill(genJet1Plus2.M(), genWeight);
                    gendiJetMass_2_Zinc4jet->Fill(genJet1Plus2.M(), genWeight);
                    
                    gendiJetPt_Zinc4jet->Fill(genJet1Plus2.Pt(), genWeight);
                    gendiJetPt_2_Zinc4jet->Fill(genJet1Plus2.Pt(), genWeight);
                    
                    gendPhiLepJet4_Zinc4jet->Fill(deltaPhi(genLep1, genNewFourthJ), genWeight);
                    gendPhiLepJet4_2_Zinc4jet->Fill(deltaPhi(genLep1, genNewFourthJ), genWeight);
                    //---
                    genFourthJetRapidity_Zinc4jet->Fill(fabs(genNewFourthJ.Rapidity()), genWeight);
                    genFourthJetRapidityFull_Zinc4jet->Fill(genNewFourthJ.Rapidity(), genWeight);
                    //*************************************** end edit ***************************************************************//
                    genZMass_Zinc4jet->Fill(genZ.M(), genWeight);
                    genZPt_Zinc4jet->Fill(genZ.Pt(), genWeight);
                    genZRapidity_Zinc4jet->Fill(genZ.Rapidity(), genWeight);
                    genZEta_Zinc4jet->Fill(genZ.Eta(), genWeight);
                    genFourthJetPtEta_Zinc4jet->Fill(genJets[3].pt, fabs(genJets[3].eta), genWeight);
                    genFirstHighestJetPt_Zinc4jet->Fill(genJets[0].pt, genWeight);
                    genSecondHighestJetPt_Zinc4jet->Fill(genJets[1].pt, genWeight);
                    genThirdHighestJetPt_Zinc4jet->Fill(genJets[2].pt, genWeight);
                    
                }
                if (nGoodGenJets >= 5){
                    genZNGoodJets_Zinc->Fill(5., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(5., genWeight);
                    genFifthJetPt_Zinc5jet->Fill(genJets[4].pt, genWeight);
                    genFifthJetPt_1_Zinc5jet->Fill(genJets[4].pt, genWeight);
                    genFifthJetPt_2_Zinc5jet->Fill(genJets[4].pt, genWeight);
                    genFifthJetEta_Zinc5jet->Fill(fabs(genJets[4].eta), genWeight);
                    genFifthJetEta_2_Zinc5jet->Fill(fabs(genJets[4].eta), genWeight);
                    genJetsHT_Zinc5jet->Fill(genJetsHT, genWeight);
                    genJetsHT_1_Zinc5jet->Fill(genJetsHT, genWeight);
                    genJetsHT_2_Zinc5jet->Fill(genJetsHT, genWeight);
                    //*************************************** begin edit *************************************************************//
                    gendPhiLepJet5_Zinc5jet->Fill(deltaPhi(genLep1, genNewFifthJ), genWeight);
                    gendPhiLepJet5_2_Zinc5jet->Fill(deltaPhi(genLep1, genNewFifthJ), genWeight);
                    //*************************************** end edit ***************************************************************//
                    
                    genZMass_Zinc5jet->Fill(genZ.M(), genWeight);
                    genZPt_Zinc5jet->Fill(genZ.Pt(), genWeight);
                    genZRapidity_Zinc5jet->Fill(genZ.Rapidity(), genWeight);
                    genZEta_Zinc5jet->Fill(genZ.Eta(), genWeight);
                    genFifthJetPtEta_Zinc5jet->Fill(genJets[4].pt, fabs(genJets[4].eta), genWeight);
                    genFirstHighestJetPt_Zinc5jet->Fill(genJets[0].pt, genWeight);
                    genSecondHighestJetPt_Zinc5jet->Fill(genJets[1].pt, genWeight);
                    genThirdHighestJetPt_Zinc5jet->Fill(genJets[2].pt, genWeight);
                    
                }
                if (nGoodGenJets >= 6){
                    genZNGoodJets_Zinc->Fill(6., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(6., genWeight);
                    genSixthJetPt_Zinc6jet->Fill(genJets[5].pt, genWeight);
                    genSixthJetPt_1_Zinc6jet->Fill(genJets[5].pt, genWeight);
                    genSixthJetEta_Zinc6jet->Fill(fabs(genJets[5].eta), genWeight);
                    genJetsHT_Zinc6jet->Fill(genJetsHT, genWeight);
                    genJetsHT_1_Zinc6jet->Fill(genJetsHT, genWeight);
                    
                    genZMass_Zinc6jet->Fill(genZ.M(), genWeight);
                    genZPt_Zinc6jet->Fill(genZ.Pt(), genWeight);
                    genZRapidity_Zinc6jet->Fill(genZ.Rapidity(), genWeight);
                    genZEta_Zinc6jet->Fill(genZ.Eta(), genWeight);
                    genSixthJetPtEta_Zinc6jet->Fill(genJets[5].pt, fabs(genJets[5].eta), genWeight);
                    genFirstHighestJetPt_Zinc6jet->Fill(genJets[0].pt, genWeight);
                    genSecondHighestJetPt_Zinc6jet->Fill(genJets[1].pt, genWeight);
                    genThirdHighestJetPt_Zinc6jet->Fill(genJets[2].pt, genWeight);
                }
                if (nGoodGenJets >= 7){
                    genZNGoodJets_Zinc->Fill(7., genWeight);
                    genZNGoodJetsFull_Zinc->Fill(7., genWeight);
                    genZNGoodJetsFull_Zexc->Fill(7., genWeight);
                }
                
                if (nGoodGenJets >= 8) genZNGoodJets_Zinc->Fill(8., genWeight);
                if (doW && nGoodGenJets >= 9) genZNGoodJets_Zinc->Fill(9., genWeight);
                if (doW && nGoodGenJets >= 10) genZNGoodJets_Zinc->Fill(10., genWeight);
            }
        }
        //end of Filling gen and parton histos    //
        
        //=======================================================================================================//
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;

        //=======================================================================================================//
        //      Selection for Reco Histos      //
        //====================================//
        if (hasRecoInfo && passesLeptonCut && passesJetCut && passesDPSPartonCut) {
            //=======================================================================================================//
            
            if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
            //=======================================================================================================//
            //      Start filling histograms      //
            //====================================//
            
            
            TotalRecoWeightPassRECO+=weight;
            TotalGenWeightPassRECO+=genWeightBackup;
            NVtx->Fill(EvtInfo_NumVtx, weight);
            
            //NVtx->Fill(EvtInfo_NumVtx + 1000 , weight);
            if (fileName.find("Sherpa") != string::npos && fileName.find("UNFOL") == string::npos ) PUWeight->Fill(puWeight.weight(int(PU_npT)) * reweighting * mcEveWeight_, 1);
            else PUWeight->Fill(puWeight.weight(int(PU_npT)) * reweighting, 1);
            if (nGoodJets == 0){
                PUWeight0->Fill(puWeight.weight(int(PU_npT)) * reweighting, 1);
            }
            else {
                PUWeight1->Fill(puWeight.weight(int(PU_npT)) * reweighting, 1);
            }
            
            if (lepton1.charge > 0){
                MuPlusPt->Fill(lepton1.pt, weight);
                MuPlusEta->Fill(lepton1.eta, weight);
                if (doZ || doTT){
                    MuMinusPt->Fill(lepton2.pt, weight);
                    MuMinusEta->Fill(lepton2.eta, weight);
                }
            }
            else {
                MuMinusPt->Fill(lepton1.pt, weight);
                MuMinusEta->Fill(lepton1.eta, weight);
                if (doZ || doTT){
                    MuPlusPt->Fill(lepton2.pt, weight);
                    MuPlusEta->Fill(lepton2.eta, weight);
                }
            }
            
            nEventsIncl0Jets++;
            
            ZNGoodJets_Zinc->Fill(0., weight);
            ZNGoodJetsFull_Zinc->Fill(0., weight);
            ZNGoodJets_Zexc->Fill(nGoodJets, weight);
            if (nGoodJets < 7){
                ZNGoodJetsFull_Zexc->Fill(nGoodJets, weight);
            }
            
            MT_Zinc0jet->Fill(MT, weight);
            if(lepton1.pt > 25 && lepton1.pt <= 30)   MT_Zinc0jet_leppt_25_30->Fill(MT, weight);
            if(lepton1.pt > 30 && lepton1.pt <= 35)   MT_Zinc0jet_leppt_30_35->Fill(MT, weight);
            if(lepton1.pt > 35 && lepton1.pt <= 40)   MT_Zinc0jet_leppt_35_40->Fill(MT, weight);
            if(lepton1.pt > 40 && lepton1.pt <= 45)   MT_Zinc0jet_leppt_40_45->Fill(MT, weight);
          
            MET_Zinc0jet->Fill(METpt, weight);
            if(lepton1.pt > 25 && lepton1.pt <= 30)   MET_Zinc0jet_leppt_25_30->Fill(METpt, weight);
            if(lepton1.pt > 30 && lepton1.pt <= 35)   MET_Zinc0jet_leppt_30_35->Fill(METpt, weight);
            if(lepton1.pt > 35 && lepton1.pt <= 40)   MET_Zinc0jet_leppt_35_40->Fill(METpt, weight);
            if(lepton1.pt > 40 && lepton1.pt <= 45)   MET_Zinc0jet_leppt_40_45->Fill(METpt, weight);
            
            MuPFIso_Zinc0jet->Fill(lepton1.iso, weight);
            MuPFIso_2ndZinc0jet->Fill(lepton1.iso, weight);
            MuPFIso_3rdZinc0jet->Fill(lepton1.iso, weight);
            lepPt_Zinc0jet->Fill(lepton1.pt, weight);

            lepEta_Zinc0jet->Fill(lepton1.eta, weight);
            if(lepton1.pt > 25 && lepton1.pt <= 30)   lepEta_Zinc0jet_leppt_25_30->Fill(lepton1.eta, weight);
            if(lepton1.pt > 30 && lepton1.pt <= 35)   lepEta_Zinc0jet_leppt_30_35->Fill(lepton1.eta, weight);
            if(lepton1.pt > 35 && lepton1.pt <= 40)   lepEta_Zinc0jet_leppt_35_40->Fill(lepton1.eta, weight);
            if(lepton1.pt > 40 && lepton1.pt <= 45)   lepEta_Zinc0jet_leppt_40_45->Fill(lepton1.eta, weight);

            lepPhi_Zinc0jet->Fill(lepton1.phi, weight);
            
            lepPtvsMET_Zinc0jet->Fill(lepton1.pt, METpt, weight);
           
            ZNGoodJetsNVtx_Zexc->Fill(nGoodJets, EvtInfo_NumVtx  , weight);
            ZNGoodJets_Zinc_NoWeight->Fill(0.);
            ZMass_Zinc0jet->Fill(Z.M(), weight);
            METphi_Zinc0jet->Fill(METphi, weight);
            ZPt_Zinc0jet->Fill(Z.Pt(), weight);
            ZRapidity_Zinc0jet->Fill(Z.Rapidity(), weight);
            ZEta_Zinc0jet->Fill(Z.Eta(), weight);
            if (doZ || doTT){
                lepPt_Zinc0jet->Fill(lepton2.pt, weight);
                lepEta_Zinc0jet->Fill(lepton2.eta, weight);
                lepPhi_Zinc0jet->Fill(lepton2.phi, weight);
            }
            lepEtaEta_Zinc0jet->Fill(lepton1.eta, lepton2.eta, weight);
            dPhiLeptons_Zinc0jet->Fill(deltaPhi(lep1, lep2), weight);
            dEtaLeptons_Zinc0jet->Fill(lepton1.eta - lepton2.eta, weight);
            dRLeptons_Zinc0jet->Fill(deltaR(lepton1.phi, lepton1.eta, lepton2.phi, lepton2.eta), weight);
            SpTLeptons_Zinc0jet->Fill(SpTsub(lep1, lep2), weight);
            if (nGoodJets == 0){
                nEventsExcl0Jets++;
                ZNGoodJets_Zexc_NoWeight->Fill(0.);
                ZMass_Zexc0jet->Fill(Z.M(), weight);
                ZPt_Zexc0jet->Fill(Z.Pt(), weight);
                ZRapidity_Zexc0jet->Fill(Z.Rapidity(), weight);
                ZEta_Zexc0jet->Fill(Z.Eta(), weight);
                lepPt_Zexc0jet->Fill(lepton1.pt, weight);
                lepEta_Zexc0jet->Fill(lepton1.eta, weight);
                lepPhi_Zexc0jet->Fill(lepton1.phi, weight);
                if ( doZ || doTT){
                    lepPt_Zexc0jet->Fill(lepton2.pt, weight);
                    lepEta_Zexc0jet->Fill(lepton2.eta, weight);
                    lepPhi_Zexc0jet->Fill(lepton2.phi, weight);
                }
                dPhiLeptons_Zexc0jet->Fill(deltaPhi(lep1, lep2), weight);
                dEtaLeptons_Zexc0jet->Fill(lepton1.eta - lepton2.eta, weight);
                SpTLeptons_Zexc0jet->Fill(SpTsub(lep1, lep2), weight);
            }
            //==0 
            if (nGoodJets >= 1){
                ZNGoodJets_Zinc->Fill(1., weight);
                ZNGoodJetsFull_Zinc->Fill(1., weight);
                FirstJetPt_Zinc1jet->Fill(jets[0].pt, weight);
                FirstJetPt_1_Zinc1jet->Fill(jets[0].pt, weight);
                FirstJetPt_2_Zinc1jet->Fill(jets[0].pt, weight);
                FirstJetEta_Zinc1jet->Fill(fabs(jets[0].eta), weight);
                FirstJetEta_2_Zinc1jet->Fill(fabs(jets[0].eta), weight);
                FirstJetEtaFull_Zinc1jet->Fill(jets[0].eta, weight);
                JetsHT_Zinc1jet->Fill(jetsHT, weight);
                JetsHT_1_Zinc1jet->Fill(jetsHT, weight);
                JetsHT_2_Zinc1jet->Fill(jetsHT, weight);
                //*************************************** begin edit *************************************************************//
                dPhiLepJet1_Zinc1jet->Fill(deltaPhi(lep1, newLeadJ), weight);
                dPhiLepJet1_2_Zinc1jet->Fill(deltaPhi(lep1, newLeadJ), weight);
                //cout << deltaPhi(lep1, newLeadJ) << endl;
                MeanNJetsHT_1D_Zinc1jet->Fill(jetsHT, weight*nGoodJets);
                MeanNJetsHT_Zinc1jet->Fill(jetsHT, nGoodJets, weight);
                //---
                FirstJetRapidity_Zinc1jet->Fill(fabs(newLeadJ.Rapidity()), weight);
                FirstJetRapidityFull_Zinc1jet->Fill(newLeadJ.Rapidity(), weight);
                FirstJetmass_Zinc1jet->Fill(newLeadJ.M(), weight);
                FirstJetmass_1_Zinc1jet->Fill(newLeadJ.M(), weight);
                //*************************************** end edit ***************************************************************//
                
                ZNGoodJets_Zinc_NoWeight->Fill(1.);
                ZMass_Zinc1jet->Fill(Z.M(), weight);
                MET_Zinc1jet->Fill(METpt, weight);
                METphi_Zinc1jet->Fill(METphi, weight);
                MT_Zinc1jet->Fill(MT, weight);
                ZPt_Zinc1jet->Fill(Z.Pt(), weight);
                ZRapidity_Zinc1jet->Fill(Z.Rapidity(), weight);
                ZEta_Zinc1jet->Fill(Z.Eta(), weight);
                lepPt_Zinc1jet->Fill(lepton1.pt, weight);
                lepEta_Zinc1jet->Fill(lepton1.eta, weight);
                lepPhi_Zinc1jet->Fill(lepton1.phi, weight);
                if (doZ || doTT){
                    lepPt_Zinc1jet->Fill(lepton2.pt, weight);
                    lepEta_Zinc1jet->Fill(lepton2.eta, weight);
                    lepPhi_Zinc1jet->Fill(lepton2.phi, weight);
                }
                dPhiLeptons_Zinc1jet->Fill(deltaPhi(lep1, lep2), weight);
                dEtaLeptons_Zinc1jet->Fill(lepton1.eta - lepton2.eta, weight);
                dRLeptons_Zinc1jet->Fill(deltaR(lepton1.phi, lepton1.eta, lepton2.phi, lepton2.eta), weight);
                SpTLeptons_Zinc1jet->Fill(SpTsub(lep1, lep2), weight);
                FirstHighestJetPt_Zinc1jet->Fill(jets[0].pt, weight);
                FirstJetPtEta_Zinc1jet->Fill(jets[0].pt, fabs(jets[0].eta), weight);
                FirstJetPhi_Zinc1jet->Fill(jets[0].phi, weight);
                
                for (unsigned short j(0); j < nGoodJets; j++){
                    AllJetPt_Zinc1jet->Fill(jets[j].pt, weight);
                    AllJetEta_Zinc1jet->Fill(jets[j].eta, weight);
                    AllJetPhi_Zinc1jet->Fill(jets[j].phi, weight);
                }
                if ( doW ) dEtaBosonJet_Zinc1jet->Fill(fabs(jets[0].eta - lepton1.eta), weight);
                else dEtaBosonJet_Zinc1jet->Fill(fabs(jets[0].eta-Z.Eta()), weight);
                for ( int i =0 ; i < NbinsEta2D - 1 ; i++){
                    if ( fabs(jets[0].eta) >= j_Y_range[i] &&  fabs(jets[0].eta) < j_Y_range[i+1] )                                                FirstJetPt_Zinc1jet_Eta[i]->Fill(fabs(jets[0].pt), weight);
                }
                
                if (nGoodJets == 1){
                    // compute Delta pt between Z and jets
                    if (Z.Pt() > 0.8 * RecJetPtMin  && jets[0].pt/Z.Pt() < 1.2 && jets[0].pt/Z.Pt() > 0.8 && deltaPhi(leadJ, Z) > 2.7){
                        hPtEtaBackJet_Zexc1jet->Fill(leadJ.Pt(), leadJ.Eta(), weight);
                        if (patJetPfAk05jetpuMVA_->at(jets[0].patIndex) > 0 ) hPtEtaBackJetMVA_Zexc1jet->Fill(leadJ.Pt(), leadJ.Eta(), weight);
                    }
                    
                    nEventsExcl1Jets++;
                    ZNGoodJets_Zexc_NoWeight->Fill(1.);
                    ZMass_Zexc1jet->Fill(Z.M(), weight);
                    ZPt_Zexc1jet->Fill(Z.Pt(), weight);
                    ZRapidity_Zexc1jet->Fill(Z.Rapidity(), weight);
                    ZEta_Zexc1jet->Fill(Z.Eta(), weight);
                    lepPt_Zexc1jet->Fill(lepton1.pt, weight);
                    lepEta_Zexc1jet->Fill(lepton1.eta, weight);
                    lepPhi_Zexc1jet->Fill(lepton1.phi, weight);
                    if (doZ || doTT){
                        lepPt_Zexc1jet->Fill(lepton2.pt, weight);
                        lepEta_Zexc1jet->Fill(lepton2.eta, weight);
                        lepPhi_Zexc1jet->Fill(lepton2.phi, weight);
                    }
                    dPhiLeptons_Zexc1jet->Fill(deltaPhi(lep1, lep2), weight);
                    dEtaLeptons_Zexc1jet->Fill(lepton1.eta - lepton2.eta, weight);
                    SpTLeptons_Zexc1jet->Fill(SpTsub(lep1, lep2), weight);
                    FirstJetPt_Zexc1jet->Fill(jets[0].pt, weight);
                    FirstJetEta_Zexc1jet->Fill(jets[0].eta, weight);
                    FirstJetPhi_Zexc1jet->Fill(jets[0].phi, weight);
                    if ( doW ) dEtaBosonJet_Zexc1jet->Fill(fabs(jets[0].eta - lepton1.eta), weight);
                    else dEtaBosonJet_Zexc1jet->Fill(fabs(jets[0].eta-Z.Eta()), weight);
                    
                }
            }
            //==1 
            if (nGoodJets >= 2){
                TLorentzVector jet1Plus2PlusZ = jet1Plus2 + Z;
                
                ZNGoodJets_Zinc->Fill(2., weight);
                ZNGoodJetsFull_Zinc->Fill(2., weight);
                MET_Zinc2jet->Fill(METpt, weight);
                MT_Zinc2jet->Fill(MT, weight);
                SecondJetPt_Zinc2jet->Fill(jets[1].pt, weight);
                SecondJetPt_1_Zinc2jet->Fill(jets[1].pt, weight);
                SecondJetPt_2_Zinc2jet->Fill(jets[1].pt, weight);
                SecondJetEta_Zinc2jet->Fill(fabs(jets[1].eta), weight);
                SecondJetEta_2_Zinc2jet->Fill(fabs(jets[1].eta), weight);
                SecondJetEtaFull_Zinc2jet->Fill(jets[1].eta, weight);
                JetsHT_Zinc2jet->Fill(jetsHT, weight);
                JetsHT_1_Zinc2jet->Fill(jetsHT, weight);
                JetsHT_2_Zinc2jet->Fill(jetsHT, weight);
                //*************************************** begin edit *************************************************************//
                dRapidityJets_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight);
                dRapidityJets_2_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight);
                
                dRapidityJetsFB_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight);
                dRapidityJetsFB_2_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight);
                
                diJetMass_Zinc2jet->Fill(jet1Plus2.M(), weight);
                diJetMass_2_Zinc2jet->Fill(jet1Plus2.M(), weight);
                
                dPhiJets_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                dPhiJets_2_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                
                dPhiJetsFB_Zinc2jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), weight);
                dPhiJetsFB_2_Zinc2jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), weight);
                
                dRJets_Zinc2jet->Fill(deltaRYPhi(newLeadJ, newSecondJ), weight);
                dRJets_2_Zinc2jet->Fill(deltaRYPhi(newLeadJ, newSecondJ), weight);
                
                diJetPt_Zinc2jet->Fill(jet1Plus2.Pt(), weight);
                diJetPt_2_Zinc2jet->Fill(jet1Plus2.Pt(), weight);
            
                dPhiLepJet2_Zinc2jet->Fill(deltaPhi(lep1, newSecondJ), weight);
                dPhiLepJet2_2_Zinc2jet->Fill(deltaPhi(lep1, newSecondJ), weight);
                
                MeanNJetsHT_Zinc2jet->Fill(jetsHT, nGoodJets, weight);
                MeanNJetsdRapidity_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), nGoodJets, weight);
                MeanNJetsdRapidityFB_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, nGoodJets, weight);
                //---
                SecondJetRapidity_Zinc2jet->Fill(fabs(newSecondJ.Rapidity()), weight);
                SecondJetRapidityFull_Zinc2jet->Fill(newSecondJ.Rapidity(), weight);
                SecondJetmass_Zinc2jet->Fill(newSecondJ.M(), weight);
                SecondJetmass_1_Zinc2jet->Fill(newSecondJ.M(), weight);
                MeanNJetsHT_1D_Zinc2jet->Fill(jetsHT, weight*nGoodJets);
                MeanNJetsdRapidity_1D_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight*nGoodJets);
                MeanNJetsdRapidityFB_1D_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight*nGoodJets);
                //*************************************** end edit ***************************************************************//
                
                ZNGoodJets_Zinc_NoWeight->Fill(2.);
                ZMass_Zinc2jet->Fill(Z.M(), weight);
                METphi_Zinc2jet->Fill(METphi, weight);
                TwoJetsPtDiff_Zinc2jet->Fill(jet1Minus2.Pt(), weight);
                BestTwoJetsPtDiff_Zinc2jet->Fill(bestJet1Minus2.Pt(), weight);
                ZPt_Zinc2jet->Fill(Z.Pt(), weight);
                ZRapidity_Zinc2jet->Fill(Z.Rapidity(), weight);
                ZEta_Zinc2jet->Fill(Z.Eta(), weight);
                lepPt_Zinc2jet->Fill(lepton1.pt, weight);
                lepEta_Zinc2jet->Fill(lepton1.eta, weight);
                lepPhi_Zinc2jet->Fill(lepton1.phi, weight);
                JetsMass_Zinc2jet->Fill(jet1Plus2.M(), weight);
                if (doZ || doTT){
                    lepPt_Zinc2jet->Fill(lepton2.pt, weight);
                    lepEta_Zinc2jet->Fill(lepton2.eta, weight);
                    lepPhi_Zinc2jet->Fill(lepton2.phi, weight);
                }
                dPhiLeptons_Zinc2jet->Fill(deltaPhi(lep1, lep2), weight);
                dEtaLeptons_Zinc2jet->Fill(lepton1.eta - lepton2.eta, weight);
                dRLeptons_Zinc2jet->Fill(deltaR(lepton1.phi, lepton1.eta, lepton2.phi, lepton2.eta), weight);
                SpTLeptons_Zinc2jet->Fill(SpTsub(lep1, lep2), weight);
                FirstHighestJetPt_Zinc2jet->Fill(jets[0].pt, weight);
                SecondHighestJetPt_Zinc2jet->Fill(jets[1].pt, weight);
                SecondJetPtEta_Zinc2jet->Fill(jets[1].pt, fabs(jets[1].eta), weight);
                RatioJetPt21_Zinc2jet->Fill(jets[1].pt/jets[0].pt, weight);
                SecondJetPhi_Zinc2jet->Fill(jets[1].phi, weight);
                ptBal_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                BestdPhiJets_Zinc2jet->Fill(deltaPhi(bestTwoJets.first, bestTwoJets.second), weight);
                dEtaJets_Zinc2jet->Fill(leadJ.Eta() - secondJ.Eta(), weight);
                dEtaFirstJetZ_Zinc2jet->Fill(leadJ.Eta() - Z.Eta(), weight);
                dEtaSecondJetZ_Zinc2jet->Fill(secondJ.Eta() - Z.Eta(), weight);
                dEtaJet1Plus2Z_Zinc2jet->Fill(jet1Plus2.Eta() - Z.Eta(), weight);
                PHI_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                BestPHI_Zinc2jet->Fill(PHI(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                PHI_T_Zinc2jet->Fill(PHI_T(lep1, lep2, leadJ, secondJ), weight);
                BestPHI_T_Zinc2jet->Fill(PHI_T(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                SpT_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                BestSpT_Zinc2jet->Fill(SpT(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                SpTJets_Zinc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                BestSpTJets_Zinc2jet->Fill(SpTsub(bestTwoJets.first, bestTwoJets.second), weight);
                SPhi_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                BestSPhi_Zinc2jet->Fill(SPhi(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                
                
                for ( int i =0 ; i < NbinsEta2D - 1 ; i++){
                    if ( fabs(jets[1].eta) >= j_Y_range[i] &&  fabs(jets[1].eta) < j_Y_range[i+1]                                   )                                                SecondJetPt_Zinc2jet_Eta[i]->Fill(fabs(jets[0].pt), weight);
                }
                //--- V + 2 jets EWK histograms
                if (passesEWKJetPt){
                    // met histos
                    METEWK_Zinc2jet->Fill(METpt, weight);
                    METphiEWK_Zinc2jet->Fill(METphi,  weight);
                    MTEWK_Zinc2jet->Fill(MT,  weight);
                    
                    // jet histos
                    ZNGoodJetsEWK_Zexc->Fill(nGoodJets, weight);
                    FirstJetPtEWK_Zinc2jet->Fill(jets[0].pt, weight);
                    FirstJetEtaEWK_Zinc2jet->Fill(jets[0].eta, weight);
                    FirstJetPhiEWK_Zinc2jet->Fill(jets[0].phi, weight);
                    
                    
                    SecondJetPtEWK_Zinc2jet->Fill(jets[1].pt, weight);
                    SecondJetEtaEWK_Zinc2jet->Fill(jets[1].eta, weight);
                    SecondJetPhiEWK_Zinc2jet->Fill(jets[1].phi, weight);
                    
                    int temIND (0), temIND1(1);
                    if (fabs(jets[1].eta) > fabs(jets[0].eta)){
                        temIND = 1;
                        temIND1 = 0;
                    }
                    ForwardJetPtEWK_Zinc2jet->Fill(jets[temIND].pt, weight);
                    ForwardJetEtaEWK_Zinc2jet->Fill(jets[temIND].eta, weight);
                    ForwardJetPhiEWK_Zinc2jet->Fill(jets[temIND].phi, weight);
                    
                    CentralJetPtEWK_Zinc2jet->Fill(jets[temIND1].pt, weight);
                    CentralJetEtaEWK_Zinc2jet->Fill(jets[temIND1].eta, weight);
                    CentralJetPhiEWK_Zinc2jet->Fill(jets[temIND1].phi, weight);
                    
                    JetsHTEWK_Zinc2jet->Fill(jetsHT, weight);
                    JetsMassEWK_Zinc2jet->Fill(jet1Plus2.M(), weight);
                    
                    // multi jet variables
                    SumEtaJetsEWK_Zinc2jet->Fill(fabs(leadJ.Eta() + secondJ.Eta()),  weight);
                    AbsSumEtaJetsEWK_Zinc2jet->Fill(fabs(leadJ.Eta()) + fabs(secondJ.Eta()),  weight);
                    TwoJetsPtDiffEWK_Zinc2jet->Fill(jet1Minus2.Pt(), weight);
                    ptBalEWK_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                    dPhiJetsEWK_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                    dEtaJetsEWK_Zinc2jet->Fill(fabs(leadJ.Eta() - secondJ.Eta()), weight);
                    SpTJetsEWK_Zinc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                    dPhiJetsEWK_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                    
                    // find jet properties of the third jet that is between the two leading jets
                    int nGoodJetsBtw(0.);
                    double jetsHTBtw(0.);
                    if (nJetsAdd > 2){
                        ThirdJetPtEWKadd_Zinc2jet->Fill(jetsAdditional[2].pt, weight);
                        ThirdJetEtaEWKadd_Zinc2jet->Fill(jetsAdditional[2].eta, weight);
                        ThirdJetPhiEWKadd_Zinc2jet->Fill(jetsAdditional[2].phi, weight);
                        
                        for (unsigned short i(2); i < nJetsAdd; i++){
                            int coutBtw = 0 ;
                            if (jetsAdditional[i].eta < max(jets[0].eta,jets[1].eta) - 0.5
                                && jetsAdditional[i].eta > min(jets[0].eta,jets[1].eta) + 0.5){
                                jetsHTBtw += jetsAdditional[i].pt ;
                                nGoodJetsBtw++;
                                AllJetPtEWKbtw_Zinc2jet->Fill(jetsAdditional[i].pt, weight);
                                AllJetEtaEWKbtw_Zinc2jet->Fill(jetsAdditional[i].eta, weight);
                                AllJetPhiEWKbtw_Zinc2jet->Fill(jetsAdditional[i].phi, weight);
                                if (coutBtw == 0){
                                    ThirdJetPtEWKbtw_Zinc2jet->Fill(jetsAdditional[i].pt, weight);
                                    ThirdJetEtaEWKbtw_Zinc2jet->Fill(jetsAdditional[i].eta, weight);
                                    ThirdJetPhiEWKbtw_Zinc2jet->Fill(jetsAdditional[i].phi, weight);
                                    coutBtw = 1;
                                }
                            }
                        }
                    }
                    ZNGoodJetsEWKbtw_Zexc->Fill(nGoodJetsBtw, weight);
                    JetsHTEWKbtw_Zinc2jet->Fill(jetsHTBtw, weight);
                    
                    // at least one forward jet
                    if (passesEWKJetFwdEta){
                        METEWKfwd_Zinc2jet->Fill(METpt,  weight);
                        METphiEWKfwd_Zinc2jet->Fill(METphi,  weight);
                        MTEWKfwd_Zinc2jet->Fill(MT,  weight);
                        
                        // jet histos
                        ZNGoodJetsEWKfwd_Zexc->Fill(nGoodJets, weight);
                        FirstJetPtEWKfwd_Zinc2jet->Fill(jets[0].pt, weight);
                        FirstJetEtaEWKfwd_Zinc2jet->Fill(jets[0].eta, weight);
                        FirstJetPhiEWKfwd_Zinc2jet->Fill(jets[0].phi, weight);
                        
                        SecondJetPtEWKfwd_Zinc2jet->Fill(jets[1].pt, weight);
                        SecondJetEtaEWKfwd_Zinc2jet->Fill(jets[1].eta, weight);
                        SecondJetPhiEWKfwd_Zinc2jet->Fill(jets[1].phi, weight);
                        
                        JetsHTEWKfwd_Zinc2jet->Fill(jetsHT, weight);
                        JetsMassEWKfwd_Zinc2jet->Fill(jet1Plus2.M(), weight);
                        
                        // multi jet variables
                        SumEtaJetsEWKfwd_Zinc2jet->Fill(leadJ.Eta() + secondJ.Eta(), weight);
                        TwoJetsPtDiffEWKfwd_Zinc2jet->Fill(jet1Minus2.Pt(), weight);
                        ptBalEWKfwd_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                        dPhiJetsEWKfwd_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                        dEtaJetsEWKfwd_Zinc2jet->Fill(fabs(leadJ.Eta() - secondJ.Eta()), weight);
                        SpTJetsEWKfwd_Zinc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                        dPhiJetsEWKfwd_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                        
                        
                    }//--- end at least one forward jet
                    
                    if (jet1Plus2.M() > 1000.){
                        METEWKmjj_Zinc2jet->Fill(METpt,  weight);
                        METphiEWKmjj_Zinc2jet->Fill(METphi,  weight);
                        MTEWKmjj_Zinc2jet->Fill(MT,  weight);
                        short nGoodJetsAdd(nJetsAdd-2);
                        double jetsHTAdd(0);
                        // jet histos
                        ZNGoodJetsEWKmjj_Zexc->Fill(nGoodJetsAdd, weight);
                        FirstJetPtEWKmjj_Zinc2jet->Fill(jets[0].pt, weight);
                        FirstJetEtaEWKmjj_Zinc2jet->Fill(jets[0].eta, weight);
                        FirstJetPhiEWKmjj_Zinc2jet->Fill(jets[0].phi, weight);
                        
                        
                        SecondJetPtEWKmjj_Zinc2jet->Fill(jets[1].pt, weight);
                        SecondJetEtaEWKmjj_Zinc2jet->Fill(jets[1].eta, weight);
                        SecondJetPhiEWKmjj_Zinc2jet->Fill(jets[1].phi, weight);
                        
                        JetsHTEWKmjj_Zinc2jet->Fill(jetsHT, weight);
                        JetsMassEWKmjj_Zinc2jet->Fill(jet1Plus2.M(), weight);
                        
                        // multi jet variables
                        SumEtaJetsEWKmjj_Zinc2jet->Fill(leadJ.Eta() + secondJ.Eta(),  weight);
                        TwoJetsPtDiffEWKmjj_Zinc2jet->Fill(jet1Minus2.Pt(), weight);
                        ptBalEWKmjj_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                        dPhiJetsEWKmjj_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                        dEtaJetsEWKmjj_Zinc2jet->Fill(leadJ.Eta() - secondJ.Eta(), weight);
                        SpTJetsEWKmjj_Zinc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                        dPhiJetsEWKmjj_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                        
                        // find jet properties of the third jet that is  between the two leading jets
                        int nGoodJetsmjjBtw(0.);
                        if (nJetsAdd > 2){
                            double jetsHTmjjBtw(0.);
                            for (unsigned short i(2); i < nJetsAdd; i++){
                                if (jetsAdditional[i].eta < max(jets[0].eta,jets[1].eta) - 0.5
                                    && jetsAdditional[i].eta > min(jets[0].eta,jets[1].eta) + 0.5 ){
                                    jetsHTmjjBtw += jetsAdditional[i].pt ;
                                    nGoodJetsmjjBtw++;
                                }
                                jetsHTAdd += jetsAdditional[i].pt ;
                            }
                            
                            ThirdJetPtEWKmjj_Zinc3jet->Fill(jetsAdditional[2].pt, weight);
                            JetsHTEWKmjjAdd_Zinc2jet->Fill(jetsHTAdd, weight);
                            TLorentzVector thirdJAdd;
                            thirdJAdd.SetPtEtaPhiE(jetsAdditional[2].pt, jetsAdditional[2].eta, jetsAdditional[2].phi, jetsAdditional[2].energy);
                            double tempRapidiy3Jet = thirdJAdd.Rapidity() - 0.5 * (leadJ.Rapidity() + secondJ.Rapidity());
                            ThirdJetEtaEWKmjj_Zinc3jet->Fill(tempRapidiy3Jet, weight);
                        }
                    }//--- end dijet mass > 1000
                    
                    //--- higher jet properties
                    if (nGoodJets > 2){
                        METEWK_Zinc3jet->Fill(METpt, weight);
                        METphiEWK_Zinc3jet->Fill(METphi, weight);
                        MTEWK_Zinc3jet->Fill(MT, weight);
                        
                        TLorentzVector thirdJ;
                        thirdJ.SetPtEtaPhiE(jets[2].pt, jets[2].eta, jets[2].phi, jets[2].energy);
                        double tempRapidiy3Jet = thirdJ.Rapidity() - 0.5 * (leadJ.Rapidity() + secondJ.Rapidity());
                        EtaThirdJetsEWK_Zinc3jet->Fill(tempRapidiy3Jet, weight);
                    }
                }
                
                for (unsigned short j(0); j < nGoodJets; j++){
                    AllJetPt_Zinc2jet->Fill(jets[j].pt, weight);
                    AllJetEta_Zinc2jet->Fill(jets[j].eta, weight);
                    AllJetPhi_Zinc2jet->Fill(jets[j].phi, weight);
                }
                if (Z.Pt() < 25){
                    ptBal_LowPt_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                    dPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                    BestdPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(bestTwoJets.first, bestTwoJets.second), weight);
                    dPhiLeptons_LowPt_Zinc2jet->Fill(deltaPhi(lep1, lep2), weight);
                    PHI_LowPt_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                    BestPHI_LowPt_Zinc2jet->Fill(PHI(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                    PHI_T_LowPt_Zinc2jet->Fill(PHI_T(lep1, lep2, leadJ, secondJ), weight);
                    BestPHI_T_LowPt_Zinc2jet->Fill(PHI_T(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                    SpT_LowPt_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    BestSpT_LowPt_Zinc2jet->Fill(SpT(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                    SpTJets_LowPt_Zinc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                    BestSpTJets_LowPt_Zinc2jet->Fill(SpTsub(bestTwoJets.first, bestTwoJets.second), weight);
                    SpTLeptons_LowPt_Zinc2jet->Fill(SpTsub(lep1, lep2), weight);
                    SPhi_LowPt_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    BestSPhi_LowPt_Zinc2jet->Fill(SPhi(lep1, lep2, bestTwoJets.first, bestTwoJets.second), weight);
                    if (SpT(lep1, lep2, leadJ, secondJ) < 0.5){
                        PHI_LowSpT_LowPt_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        SPhi_LowSpT_LowPt_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    }
                    else {
                        PHI_HighSpT_LowPt_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        SPhi_HighSpT_LowPt_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    }
                    if (SPhi(lep1, lep2, leadJ, secondJ) < 0.5){
                        SpT_LowSPhi_LowPt_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    }
                    else {
                        SpT_HighSPhi_LowPt_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    }
                }
                else {
                    ptBal_HighPt_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(),weight);
                    dPhiJets_HighPt_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                    dPhiLeptons_HighPt_Zinc2jet->Fill(deltaPhi(lep1, lep2), weight);
                    PHI_HighPt_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                    PHI_T_HighPt_Zinc2jet->Fill(PHI_T(lep1, lep2, leadJ, secondJ), weight);
                    SpT_HighPt_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    SpTJets_HighPt_Zinc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                    SpTLeptons_HighPt_Zinc2jet->Fill(SpTsub(lep1, lep2), weight);
                    SPhi_HighPt_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    if (SpT(lep1, lep2, leadJ, secondJ) < 0.5){
                        PHI_LowSpT_HighPt_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        SPhi_LowSpT_HighPt_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    }
                    else {
                        PHI_HighSpT_HighPt_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        SPhi_HighSpT_HighPt_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    }
                    if (SPhi(lep1, lep2, leadJ, secondJ) < 0.5){
                        SpT_LowSPhi_HighPt_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    }
                    else {
                        SpT_HighSPhi_HighPt_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    }
                }
                if (SPhi(lep1, lep2, leadJ, secondJ) < 0.5){
                    SpT_LowSPhi_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                }
                else {
                    SpT_HighSPhi_Zinc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                }
                if (SpT(lep1, lep2, leadJ, secondJ) < 0.5){
                    PHI_LowSpT_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                    SPhi_LowSpT_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                }
                else {
                    PHI_HighSpT_Zinc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                    SPhi_HighSpT_Zinc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                }
                if (nGoodJets == 2){
                    nEventsExcl2Jets++;
                    ZNGoodJets_Zexc_NoWeight->Fill(2.);
                    ZMass_Zexc2jet->Fill(Z.M(), weight);
                    ZPt_Zexc2jet->Fill(Z.Pt(), weight);
                    ZRapidity_Zexc2jet->Fill(Z.Rapidity(), weight);
                    ZEta_Zexc2jet->Fill(Z.Eta(), weight);
                    lepPt_Zexc2jet->Fill(lepton1.pt, weight);
                    lepEta_Zexc2jet->Fill(lepton1.eta, weight);
                    lepPhi_Zexc2jet->Fill(lepton1.phi, weight);
                    if (doZ || doTT){
                        lepPt_Zexc2jet->Fill(lepton2.pt, weight);
                        lepEta_Zexc2jet->Fill(lepton2.eta, weight);
                        lepPhi_Zexc2jet->Fill(lepton2.phi, weight);
                    }
                    dPhiLeptons_Zexc2jet->Fill(deltaPhi(lep1, lep2), weight);
                    dEtaLeptons_Zexc2jet->Fill(lepton1.eta - lepton2.eta, weight);
                    SpTLeptons_Zexc2jet->Fill(SpTsub(lep1, lep2), weight);
                    SecondJetPt_Zexc2jet->Fill(jets[1].pt, weight);
                    SecondJetEta_Zexc2jet->Fill(jets[1].eta, weight);
                    SecondJetPhi_Zexc2jet->Fill(jets[1].phi, weight);
                    
                    //-- DPS Histograms
                    TwoJetsPtDiff_Zexc2jet->Fill(jet1Minus2.Pt(), weight);
                    JetsMass_Zexc2jet->Fill(jet1Plus2.M(), weight);
                    ptBal_Zexc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                    dPhiJets_Zexc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                    dEtaJets_Zexc2jet->Fill(leadJ.Eta() - secondJ.Eta(), weight);
                    dEtaFirstJetZ_Zexc2jet->Fill(leadJ.Eta() - Z.Eta(), weight);
                    dEtaSecondJetZ_Zexc2jet->Fill(secondJ.Eta() - Z.Eta(), weight);
                    dEtaJet1Plus2Z_Zexc2jet->Fill(jet1Plus2.Eta() - Z.Eta(), weight);
                    PHI_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                    PHI_T_Zexc2jet->Fill(PHI_T(lep1, lep2, leadJ, secondJ), weight);
                    SpT_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    SpTJets_Zexc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                    SPhi_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    if (Z.Pt() < 25){
                        ptBal_LowPt_Zexc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                        dPhiJets_LowPt_Zexc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                        dPhiLeptons_LowPt_Zexc2jet->Fill(deltaPhi(lep1, lep2), weight);
                        PHI_LowPt_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        PHI_T_LowPt_Zexc2jet->Fill(PHI_T(lep1, lep2, leadJ, secondJ), weight);
                        SpT_LowPt_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                        SpTJets_LowPt_Zexc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                        SpTLeptons_LowPt_Zexc2jet->Fill(SpTsub(lep1, lep2), weight);
                        SPhi_LowPt_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                        if (SpT(lep1, lep2, leadJ, secondJ) < 0.5){
                            PHI_LowSpT_LowPt_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                            SPhi_LowSpT_LowPt_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                        }
                        else {
                            PHI_HighSpT_LowPt_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                            SPhi_HighSpT_LowPt_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                        }
                        if (SPhi(lep1, lep2, leadJ, secondJ) < 0.5){
                            SpT_LowSPhi_LowPt_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                        }
                        else {
                            SpT_HighSPhi_LowPt_Zexc2jet ->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                        }
                    }
                    else {
                        ptBal_HighPt_Zexc2jet->Fill(jet1Plus2PlusZ.Pt(),weight);
                        dPhiJets_HighPt_Zexc2jet->Fill(deltaPhi(leadJ, secondJ), weight);
                        dPhiLeptons_HighPt_Zexc2jet->Fill(deltaPhi(lep1, lep2), weight);
                        PHI_HighPt_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        PHI_T_HighPt_Zexc2jet->Fill(PHI_T(lep1, lep2, leadJ, secondJ), weight);
                        SpT_HighPt_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                        SpTJets_HighPt_Zexc2jet->Fill(SpTsub(leadJ, secondJ), weight);
                        SpTLeptons_HighPt_Zexc2jet->Fill(SpTsub(lep1, lep2), weight);
                        SPhi_HighPt_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                        if (SpT(lep1, lep2, leadJ, secondJ) < 0.5){
                            PHI_LowSpT_HighPt_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                            SPhi_LowSpT_HighPt_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                        }
                        else {
                            PHI_HighSpT_HighPt_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                            SPhi_HighSpT_HighPt_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                        }
                        if (SPhi(lep1, lep2, leadJ, secondJ) < 0.5){
                            SpT_LowSPhi_HighPt_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                        }
                        else {
                            SpT_HighSPhi_HighPt_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                        }
                    }
                    if (SPhi(lep1, lep2, leadJ, secondJ) < 0.5){
                        SpT_LowSPhi_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    }
                    else {
                        SpT_HighSPhi_Zexc2jet->Fill(SpT(lep1, lep2, leadJ, secondJ), weight);
                    }
                    if (SpT(lep1, lep2, leadJ, secondJ) < 0.5){
                        PHI_LowSpT_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        SPhi_LowSpT_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    }
                    else {
                        PHI_HighSpT_Zexc2jet->Fill(PHI(lep1, lep2, leadJ, secondJ), weight);
                        SPhi_HighSpT_Zexc2jet->Fill(SPhi(lep1, lep2, leadJ, secondJ), weight);
                    }
                }
            }
            //==2 
            if (nGoodJets >= 3) {
                ZNGoodJets_Zinc->Fill(3., weight);
                ZNGoodJetsFull_Zinc->Fill(3., weight);
                ThirdJetPt_Zinc3jet->Fill(jets[2].pt, weight);
                ThirdJetPt_1_Zinc3jet->Fill(jets[2].pt, weight);
                ThirdJetPt_2_Zinc3jet->Fill(jets[2].pt, weight);
                ThirdJetEta_Zinc3jet->Fill(fabs(jets[2].eta), weight);
                ThirdJetEta_2_Zinc3jet->Fill(fabs(jets[2].eta), weight);
                ThirdJetEtaFull_Zinc3jet->Fill(jets[2].eta, weight);
                JetsHT_Zinc3jet->Fill(jetsHT, weight);
                JetsHT_1_Zinc3jet->Fill(jetsHT, weight);
                JetsHT_2_Zinc3jet->Fill(jetsHT, weight);
                //*************************************** begin edit *************************************************************//
                
                dRapidityJets_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight);
                dRapidityJets_2_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight);
                
                dRapidityJets_First_Third_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), weight);
                dRapidityJets_2_First_Third_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), weight);
                
                dRapidityJets_Second_Third_Zinc3jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), weight);
                dRapidityJets_2_Second_Third_Zinc3jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), weight);
                
                dRapidityJetsFB_Zinc3jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight);
                dRapidityJetsFB_2_Zinc3jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight);
                
                dPhiJets_Zinc3jet->Fill(deltaPhi(leadJ, secondJ), weight);
                dPhiJets_2_Zinc3jet->Fill(deltaPhi(leadJ, secondJ), weight);
                
                dPhiJetsFB_Zinc3jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), weight);
                dPhiJetsFB_2_Zinc3jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), weight);
                
                diJetMass_Zinc3jet->Fill(jet1Plus2.M(), weight);
                diJetMass_2_Zinc3jet->Fill(jet1Plus2.M(), weight);
                
                diJetPt_Zinc3jet->Fill(jet1Plus2.Pt(), weight);
                diJetPt_2_Zinc3jet->Fill(jet1Plus2.Pt(), weight);
                
                dPhiLepJet3_Zinc3jet->Fill(deltaPhi(lep1, newThirdJ), weight);
                dPhiLepJet3_2_Zinc3jet->Fill(deltaPhi(lep1, newThirdJ), weight);
                //---
                ThirdJetRapidity_Zinc3jet->Fill(fabs(newThirdJ.Rapidity()), weight);
                ThirdJetRapidityFull_Zinc3jet->Fill(newThirdJ.Rapidity(), weight);
                ThirdJetmass_Zinc3jet->Fill(newThirdJ.M(), weight);
                ThirdJetmass_1_Zinc3jet->Fill(newThirdJ.M(), weight);
                //*************************************** end edit ***************************************************************//
                
                ZNGoodJets_Zinc_NoWeight->Fill(3.);
                ZMass_Zinc3jet->Fill(Z.M(), weight);
                MET_Zinc3jet->Fill(METpt, weight);
                METphi_Zinc3jet->Fill(METphi, weight);
                MT_Zinc3jet->Fill(MT, weight);
                ZPt_Zinc3jet->Fill(Z.Pt(), weight);
                ZRapidity_Zinc3jet->Fill(Z.Rapidity(), weight);
                ZEta_Zinc3jet->Fill(Z.Eta(), weight);
                lepPt_Zinc3jet->Fill(lepton1.pt, weight);
                lepEta_Zinc3jet->Fill(lepton1.eta, weight);
                lepPhi_Zinc3jet->Fill(lepton1.phi, weight);
                if ( doZ || doTT){
                    lepPt_Zinc3jet->Fill(lepton2.pt, weight);
                    lepEta_Zinc3jet->Fill(lepton2.eta, weight);
                    lepPhi_Zinc3jet->Fill(lepton2.phi, weight);
                }
                dPhiLeptons_Zinc3jet->Fill(deltaPhi(lep1, lep2), weight);
                dEtaLeptons_Zinc3jet->Fill(lepton1.eta - lepton2.eta, weight);
                dRLeptons_Zinc3jet->Fill(deltaR(lepton1.phi, lepton1.eta, lepton2.phi, lepton2.eta), weight);
                SpTLeptons_Zinc3jet->Fill(SpTsub(lep1, lep2), weight);
                FirstHighestJetPt_Zinc3jet->Fill(jets[0].pt, weight);
                SecondHighestJetPt_Zinc3jet->Fill(jets[1].pt, weight);
                ThirdHighestJetPt_Zinc3jet->Fill(jets[2].pt, weight);
                ThirdJetPtEta_Zinc3jet->Fill(jets[2].pt, fabs(jets[2].eta), weight);
                RatioJetPt32_Zinc3jet->Fill(jets[2].pt/jets[1].pt, weight);
                ThirdJetPhi_Zinc3jet->Fill(jets[2].phi, weight);
                
                for (unsigned short j(0); j < nGoodJets; j++){
                    AllJetPt_Zinc3jet->Fill(jets[j].pt, weight);
                    AllJetEta_Zinc3jet->Fill(jets[j].eta, weight);
                    AllJetPhi_Zinc3jet->Fill(jets[j].phi, weight);
                }
                if (nGoodJets == 3){
                    nEventsExcl3Jets++;
                    ZNGoodJets_Zexc_NoWeight->Fill(3.);
                    ZMass_Zexc3jet->Fill(Z.M(), weight);
                    ZPt_Zexc3jet->Fill(Z.Pt(), weight);
                    ZRapidity_Zexc3jet->Fill(Z.Rapidity(), weight);
                    ZEta_Zexc3jet->Fill(Z.Eta(), weight);
                    lepPt_Zexc3jet->Fill(lepton1.pt, weight);
                    lepEta_Zexc3jet->Fill(lepton1.eta, weight);
                    lepPhi_Zexc3jet->Fill(lepton1.phi, weight);
                    if (doZ || doTT){
                        lepPt_Zexc3jet->Fill(lepton2.pt, weight);
                        lepEta_Zexc3jet->Fill(lepton2.eta, weight);
                        lepPhi_Zexc3jet->Fill(lepton2.phi, weight);
                    }
                    dPhiLeptons_Zexc3jet->Fill(deltaPhi(lep1, lep2), weight);
                    dEtaLeptons_Zexc3jet->Fill(lepton1.eta - lepton2.eta, weight);
                    SpTLeptons_Zexc3jet->Fill(SpTsub(lep1, lep2), weight);
                }
            }
            //==3 
            if (nGoodJets >= 4){
                ZNGoodJets_Zinc->Fill(4., weight);
                ZNGoodJetsFull_Zinc->Fill(4., weight);
                FourthJetPt_Zinc4jet->Fill(jets[3].pt, weight);
                FourthJetPt_1_Zinc4jet->Fill(jets[3].pt, weight);
                FourthJetPt_2_Zinc4jet->Fill(jets[3].pt, weight);
                FourthJetEta_Zinc4jet->Fill(fabs(jets[3].eta), weight);
                FourthJetEta_2_Zinc4jet->Fill(fabs(jets[3].eta), weight);
                FourthJetEtaFull_Zinc4jet->Fill(jets[3].eta, weight);
                JetsHT_Zinc4jet->Fill(jetsHT, weight);
                JetsHT_1_Zinc4jet->Fill(jetsHT, weight);
                JetsHT_2_Zinc4jet->Fill(jetsHT, weight);
                //*************************************** begin edit *************************************************************//
                
                dRapidityJets_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight);
                dRapidityJets_2_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), weight);
                
                dRapidityJets_First_Third_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), weight);
                dRapidityJets_2_First_Third_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), weight);
                
                dRapidityJets_Second_Third_Zinc4jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), weight);
                dRapidityJets_2_Second_Third_Zinc4jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), weight);
                
                dRapidityJetsFB_Zinc4jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight);
                dRapidityJetsFB_2_Zinc4jet->Fill(ForwardJetRapidity - BackwardJetRapidity, weight);
                
                dPhiJets_Zinc4jet->Fill(deltaPhi(leadJ, secondJ), weight);
                dPhiJets_2_Zinc4jet->Fill(deltaPhi(leadJ, secondJ), weight);
                
                dPhiJetsFB_Zinc4jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), weight);
                dPhiJetsFB_2_Zinc4jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), weight);
                
                diJetMass_Zinc4jet->Fill(jet1Plus2.M(), weight);
                diJetMass_2_Zinc4jet->Fill(jet1Plus2.M(), weight);
                
                diJetPt_Zinc4jet->Fill(jet1Plus2.Pt(), weight);
                diJetPt_2_Zinc4jet->Fill(jet1Plus2.Pt(), weight);
                
                dPhiLepJet4_Zinc4jet->Fill(deltaPhi(lep1, newFourthJ), weight);
                dPhiLepJet4_2_Zinc4jet->Fill(deltaPhi(lep1, newFourthJ), weight);
                //---
                FourthJetRapidity_Zinc4jet->Fill(fabs(newFourthJ.Rapidity()), weight);
                FourthJetRapidityFull_Zinc4jet->Fill(newFourthJ.Rapidity(), weight);
                FourthJetmass_Zinc4jet->Fill(newFourthJ.M(), weight);
                FourthJetmass_1_Zinc4jet->Fill(newFourthJ.M(), weight);
                //*************************************** end edit ***************************************************************//
                
                ZNGoodJets_Zinc_NoWeight->Fill(4.);
                ZMass_Zinc4jet->Fill(Z.M(), weight);
                ZPt_Zinc4jet->Fill(Z.Pt(), weight);
                ZRapidity_Zinc4jet->Fill(Z.Rapidity(), weight);
                ZEta_Zinc4jet->Fill(Z.Eta(), weight);
                lepPt_Zinc4jet->Fill(lepton1.pt, weight);
                lepEta_Zinc4jet->Fill(lepton1.eta, weight);
                lepPhi_Zinc4jet->Fill(lepton1.phi, weight);
                if (doZ || doTT){
                    lepEta_Zinc4jet->Fill(lepton2.eta, weight);
                    lepPt_Zinc4jet->Fill(lepton2.pt, weight);
                    lepPhi_Zinc4jet->Fill(lepton2.phi, weight);
                }
                dPhiLeptons_Zinc4jet->Fill(deltaPhi(lep1, lep2), weight);
                dEtaLeptons_Zinc4jet->Fill(lepton1.eta - lepton2.eta, weight);
                dRLeptons_Zinc4jet->Fill(deltaR(lepton1.phi, lepton1.eta, lepton2.phi, lepton2.eta), weight);
                SpTLeptons_Zinc4jet->Fill(SpTsub(lep1, lep2), weight);
                FirstHighestJetPt_Zinc4jet->Fill(jets[0].pt, weight);
                SecondHighestJetPt_Zinc4jet->Fill(jets[1].pt, weight);
                ThirdHighestJetPt_Zinc4jet->Fill(jets[2].pt, weight);
                FourthJetPtEta_Zinc4jet->Fill(jets[3].pt, fabs(jets[3].eta), weight);
                FourthJetPhi_Zinc4jet->Fill(jets[3].phi, weight);
                
                for (unsigned short j(0); j < nGoodJets; j++){
                    AllJetPt_Zinc4jet->Fill(jets[j].pt, weight);
                    AllJetEta_Zinc4jet->Fill(jets[j].eta, weight);
                    AllJetPhi_Zinc4jet->Fill(jets[j].phi, weight);
                }
                if (nGoodJets == 4){
                    ZNGoodJets_Zexc_NoWeight->Fill(4.);
                    ZMass_Zexc4jet->Fill(Z.M(), weight);
                    ZPt_Zexc4jet->Fill(Z.Pt(), weight);
                    ZRapidity_Zexc4jet->Fill(Z.Rapidity(), weight);
                    ZEta_Zexc4jet->Fill(Z.Eta(), weight);
                    lepPt_Zexc4jet->Fill(lepton1.pt, weight);
                    lepEta_Zexc4jet->Fill(lepton1.eta, weight);
                    lepPhi_Zexc4jet->Fill(lepton1.phi, weight);
                    if (doZ || doTT){
                        lepPt_Zexc4jet->Fill(lepton2.pt, weight);
                        lepEta_Zexc4jet->Fill(lepton2.eta, weight);
                        lepPhi_Zexc4jet->Fill(lepton2.phi, weight);
                    }
                    dPhiLeptons_Zexc4jet->Fill(deltaPhi(lep1, lep2), weight);
                    dEtaLeptons_Zexc4jet->Fill(lepton1.eta - lepton2.eta, weight);
                    SpTLeptons_Zexc4jet->Fill(SpTsub(lep1, lep2), weight);
                }
            }
            //==4 
            if (nGoodJets >= 5){
                ZNGoodJets_Zinc->Fill(5., weight);
                ZNGoodJetsFull_Zinc->Fill(5., weight);
                FifthJetPt_Zinc5jet->Fill(jets[4].pt, weight);
                FifthJetPt_1_Zinc5jet->Fill(jets[4].pt, weight);
                FifthJetPt_2_Zinc5jet->Fill(jets[4].pt, weight);
                FifthJetEta_Zinc5jet->Fill(fabs(jets[4].eta), weight);
                FifthJetEta_2_Zinc5jet->Fill(fabs(jets[4].eta), weight);
                FifthJetEtaFull_Zinc5jet->Fill(jets[4].eta, weight);
                JetsHT_Zinc5jet->Fill(jetsHT, weight);
                JetsHT_1_Zinc5jet->Fill(jetsHT, weight);
                JetsHT_2_Zinc5jet->Fill(jetsHT, weight);
                //*************************************** begin edit *************************************************************//
                dPhiLepJet5_Zinc5jet->Fill(deltaPhi(lep1, newFifthJ), weight);
                dPhiLepJet5_2_Zinc5jet->Fill(deltaPhi(lep1, newFifthJ), weight);
                //*************************************** end edit ***************************************************************//
                
                ZNGoodJets_Zinc_NoWeight->Fill(5.);
                ZMass_Zinc5jet->Fill(Z.M(), weight);
                ZPt_Zinc5jet->Fill(Z.Pt(), weight);
                ZRapidity_Zinc5jet->Fill(Z.Rapidity(), weight);
                ZEta_Zinc5jet->Fill(Z.Eta(), weight);
                lepPt_Zinc5jet->Fill(lepton1.pt, weight);
                lepEta_Zinc5jet->Fill(lepton1.eta, weight);
                lepPhi_Zinc5jet->Fill(lepton1.phi, weight);
                if (doZ || doTT){
                    lepPt_Zinc5jet->Fill(lepton2.pt, weight);
                    lepEta_Zinc5jet->Fill(lepton2.eta, weight);
                    lepPhi_Zinc5jet->Fill(lepton2.phi, weight);
                }
                dPhiLeptons_Zinc5jet->Fill(deltaPhi(lep1, lep2), weight);
                dEtaLeptons_Zinc5jet->Fill(lepton1.eta - lepton2.eta, weight);
                dRLeptons_Zinc5jet->Fill(deltaR(lepton1.phi, lepton1.eta, lepton2.phi, lepton2.eta), weight);
                SpTLeptons_Zinc5jet->Fill(SpTsub(lep1, lep2), weight);
                FirstHighestJetPt_Zinc5jet->Fill(jets[0].pt, weight);
                SecondHighestJetPt_Zinc5jet->Fill(jets[1].pt, weight);
                ThirdHighestJetPt_Zinc5jet->Fill(jets[2].pt, weight);
                FifthJetPtEta_Zinc5jet->Fill(jets[4].pt, fabs(jets[4].eta), weight);
                FifthJetPhi_Zinc5jet->Fill(jets[4].phi, weight);
                
                if (nGoodJets == 5){
                    ZNGoodJets_Zexc_NoWeight->Fill(5.);
                    ZMass_Zexc5jet->Fill(Z.M(), weight);
                    ZPt_Zexc5jet->Fill(Z.Pt(), weight);
                    ZRapidity_Zexc5jet->Fill(Z.Rapidity(), weight);
                    ZEta_Zexc5jet->Fill(Z.Eta(), weight);
                    lepPt_Zexc5jet->Fill(lepton1.pt, weight);
                    lepEta_Zexc5jet->Fill(lepton1.eta, weight);
                    lepPhi_Zexc5jet->Fill(lepton1.phi, weight);
                    if (doZ || doTT){
                        lepPt_Zexc5jet->Fill(lepton2.pt, weight);
                        lepEta_Zexc5jet->Fill(lepton2.eta, weight);
                        lepPhi_Zexc5jet->Fill(lepton2.phi, weight);
                    }
                    dPhiLeptons_Zexc5jet->Fill(deltaPhi(lep1, lep2), weight);
                    dEtaLeptons_Zexc5jet->Fill(lepton1.eta - lepton2.eta, weight);
                    SpTLeptons_Zexc5jet->Fill(SpTsub(lep1, lep2), weight);
                }
            }
            //==5 
            if (nGoodJets >= 6){
                ZNGoodJets_Zinc->Fill(6., weight);
                ZNGoodJetsFull_Zinc->Fill(6., weight);
                SixthJetPt_Zinc6jet->Fill(jets[5].pt, weight);
                SixthJetPt_1_Zinc6jet->Fill(jets[5].pt, weight);
                SixthJetEta_Zinc6jet->Fill(fabs(jets[5].eta), weight);
                SixthJetEtaFull_Zinc6jet->Fill(jets[5].eta, weight);
                JetsHT_Zinc6jet->Fill(jetsHT, weight);
                JetsHT_1_Zinc6jet->Fill(jetsHT, weight);
                
                ZNGoodJets_Zinc_NoWeight->Fill(6.);
                ZMass_Zinc6jet->Fill(Z.M(), weight);
                ZPt_Zinc6jet->Fill(Z.Pt(), weight);
                ZRapidity_Zinc6jet->Fill(Z.Rapidity(), weight);
                ZEta_Zinc6jet->Fill(Z.Eta(), weight);
                lepPt_Zinc6jet->Fill(lepton1.pt, weight);
                lepEta_Zinc6jet->Fill(lepton1.eta, weight);
                lepPhi_Zinc6jet->Fill(lepton1.phi, weight);
                FirstHighestJetPt_Zinc6jet->Fill(jets[0].pt, weight);
                SecondHighestJetPt_Zinc6jet->Fill(jets[1].pt, weight);
                ThirdHighestJetPt_Zinc6jet->Fill(jets[2].pt, weight);
                SixthJetPtEta_Zinc6jet->Fill(jets[5].pt, fabs(jets[5].eta), weight);
                SixthJetPhi_Zinc6jet->Fill(jets[5].phi, weight);
                
                if (nGoodJets == 6){
                    ZNGoodJets_Zexc_NoWeight->Fill(6.);
                    ZMass_Zexc6jet->Fill(Z.M(), weight);
                    ZPt_Zexc6jet->Fill(Z.Pt(), weight);
                    ZRapidity_Zexc6jet->Fill(Z.Rapidity(), weight);
                    ZEta_Zexc6jet->Fill(Z.Eta(), weight);
                    lepPt_Zexc6jet->Fill(lepton1.pt, weight);
                    lepEta_Zexc6jet->Fill(lepton1.eta, weight);
                    lepPhi_Zexc6jet->Fill(lepton1.phi, weight);
                }
            }
            //==6 
            if (nGoodJets >= 7){
                ZNGoodJets_Zinc->Fill(7., weight);
                ZNGoodJetsFull_Zinc->Fill(7., weight);
                ZNGoodJetsFull_Zexc->Fill(7., weight);
            }
            //==7
            
            //more bins in Single Lepton dataset than Double --> xsec bigger
            if (nGoodJets >= 8) ZNGoodJets_Zinc->Fill(8., weight);
            if (doW && nGoodJets >= 9)  ZNGoodJets_Zinc->Fill(9., weight);
            if (doW && nGoodJets >= 10) ZNGoodJets_Zinc->Fill(10., weight);
        }
        //end of if (hasRecoInfo && passesLeptonCut && passesJetCut && passesDPSPartonCut)
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        
        //=======================================================================================================//
        //             Unfolding              //
        //====================================//
        if (hasRecoInfo && hasGenInfo && passesGenLeptonCut && passesLeptonCut){
            //-- jet multiplicity exc
            hresponseZNGoodJets_Zexc->Fill(nGoodJets, nGoodGenJets, weight);
            hresponseMT_Zinc0jet->Fill(MT, genMT, weight);
            hresponseMET_Zinc0jet->Fill(METpt, genLepton2.pt, weight);
            
            if (nGoodGenJets <= 7 && nGoodJets <= 7){
                hresponseZNGoodJetsFull_Zexc->Fill(nGoodJets, nGoodGenJets, weight);
            }
            else if (nGoodGenJets <= 7 && nGoodJets > 7){
                hresponseZNGoodJetsFull_Zexc->Fill(7., nGoodGenJets, weight);
            }
            else if (nGoodGenJets > 7 && nGoodJets <= 7){
                hresponseZNGoodJetsFull_Zexc->Fill(nGoodJets, 7., weight);
            }
            else if (nGoodGenJets > 7 && nGoodJets > 7){
                hresponseZNGoodJetsFull_Zexc->Fill(7., 7., weight);
            }
            
            hresponseZNGoodJets_Zinc->Fill(0., 0., weight);
            hresponseZNGoodJetsFull_Zinc->Fill(0., 0., weight);
            //Z
            hresponseZMass_Zinc0jet->Fill(Z.M(), genZ.M(), weight);
            hresponseZPt_Zinc0jet->Fill(Z.Pt(), genZ.Pt(), weight);
            hresponseZRapidity_Zinc0jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
            hresponseZEta_Zinc0jet->Fill(Z.Eta(), genZ.Eta(), weight);
            //lep
            hresponselepPt_Zinc0jet->Fill(lep1.Pt(), genLep1.Pt(), weight);

            hresponselepEta_Zinc0jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
            if(lepton1.pt > 25 && lepton1.pt <= 30)   hresponselepEta_Zinc0jet_leppt_25_30->Fill(lep1.Eta(), genLep1.Eta(), weight);
            if(lepton1.pt > 30 && lepton1.pt <= 35)   hresponselepEta_Zinc0jet_leppt_30_35->Fill(lep1.Eta(), genLep1.Eta(), weight);
            if(lepton1.pt > 35 && lepton1.pt <= 40)   hresponselepEta_Zinc0jet_leppt_35_40->Fill(lep1.Eta(), genLep1.Eta(), weight);
            if(lepton1.pt > 40 && lepton1.pt <= 45)   hresponselepEta_Zinc0jet_leppt_40_45->Fill(lep1.Eta(), genLep1.Eta(), weight);


            hresponselepPhi_Zinc0jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
            if (nGoodGenJets == 0 && nGoodJets == 0){
                //Z
                hresponseZMass_Zexc0jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc0jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc0jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc0jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc0jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc0jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc0jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- First Jet Pt
            if (nGoodGenJets >= 1 && nGoodJets >= 1){
                hresponseZNGoodJets_Zinc->Fill(1., 1., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(1., 1., weight);
                //Z
                hresponseZMass_Zinc1jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zinc1jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zinc1jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zinc1jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zinc1jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zinc1jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zinc1jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
                //Jets
                hresponseFirstJetPt_Zinc1jet->Fill(jets[0].pt, genJets[0].pt, weight);
                hresponseFirstJetEta_Zinc1jet->Fill(fabs(jets[0].eta), fabs(genJets[0].eta), weight);
                hresponseJetsHT_Zinc1jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponseFirstJetPt_1_Zinc1jet->Fill(jets[0].pt, genJets[0].pt, weight);
                hresponseFirstJetPt_2_Zinc1jet->Fill(jets[0].pt, genJets[0].pt, weight);
                hresponseFirstJetEta_2_Zinc1jet->Fill(fabs(jets[0].eta), fabs(genJets[0].eta), weight);
                hresponseJetsHT_1_Zinc1jet->Fill(jetsHT, genJetsHT, weight);
                hresponseJetsHT_2_Zinc1jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponsedPhiLepJet1_Zinc1jet->Fill(deltaPhi(lep1, newLeadJ), deltaPhi(genLep1, genNewLeadJ), weight);
                hresponsedPhiLepJet1_2_Zinc1jet->Fill(deltaPhi(lep1, newLeadJ), deltaPhi(genLep1, genNewLeadJ), weight);
            }
            if (nGoodGenJets == 1 && nGoodJets == 1){
                //Z
                hresponseZMass_Zexc1jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc1jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc1jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc1jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc1jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc1jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc1jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- Second Jet Pt
            if (nGoodGenJets >= 2 && nGoodJets >= 2){
                hresponseZNGoodJets_Zinc->Fill(2., 2., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(2., 2., weight);
                //Z
                hresponseZMass_Zinc2jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zinc2jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zinc2jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zinc2jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zinc2jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zinc2jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zinc2jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
                //Jets
                hresponseSecondJetPt_Zinc2jet->Fill(jets[1].pt, genJets[1].pt, weight);
                hresponseSecondJetEta_Zinc2jet->Fill(fabs(jets[1].eta), fabs(genJets[1].eta), weight);
                hresponseJetsHT_Zinc2jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponseSecondJetPt_1_Zinc2jet->Fill(jets[1].pt, genJets[1].pt, weight);
                hresponseSecondJetPt_2_Zinc2jet->Fill(jets[1].pt, genJets[1].pt, weight);
                hresponseSecondJetEta_2_Zinc2jet->Fill(fabs(jets[1].eta), fabs(genJets[1].eta), weight);
                hresponseJetsHT_1_Zinc2jet->Fill(jetsHT, genJetsHT, weight);
                hresponseJetsHT_2_Zinc2jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponsedRapidityJets_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), weight);
                hresponsedRapidityJets_2_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), weight);
                
                hresponsedRapidityJetsFB_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, genForwardJetRapidity - genBackwardJetRapidity, weight);
                hresponsedRapidityJetsFB_2_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, genForwardJetRapidity - genBackwardJetRapidity, weight);
                
                hresponsedPhiJets_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), deltaPhi(genLeadJ, genSecondJ), weight);
                hresponsedPhiJets_2_Zinc2jet->Fill(deltaPhi(leadJ, secondJ), deltaPhi(genLeadJ, genSecondJ), weight);
                
                hresponsedPhiJetsFB_Zinc2jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), weight);
                hresponsedPhiJetsFB_2_Zinc2jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), weight);
                
                hresponsedRJets_Zinc2jet->Fill(deltaRYPhi(newLeadJ, newSecondJ), deltaRYPhi(genNewLeadJ, genNewSecondJ), weight);
                hresponsedRJets_2_Zinc2jet->Fill(deltaRYPhi(newLeadJ, newSecondJ), deltaRYPhi(genNewLeadJ, genNewSecondJ), weight);
                
                hresponsediJetMass_Zinc2jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                hresponsediJetMass_2_Zinc2jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                
                hresponsediJetPt_Zinc2jet->Fill(jet1Plus2.Pt(), genJet1Plus2.Pt(), weight);
                hresponsediJetPt_2_Zinc2jet->Fill(jet1Plus2.Pt(), genJet1Plus2.Pt(), weight);
                
                hresponsedPhiLepJet2_Zinc2jet->Fill(deltaPhi(lep1, newSecondJ), deltaPhi(genLep1, genNewSecondJ), weight);
                hresponsedPhiLepJet2_2_Zinc2jet->Fill(deltaPhi(lep1, newSecondJ), deltaPhi(genLep1, genNewSecondJ), weight);
            }
            if (nGoodGenJets == 2 && nGoodJets == 2){
                //Z
                hresponseZMass_Zexc2jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc2jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc2jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc2jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc2jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc2jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc2jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- Third Jet Pt
            if (nGoodGenJets >= 3 && nGoodJets >= 3){
                hresponseZNGoodJets_Zinc->Fill(3., 3., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(3., 3., weight);
                //Z
                hresponseZMass_Zinc3jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zinc3jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zinc3jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zinc3jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zinc3jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zinc3jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zinc3jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
                //Jets
                hresponseThirdJetPt_Zinc3jet->Fill(jets[2].pt, genJets[2].pt, weight);
                hresponseThirdJetEta_Zinc3jet->Fill(fabs(jets[2].eta), fabs(genJets[2].eta), weight);
                hresponseJetsHT_Zinc3jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponseThirdJetPt_1_Zinc3jet->Fill(jets[2].pt, genJets[2].pt, weight);
                hresponseThirdJetPt_2_Zinc3jet->Fill(jets[2].pt, genJets[2].pt, weight);
                hresponseThirdJetEta_2_Zinc3jet->Fill(fabs(jets[2].eta), fabs(genJets[2].eta), weight);
                hresponseJetsHT_1_Zinc3jet->Fill(jetsHT, genJetsHT, weight);
                hresponseJetsHT_2_Zinc3jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponsedRapidityJets_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), weight);
                hresponsedRapidityJets_2_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), weight);
                
                hresponsedRapidityJets_First_Third_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                hresponsedRapidityJets_2_First_Third_Zinc3jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                
                hresponsedRapidityJets_Second_Third_Zinc3jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                hresponsedRapidityJets_2_Second_Third_Zinc3jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                
                hresponsedRapidityJetsFB_Zinc3jet->Fill(ForwardJetRapidity - BackwardJetRapidity, genForwardJetRapidity - genBackwardJetRapidity, weight);
                hresponsedRapidityJetsFB_2_Zinc3jet->Fill(ForwardJetRapidity - BackwardJetRapidity, genForwardJetRapidity - genBackwardJetRapidity, weight);
                
                hresponsedPhiJets_Zinc3jet->Fill(deltaPhi(leadJ, secondJ), deltaPhi(genLeadJ, genSecondJ), weight);
                hresponsedPhiJets_2_Zinc3jet->Fill(deltaPhi(leadJ, secondJ), deltaPhi(genLeadJ, genSecondJ), weight);
                
                hresponsedPhiJetsFB_Zinc3jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), weight);
                hresponsedPhiJetsFB_2_Zinc3jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), weight);
                
                hresponsediJetMass_Zinc3jet->Fill(jet1Plus2.M(),genJet1Plus2.M(), weight);
                hresponsediJetMass_2_Zinc3jet->Fill(jet1Plus2.M(),genJet1Plus2.M(), weight);
                
                hresponsediJetPt_Zinc3jet->Fill(jet1Plus2.Pt(), genJet1Plus2.Pt(), weight);
                hresponsediJetPt_2_Zinc3jet->Fill(jet1Plus2.Pt(), genJet1Plus2.Pt(), weight);
                
                hresponsedPhiLepJet3_Zinc3jet->Fill(deltaPhi(lep1, newThirdJ), deltaPhi(genLep1, genNewThirdJ), weight);
                hresponsedPhiLepJet3_2_Zinc3jet->Fill(deltaPhi(lep1, newThirdJ), deltaPhi(genLep1, genNewThirdJ), weight);
            }
            if (nGoodGenJets == 3 && nGoodJets == 3){
                //Z
                hresponseZMass_Zexc3jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc3jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc3jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc3jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc3jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc3jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc3jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- Fourth Jet Pt
            if (nGoodGenJets >= 4 && nGoodJets >= 4){
                hresponseZNGoodJets_Zinc->Fill(4., 4., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(4., 4., weight);
                //Z
                hresponseZMass_Zinc4jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zinc4jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zinc4jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zinc4jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zinc4jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zinc4jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zinc4jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
                //Jets
                hresponseFourthJetPt_Zinc4jet->Fill(jets[3].pt, genJets[3].pt, weight);
                hresponseFourthJetEta_Zinc4jet->Fill(fabs(jets[3].eta), fabs(genJets[3].eta), weight);
                hresponseJetsHT_Zinc4jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponseFourthJetPt_1_Zinc4jet->Fill(jets[3].pt, genJets[3].pt, weight);
                hresponseFourthJetPt_2_Zinc4jet->Fill(jets[3].pt, genJets[3].pt, weight);
                hresponseFourthJetEta_2_Zinc4jet->Fill(fabs(jets[3].eta), fabs(genJets[3].eta), weight);
                hresponseJetsHT_1_Zinc4jet->Fill(jetsHT, genJetsHT, weight);
                hresponseJetsHT_2_Zinc4jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponsedRapidityJets_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), weight);
                hresponsedRapidityJets_2_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), weight);
                
                hresponsedRapidityJets_First_Third_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                hresponsedRapidityJets_2_First_Third_Zinc4jet->Fill(fabs(newLeadJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewLeadJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                
                hresponsedRapidityJets_Second_Third_Zinc4jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                hresponsedRapidityJets_2_Second_Third_Zinc4jet->Fill(fabs(newSecondJ.Rapidity() - newThirdJ.Rapidity()), fabs(genNewSecondJ.Rapidity() - genNewThirdJ.Rapidity()), weight);
                
                hresponsedRapidityJetsFB_Zinc4jet->Fill(ForwardJetRapidity - BackwardJetRapidity, genForwardJetRapidity - genBackwardJetRapidity, weight);
                hresponsedRapidityJetsFB_2_Zinc4jet->Fill(ForwardJetRapidity - BackwardJetRapidity, genForwardJetRapidity - genBackwardJetRapidity, weight);
                
                hresponsedPhiJets_Zinc4jet->Fill(deltaPhi(leadJ, secondJ), deltaPhi(genLeadJ, genSecondJ), weight);
                hresponsedPhiJets_2_Zinc4jet->Fill(deltaPhi(leadJ, secondJ), deltaPhi(genLeadJ, genSecondJ), weight);
                
                hresponsedPhiJetsFB_Zinc4jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), weight);
                hresponsedPhiJetsFB_2_Zinc4jet->Fill(deltaPhi(vJetYOrdered[0], vJetYOrdered[vJetYOrdered.size() - 1] ), deltaPhi(genvJetYOrdered[0], genvJetYOrdered[genvJetYOrdered.size() - 1] ), weight);
                
                hresponsediJetMass_Zinc4jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                hresponsediJetMass_2_Zinc4jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                
                hresponsediJetPt_Zinc4jet->Fill(jet1Plus2.Pt(), genJet1Plus2.Pt(), weight);
                hresponsediJetPt_2_Zinc4jet->Fill(jet1Plus2.Pt(), genJet1Plus2.Pt(), weight);
                
                hresponsedPhiLepJet4_Zinc4jet->Fill(deltaPhi(lep1, newFourthJ), deltaPhi(genLep1, genNewFourthJ), weight);
                hresponsedPhiLepJet4_2_Zinc4jet->Fill(deltaPhi(lep1, newFourthJ), deltaPhi(genLep1, genNewFourthJ), weight);
            }
            if (nGoodGenJets == 4 && nGoodJets == 4){
                //Z
                hresponseZMass_Zexc4jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc4jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc4jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc4jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc4jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc4jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc4jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- Fifth Jet Pt
            if (nGoodGenJets >= 5 && nGoodJets >= 5){
                hresponseZNGoodJets_Zinc->Fill(5., 5., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(5., 5., weight);
                //Z
                hresponseZMass_Zinc5jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zinc5jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zinc5jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zinc5jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zinc5jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zinc5jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zinc5jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
                //Jets
                hresponseFifthJetPt_Zinc5jet->Fill(jets[4].pt, genJets[4].pt, weight);
                hresponseFifthJetEta_Zinc5jet->Fill(fabs(jets[4].eta), fabs(genJets[4].eta), weight);
                hresponseJetsHT_Zinc5jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponseFifthJetPt_1_Zinc5jet->Fill(jets[4].pt, genJets[4].pt, weight);
                hresponseFifthJetPt_2_Zinc5jet->Fill(jets[4].pt, genJets[4].pt, weight);
                hresponseFifthJetEta_2_Zinc5jet->Fill(fabs(jets[4].eta), fabs(genJets[4].eta), weight);
                hresponseJetsHT_1_Zinc5jet->Fill(jetsHT, genJetsHT, weight);
                hresponseJetsHT_2_Zinc5jet->Fill(jetsHT, genJetsHT, weight);
                
                hresponsedPhiLepJet5_Zinc5jet->Fill(deltaPhi(lep1, newFifthJ), deltaPhi(genLep1, genNewFifthJ), weight);
                hresponsedPhiLepJet5_2_Zinc5jet->Fill(deltaPhi(lep1, newFifthJ), deltaPhi(genLep1, genNewFifthJ), weight);
            }
            if (nGoodGenJets == 5 && nGoodJets == 5){
                //Z
                hresponseZMass_Zexc5jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc5jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc5jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc5jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc5jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc5jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc5jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- Sixth Jet Pt
            if (nGoodGenJets >= 6 && nGoodJets >= 6){
                hresponseZNGoodJets_Zinc->Fill(6., 6., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(6., 6., weight);
                //Z
                hresponseZMass_Zinc6jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zinc6jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zinc6jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zinc6jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zinc6jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zinc6jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zinc6jet->Fill(lep1.Phi(), genLep1.Phi(), weight);
            }
            if (nGoodGenJets == 6 && nGoodJets == 6){
                //Z
                hresponseZMass_Zexc6jet->Fill(Z.M(), genZ.M(), weight);
                hresponseZPt_Zexc6jet->Fill(Z.Pt(), genZ.Pt(), weight);
                hresponseZRapidity_Zexc6jet->Fill(Z.Rapidity(), genZ.Rapidity(), weight);
                hresponseZEta_Zexc6jet->Fill(Z.Eta(), genZ.Eta(), weight);
                //lep
                hresponselepPt_Zexc6jet->Fill(lep1.Pt(), genLep1.Pt(), weight);
                hresponselepEta_Zexc6jet->Fill(lep1.Eta(), genLep1.Eta(), weight);
                hresponselepPhi_Zexc6jet->Fill(lep1.Phi(), genLep1.Phi(), weight);  
            }
            //-- inc 7 jets case
            if (nGoodGenJets >= 7 && nGoodJets >= 7){
                hresponseZNGoodJets_Zinc->Fill(7., 7., weight);
                hresponseZNGoodJetsFull_Zinc->Fill(7., 7., weight);
            }
            //-- inc 8 jets case
            if (nGoodGenJets >= 8 && nGoodJets >= 8){
                hresponseZNGoodJets_Zinc->Fill(8., 8., weight);
            }
            //-- inc 9 jets case
            if (nGoodGenJets >= 9 && nGoodJets >= 9){
                hresponseZNGoodJets_Zinc->Fill(9., 9., weight);
            }
            //-- inc 9 jets case
            if (nGoodGenJets >= 10 && nGoodJets >= 10){
                hresponseZNGoodJets_Zinc->Fill(10., 10., weight);
            }
            
        }
        //end of Unfolding 
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        
        // MeanNJ use old method of filling response.
        if (hasRecoInfo && hasGenInfo){
            
            //--- Set weight for systematic for reweighted response
            double WforMeanNHT1       = weight;
            double genWforMeanNHT1    = genWeight;
            double WforMeanNHT2       = weight;
            double genWforMeanNHT2    = genWeight;
            double WforMeanNdRap12    = weight;
            double genWforMeanNdRap12 = genWeight;
            double WforMeanNdRapFB    = weight;
            double genWforMeanNdRapFB = genWeight;
            if (doRespSyst){
                WforMeanNHT1 = ReweightForResp(vecFwHT1, binEdgeHT1, weight, jetsHT);
                genWforMeanNHT1 = ReweightForResp(vecFwHT1, binEdgeHT1, genWeight, jetsHT);
                WforMeanNHT2 = ReweightForResp(vecFwHT2, binEdgeHT2, weight, jetsHT);
                genWforMeanNHT2 = ReweightForResp(vecFwHT2, binEdgeHT2, genWeight, jetsHT);
                WforMeanNdRap12 = ReweightForResp(vecFwRap12, binEdgeRap12, weight, fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()));
                genWforMeanNdRap12 = ReweightForResp(vecFwRap12, binEdgeRap12, genWeight, fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()));
                WforMeanNdRapFB = ReweightForResp(vecFwRapFB, binEdgeRapFB, weight, fabs(ForwardJetRapidity - BackwardJetRapidity));
                genWforMeanNdRapFB = ReweightForResp(vecFwRapFB, binEdgeRapFB, genWeight, fabs(ForwardJetRapidity - BackwardJetRapidity));
            }
            //------------------------------

            //-- one jet inclusive
            if (nGoodGenJets >= 1 && passesGenLeptonCut){
                if (nGoodJets >= 1 && passesLeptonCut){
                    responseMeanNJetsHT_Zinc1jet->Fill(jetsHT, nGoodJets, genJetsHT, nGoodGenJets, WforMeanNHT1);
                }
                else{
                    responseMeanNJetsHT_Zinc1jet->Miss(genJetsHT, nGoodGenJets, genWforMeanNHT1);
                }
            }
            if (nGoodJets >= 1 && passesLeptonCut){
                if (!(nGoodGenJets >= 1 && passesGenLeptonCut)){
                    responseMeanNJetsHT_Zinc1jet->Fake(jetsHT, nGoodJets, WforMeanNHT1);
                }
            }
            
            //-- two jets inclusive
            if (nGoodGenJets >= 2 && passesGenLeptonCut){
                if (nGoodJets >= 2 && passesLeptonCut){
                    responseMeanNJetsHT_Zinc2jet->Fill(jetsHT, nGoodJets, genJetsHT, nGoodGenJets, WforMeanNHT2);
                    responseMeanNJetsdRapidity_Zinc2jet->Fill(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), nGoodJets, fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), nGoodGenJets, WforMeanNdRap12);
                    responseMeanNJetsdRapidityFB_Zinc2jet->Fill(ForwardJetRapidity - BackwardJetRapidity, nGoodJets, genForwardJetRapidity - genBackwardJetRapidity, nGoodGenJets, WforMeanNdRapFB);
                }
                else {
                    responseMeanNJetsHT_Zinc2jet->Miss(genJetsHT, nGoodGenJets, genWforMeanNHT2);
                    responseMeanNJetsdRapidity_Zinc2jet->Miss(fabs(genNewLeadJ.Rapidity() - genNewSecondJ.Rapidity()), nGoodGenJets, genWforMeanNdRap12);
                    responseMeanNJetsdRapidityFB_Zinc2jet->Miss(genForwardJetRapidity - genBackwardJetRapidity, nGoodGenJets, genWforMeanNdRapFB);
                }
            }
            if (nGoodJets >= 2 && passesLeptonCut){
                if (!(nGoodGenJets >= 2 && passesGenLeptonCut)){
                    responseMeanNJetsHT_Zinc2jet->Fake(jetsHT, nGoodJets, WforMeanNHT2);
                    responseMeanNJetsdRapidity_Zinc2jet->Fake(fabs(newLeadJ.Rapidity() - newSecondJ.Rapidity()), nGoodJets, WforMeanNdRap12);
                    responseMeanNJetsdRapidityFB_Zinc2jet->Fake(ForwardJetRapidity - BackwardJetRapidity, nGoodJets, WforMeanNdRapFB);
                }
            }
        }
        //end of MeanNJ use old method of filling response.
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        
        //=======================================================================================================//

    } //End of loop over all the events
    
    //==========================================================================================================//

    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    //         Writing file             //
    //==================================//

    outputFile->cd();

    //--- Save all the histograms ---
    unsigned short numbOfHistograms = listOfHistograms.size();
    for (unsigned short i(0); i < numbOfHistograms; i++){
        string hName = listOfHistograms[i]->GetName();
        if ( (!hasGenInfo && hName.find("gen") != string::npos ) || (!hasRecoInfo && hName.find("gen") == string::npos )) continue; 
        if (fileName.find("HepMC") != string::npos){
            //if (sumSherpaW > 0) listOfHistograms[i]->Scale(1/sumSherpaW);
            //if (doTTreweighting) listOfHistograms[i]->Scale(weightSumNoTopRew / weightSum );
        }
        if (fileName.find("Sherpa2") != string::npos){
            if (sumSherpaW > 0) listOfHistograms[i]->Scale(1/sumSherpaW);
        }
        listOfHistograms[i]->Write();
    }
    //--- Save all the RooUnfoldResponses ---
    if ( hasGenInfo && hasRecoInfo ){
        unsigned short numbOfResponses = listOfResponses.size();
        for (unsigned short i(0); i < numbOfResponses; i++){
            string currentName = listOfResponses[i]->GetName();
            currentName = currentName.substr(0, currentName.find("_gen"));
            string savingName = "response" + currentName;
            outputFile->WriteTObject(listOfResponses[i], savingName.c_str());
        }
    }
    
    // let's delete all histograms, just to be safe
    for (unsigned short i(0); i < numbOfHistograms; i++){
        delete listOfHistograms[i];
    }
    
    outputFile->Write();
    outputFile->Close();
    
    //==========================================================================================================//


    cout << "Number of events                               : " << nEvents << endl;
    cout << "Total GEN weight of all events                 : " << TotalGenWeight << endl;
    cout << "Number with two good leptons no charge no mass : " << nEventsWithTwoGoodLeptonsNoChargeNoMass << endl;
    cout << "Number with two good leptons no mass           : " << nEventsWithTwoGoodLeptonsNoMass << endl;
    cout << "Total GEN pass: RECO weight of all events      : " << TotalGenWeightPassGENPU << endl;
    cout << "Total GEN pass: GEN weight of all events       : " << TotalGenWeightPassGEN << endl;
    cout << "Total RECO pass: RECO weight of all events     : " << TotalRecoWeightPassRECO << endl;
    cout << "Total RECO pass: GEN weight of all events      : " << TotalGenWeightPassRECO << endl;
    cout << "Number with two good leptons                   : " << nEventsWithTwoGoodLeptons << endl;
    cout << "How many times do we visit unfolding 0 jets    : " << nEventsUNFOLDIncl0Jets << endl;
    cout << "Number Inclusif 0 jets                         : " << nEventsIncl0Jets << endl;
    cout << "Number Exclusif 0 jets                         : " << nEventsExcl0Jets << endl;
    cout << "Number Exclusif 1 jets                         : " << nEventsExcl1Jets << endl;
    cout << "Number Exclusif 2 jets                         : " << nEventsExcl2Jets << endl;
    cout << "Number Exclusif 3 jets                         : " << nEventsExcl3Jets << endl;
    cout << "Number Inclusive 1 B-jet                       : " << nEventsIncBJets << endl;
    cout << "Number GEN Inclusif 0 jets                     : " << GENnEventsIncl0Jets << endl;
    cout << "Number GEN Inclusif 1 jets                     : " << GENnEventsIncl1Jets << endl;
    cout << "Number GEN Inclusif 2 jets                     : " << GENnEventsIncl2Jets << endl;
    cout << "Number GEN Inclusif 3 jets                     : " << GENnEventsIncl3Jets << endl;
    cout << "Sherpa weight                                  : " << sumSherpaW << endl;
    cout << "Sherpa weight fill                             : " << sumSherpaW0 << endl;
}











ZJetsAndDPS::ZJetsAndDPS(string fileName_, float lumiScale_, float puScale_, bool useTriggerCorrection_, bool useEfficiencyCorrection_, 
        int systematics_, int direction_, float xsecfactor_, int RecJetPtMin_, int RecJetPtMax_, int ZPtCutMin_, int ZEtaCutMin_, int ZEtaCutMax_, int METcut_, bool nEvents_10000_, int RecJetEtaMin_, int RecJetEtaMax_): 
    HistoSet(fileName_.substr(0, fileName_.find("_"))), nEvents_10000(nEvents_10000_), outputDirectory("Results/HistoFiles/MySetOfCuts/MuPt25/wplus/FinerBins/Systematics/Trial/"),
    fileName(fileName_), lumiScale(lumiScale_), puScale(puScale_), useTriggerCorrection(useTriggerCorrection_), useEfficiencyCorrection(useEfficiencyCorrection_), 
    systematics(systematics_), direction(direction_), xsecfactor(xsecfactor_), RecJetPtMin(RecJetPtMin_), RecJetPtMax(RecJetPtMax_), RecJetEtaMin(RecJetEtaMin_), RecJetEtaMax(RecJetEtaMax_), ZPtCutMin(ZPtCutMin_), ZEtaCutMin(ZEtaCutMin_), ZEtaCutMax(ZEtaCutMax_), METcut(METcut_)
{

    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.

    TChain *chain = new TChain("", "");
    isData = false;
    string fullFileName =  "../Data_Z_5311_New/" + fileName;


    if (fileName.find("DMu_") == 0) leptonFlavor = "Muons";
    else if (fileName.find("DE_") == 0)  leptonFlavor = "Electrons"; 
    else if (fileName.find("SMu_") == 0) leptonFlavor = "SingleMuon";
    else if (fileName.find("SE_") == 0)  leptonFlavor = "SingleElectron";
    else if (fileName.find("SMuE_") == 0){
        leptonFlavor = "TTMuE";
        fullFileName =  "../DataTTbarEMu/" + fileName;
    }
    if (fileName.find("Data") != string::npos ) isData = true;
    if ( fileName.find("SMu_") == 0 || fileName.find("SE_") == 0 ) fullFileName =  "/eos/cms/store/group/phys_smp/WPlusJets/NtuplesTomislavNEW/" + fileName;
    if (fileName.find("Sherpa2") != string::npos) fullFileName =  "../DataSherpa2/" + fileName;
    if (fileName.find("HEJ") != string::npos) fullFileName =  "../DataHEJ/" + fileName;
    
    if (fileName.find("List") == string::npos){
        if (fileName.find("Sherpa2") != string::npos){
            fullFileName += ".root";
            string treePath = fullFileName + "/tree";
            cout << "Loading file: " << fullFileName << endl;
            chain->Add(treePath.c_str());
        }
        else if (fileName.find("HEJ") != string::npos){
            fullFileName += ".root";
            string treePath = fullFileName + "/tree/tree;13";
            cout << "Loading file: " << fullFileName << endl;
            chain->Add(treePath.c_str());
        }
        else{
            fullFileName += ".root";
            string treePath = fullFileName + "/tree/tree";
            cout << "Loading file: " << fullFileName << endl;
            chain->Add(treePath.c_str());
        }
    }
    else {
        fullFileName += ".txt";
        ifstream infile(fullFileName.c_str());
        string line; 
        int countFiles(0);
        while (getline(infile, line)){
            countFiles++;
            //string treePath =  line + "/tree/tree";
            string treePath =  "../DataSherpa2/" + line + "/tree"; // currently, only sherpa2 has List of files
            chain->Add(treePath.c_str());       
        }
    }
    fChain = chain;
}

ZJetsAndDPS::~ZJetsAndDPS(){
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

string ZJetsAndDPS::CreateOutputFileName(bool useRoch, bool doFlat, int doPUStudy, bool doVarWidth, int doBJets, int doQCD, bool doSSign, bool doInvMassCut, string pdfSet, int pdfMember)
{
    ostringstream result;
    result << outputDirectory << fileName;
    result << "_EffiCorr_" << useEfficiencyCorrection;
    result << "_TrigCorr_" << useTriggerCorrection;
    result << "_Syst_" << systematics;
    if (direction == 1) result << "_Up";
    else if (direction == -1) result << "_Down";
    result << "_JetPtMin_" << RecJetPtMin;
    if (RecJetPtMax > RecJetPtMin) result << "_JetPtMax_" << RecJetPtMax;
    if (ZPtCutMin > 0) result << "_ZPtMin" << abs(ZPtCutMin);
    if (ZEtaCutMin > -999999 && ZEtaCutMin <  0) result << "_ZEtaMin_m" << abs(ZEtaCutMin);
    if (ZEtaCutMin > -999999 && ZEtaCutMin >= 0) result << "_ZEtaMin_"  << abs(ZEtaCutMin);
    if (ZEtaCutMax <  999999 && ZEtaCutMax >= 0) result << "_ZEtaMax_"  << abs(ZEtaCutMax);
    if (ZEtaCutMax <  999999 && ZEtaCutMax <  0) result << "_ZEtaMax_m" << abs(ZEtaCutMax);

    if (useRoch) result << "_rochester";
    if (!isData && doFlat) result << "_Flat";
    if (doPUStudy >= 0) result << "_Beta" << doPUStudy;
    if (doVarWidth) result << "_VarWidth";
    if (doInvMassCut) result << "_InvMass";
    if (doSSign) result << "_SS";
    if (doBJets > 0) result << "_BJets";
    if (doBJets < 0) result << "_BVeto";
    if (doQCD>0) result << "_QCD" << doQCD;
    if (METcut > 0) result << "_MET" << METcut;
    if (pdfSet != "") result << "_PDF_" << pdfSet << "_" << pdfMember;

    //--- Add your test names here ---
    //result << "_NoPUCut";
    //result << "_LooseID";
    //result << "_SRANJE";

    result << ".root";
    return result.str();
}

Int_t ZJetsAndDPS::GetEntry(Long64_t entry){
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

Long64_t ZJetsAndDPS::LoadTree(Long64_t entry){
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();Notify();  }
    return centry;
}

void ZJetsAndDPS::Init(bool hasRecoInfo, bool hasGenInfo, bool hasPartonInfo){
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set object pointer
    mcSherpaWeights_ = 0 ;
    pdfInfo_ = 0;
    genLepPt_ = 0;
    genLepEta_ = 0;
    genLepPhi_ = 0;
    genLepE_ = 0;
    genLepQ_ = 0;
    genLepId_ = 0;
    genLepSt_ = 0;
    genPhoPt_ = 0;
    genPhoEta_ = 0;
    genPhoPhi_ = 0;
    genJetPt_ = 0;
    genJetEta_ = 0;
    genJetPhi_ = 0;
    genJetE_ = 0;
    dpsParton_Pt = 0;
    dpsParton_Eta = 0;
    dpsParton_Phi = 0;
    dpsParton_E = 0;
    genMatchDPSpar = 0;
    dpsParton_dR = 0;

    gsfElecPt_ = 0;
    gsfElecEta_ = 0;
    gsfElecPhi_ = 0;
    gsfElecEnergy_ = 0;
    patElecPt_ = 0;
    patElecEta_ = 0;
    patElecPhi_ = 0;
    patElecEn_ = 0;
    patElecCharge_ = 0;
    patElecID_ = 0;
    patElecTrig_ = 0;
    patElecDetIso_ = 0;
    patElecPfIsoRho_ = 0;
    patElecScEta_ = 0;
    patElecIsPF_ = 0;

    patMuonPt_ = 0;
    patMuonEta_ = 0;
    patMuonPhi_ = 0;
    patMuonVtxZ_ = 0;
    patMuonEn_ = 0;
    patMuonCharge_ = 0;
    patMuonDxy_ = 0;
    patMuonCombId_ = 0;
    patMuonTrig_ = 0;
    patMuonPfIsoDbeta_ = 0;

    patJetPfAk05En_ = 0;
    patJetPfAk05Pt_ = 0;
    patJetPfAk05Eta_ = 0;
    patJetPfAk05Phi_ = 0;
    patJetPfAk05LooseId_ = 0;
    patJetPfAk05jetBSZ_ = 0;
    patJetPfAk05jetBZ_ = 0;
    patJetPfAk05jetpuMVA_ = 0;
    patJetPfAk05OCSV_ = 0 ;
    patJetPfAk05PartonFlavour_ = 0; 
    patMetPt_ = 0 ;
    patMetPhi_= 0 ;
    patMetSig_= 0 ;

    // Set branch addresses and branch pointers
    fCurrent = -1;
    fChain->SetMakeClass(1);
    if (fileName.find("Data") == string::npos) fChain->SetBranchAddress("PU_npT", &PU_npT, &b_PU_npT);
    if (fileName.find("UNFOLDING") != string::npos) fChain->SetBranchAddress("nup_", &nup_, &b_nup_);
    if (hasRecoInfo){
        fChain->SetBranchAddress("EvtInfo_NumVtx", &EvtInfo_NumVtx, &b_EvtInfo_NumVtx);
        fChain->SetBranchAddress("EvtInfo_RunNum", &EvtInfo_RunNum, &b_EvtInfo_RunNum); // not used
        fChain->SetBranchAddress("EvtInfo_EventNum", &EvtInfo_EventNum, &b_EvtInfo_EventNum); // not used

        fChain->SetBranchAddress("patJetPfAk05En_", &patJetPfAk05En_, &b_patJetPfAk05En_);
        fChain->SetBranchAddress("patJetPfAk05Pt_", &patJetPfAk05Pt_, &b_patJetPfAk05Pt_);
        fChain->SetBranchAddress("patJetPfAk05Eta_", &patJetPfAk05Eta_, &b_patJetPfAk05Eta_);
        fChain->SetBranchAddress("patJetPfAk05Phi_", &patJetPfAk05Phi_, &b_patJetPfAk05Phi_);
        fChain->SetBranchAddress("patJetPfAk05LooseId_", &patJetPfAk05LooseId_, &b_patJetPfAk05LooseId_);
        fChain->SetBranchAddress("patJetPfAk05jetBSZ_", &patJetPfAk05jetBSZ_, &b_patJetPfAk05jetBSZ_);
        fChain->SetBranchAddress("patJetPfAk05jetBZ_", &patJetPfAk05jetBZ_, &b_patJetPfAk05jetBZ_);
        fChain->SetBranchAddress("patJetPfAk05jetpuMVA_", &patJetPfAk05jetpuMVA_, &b_patJetPfAk05jetpuMVA_);
        fChain->SetBranchAddress("patJetPfAk05OCSV_", &patJetPfAk05OCSV_, &b_patJetPfAk05OCSV_);
        fChain->SetBranchAddress("patJetPfAk05PartonFlavour_", &patJetPfAk05PartonFlavour_, &b_patJetPfAk05PartonFlavour_);
        fChain->SetBranchAddress("patMetPt_", &patMetPt_, &b_patMetPt_);
        fChain->SetBranchAddress("patMetPhi_", &patMetPhi_, &b_patMetPhi_);
        //fChain->SetBranchAddress("patMetSig_", &patMetSig_, &b_patMetSig_); // not used

        if (leptonFlavor != "Muons"){
            //fChain->SetBranchAddress("gsfElecPt_", &gsfElecPt_, &b_gsfElecPt_); // not used
            //fChain->SetBranchAddress("gsfElecEta_", &gsfElecEta_, &b_gsfElecEta_); // not used
            //fChain->SetBranchAddress("gsfElecPhi_", &gsfElecPhi_, &b_gsfElecPhi_); // not used
            //fChain->SetBranchAddress("gsfElecEnergy_", &gsfElecEnergy_, &b_gsfElecEnergy_); // not used
            fChain->SetBranchAddress("patElecPt_", &patElecPt_, &b_patElecPt_);
            fChain->SetBranchAddress("patElecEta_", &patElecEta_, &b_patElecEta_);
            fChain->SetBranchAddress("patElecPhi_", &patElecPhi_, &b_patElecPhi_);
            fChain->SetBranchAddress("patElecEnergy_", &patElecEn_, &b_patElecEn_);
            fChain->SetBranchAddress("patElecCharge_", &patElecCharge_, &b_patElecCharge_);
            fChain->SetBranchAddress("patElecID_", &patElecID_, &b_patElecID_);
            fChain->SetBranchAddress("patElecTrig_", &patElecTrig_, &b_patElecTrig_);
            //fChain->SetBranchAddress("patElecDetIso_", &patElecDetIso_, &b_patElecDetIso_); // not used
            fChain->SetBranchAddress("patElecPfIsoRho_", &patElecPfIsoRho_, &b_patElecPfIsoRho_); 
            fChain->SetBranchAddress("patElecScEta_", &patElecScEta_, &b_patElecScEta_);
            //fChain->SetBranchAddress("patElecIsPF_", &patElecIsPF_, &b_patElecIsPF_); // not used

        }
        if (leptonFlavor != "Electrons"){
            fChain->SetBranchAddress("patMuonPt_", &patMuonPt_, &b_patMuonPt_);
            fChain->SetBranchAddress("patMuonEta_", &patMuonEta_, &b_patMuonEta_);
            fChain->SetBranchAddress("patMuonPhi_", &patMuonPhi_, &b_patMuonPhi_);
            //fChain->SetBranchAddress("patMuonVtxZ_", &patMuonVtxZ_, &b_patMuonVtxZ_); // not used
            fChain->SetBranchAddress("patMuonEn_", &patMuonEn_, &b_patMuonEn_);
            fChain->SetBranchAddress("patMuonCharge_", &patMuonCharge_, &b_patMuonCharge_);
            fChain->SetBranchAddress("patMuonDxy_", &patMuonDxy_, &b_patMuonDxy_);
            fChain->SetBranchAddress("patMuonCombId_", &patMuonCombId_, &b_patMuonCombId_);
            fChain->SetBranchAddress("patMuonTrig_", &patMuonTrig_, &b_patMuonTrig_);
            fChain->SetBranchAddress("patMuonPfIsoDbeta_", &patMuonPfIsoDbeta_, &b_patMuonPfIsoDbeta_);
        }
    }
    if (hasGenInfo){
        fChain->SetBranchAddress("genLepPt_", &genLepPt_, &b_genLepPt_);
        fChain->SetBranchAddress("genLepEta_", &genLepEta_, &b_genLepEta_);
        fChain->SetBranchAddress("genLepPhi_", &genLepPhi_, &b_genLepPhi_);
        fChain->SetBranchAddress("genLepE_", &genLepE_, &b_genLepE_);
        fChain->SetBranchAddress("genLepQ_", &genLepQ_, &b_genLepQ_);
        fChain->SetBranchAddress("genJetPt_", &genJetPt_, &b_genJetPt_);
        fChain->SetBranchAddress("genJetEta_", &genJetEta_, &b_genJetEta_);
        fChain->SetBranchAddress("genJetPhi_", &genJetPhi_, &b_genJetPhi_);
        fChain->SetBranchAddress("genJetE_", &genJetE_, &b_genJetE_);

        if (fileName.find("Sherpa") != string::npos || 
                (fileName.find("UNFOLDING") != string::npos && hasGenInfo ) || 
                //fileName.find("Powheg") != string::npos ||
                //fileName.find("P8") != string::npos ||
                //fileName.find("TopReweighting") != string::npos ||
                //fileName.find("Z2") != string::npos ||
                fileName.find("HEJ") != string::npos )
        {

            fChain->SetBranchAddress("pdfInfo_", &pdfInfo_, &b_pdfInfo_);
            fChain->SetBranchAddress("genLepId_", &genLepId_, &b_genLepId_);
            fChain->SetBranchAddress("genLepSt_", &genLepSt_, &b_genLepSt_);
            fChain->SetBranchAddress("genPhoPt_", &genPhoPt_, &b_genPhoPt_);
            fChain->SetBranchAddress("genPhoEta_", &genPhoEta_, &b_genPhoEta_);
            fChain->SetBranchAddress("genPhoPhi_", &genPhoPhi_, &b_genPhoPhi_);

//            if (fileName.find("MiNLO") != string::npos || 
//                    fileName.find("mcEveWeight") != string::npos || 
//                    fileName.find("HepMC") != string::npos){
//                fChain->SetBranchAddress("mcEveWeight_", &mcEveWeight_, &b_mcEveWeight_);
//            }
//
//            if (fileName.find("HepMC") != string::npos){
//                fChain->SetBranchAddress("mcSherpaSumWeight3_", &mcSherpaSumWeight3_, &b_mcSherpaSumWeight3_);
//                fChain->SetBranchAddress("mcSherpaWeights_", &mcSherpaWeights_, &b_mcSherpaWeights_);
//            }
            
            if (fileName.find("Sherpa2") != string::npos || fileName.find("HEJ") != string::npos){
                fChain->SetBranchAddress("mcSherpaWeights_", &mcSherpaWeights_, &b_mcSherpaWeights_);
            }

        }
    }
    if (hasPartonInfo){
        fChain->SetBranchAddress("dpsParton_Pt", &dpsParton_Pt, &b_dpsParton_Pt);
        fChain->SetBranchAddress("dpsParton_Eta", &dpsParton_Eta, &b_dpsParton_Eta);
        fChain->SetBranchAddress("dpsParton_Phi", &dpsParton_Phi, &b_dpsParton_Phi);
        fChain->SetBranchAddress("dpsParton_E", &dpsParton_E, &b_dpsParton_E);
        fChain->SetBranchAddress("genMatchDPSpar", &genMatchDPSpar, &b_genMatchDPSpar);
        fChain->SetBranchAddress("dpsParton_dR", &dpsParton_dR, &b_dpsParton_dR);
    }
    Notify();
    cout << "Branches are properly initialized." << endl;
}

Bool_t ZJetsAndDPS::Notify(){
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void ZJetsAndDPS::Show(Long64_t entry){
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}

Int_t ZJetsAndDPS::Cut(Long64_t entry){
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    printf("entry %lld", entry);
    return 1;
}
