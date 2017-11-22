//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 22 08:07:46 2012 by ROOT version 5.32/01
// from TTree tree/tree
// found on file: ../Data/DMu_2011_ALL_Louve_CHS_HLT.root
//////////////////////////////////////////////////////////

#ifndef ZJetsAndDPS_h
#define ZJetsAndDPS_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>
#include <TDatime.h>
#include <TMath.h>
#include <TRandom3.h>

// Header file for the classes stored in the TTree if any.
#include "functions.h"
#include "getFilesAndHistograms.h"
#include "standalone_LumiReWeighting.h"
//#include "Myrochcor2012.h"
#include "HistoSet.h"

using namespace std;

ClassImp(HistoSet)

class ZJetsAndDPS: public HistoSet {
  public :
    //TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    TChain          *fChain;   //!pointer to the analyzed TTree or TChain
    //TTree          *tree;
    Int_t           fCurrent; //!current Tree number in a TChain

    // Declaration of leaf types
    Double_t        PU_npT;
    Double_t        mcEveWeight_;
    Double_t        mcSherpaSumWeight3_ ;

    Int_t           EvtInfo_NumVtx;
    Int_t           EvtInfo_RunNum;
    Int_t           EvtInfo_EventNum;
    Int_t	          nup_;
    vector<double>  *mcSherpaWeights_;
    vector<double>  *pdfInfo_;
    vector<double>  *genLepPt_;
    vector<double>  *genLepEta_;
    vector<double>  *genLepPhi_;
    vector<double>  *genLepE_;
    vector<double>  *genLepQ_;
    vector<double>  *genLepId_;
    vector<double>  *genLepSt_;
    vector<double>  *genPhoPt_;
    vector<double>  *genPhoEta_;
    vector<double>  *genPhoPhi_;
    vector<double>  *genJetPt_;
    vector<double>  *genJetEta_;
    vector<double>  *genJetPhi_;
    vector<double>  *genJetE_;
    vector<double>  *dpsParton_Pt;
    vector<double>  *dpsParton_Eta;
    vector<double>  *dpsParton_Phi;
    vector<double>  *dpsParton_E;
    vector<int>     *genMatchDPSpar;
    vector<double>  *dpsParton_dR;

    vector<double>  *gsfElecPt_;
    vector<double>  *gsfElecEta_;
    vector<double>  *gsfElecPhi_;
    vector<double>  *gsfElecEnergy_;
    vector<double>  *patElecPt_;
    vector<double>  *patElecEta_;
    vector<double>  *patElecPhi_;
    vector<double>  *patElecEn_;
    vector<double>  *patElecCharge_;
    vector<double>  *patElecID_;
    vector<double>  *patElecTrig_;
    vector<double>  *patElecDetIso_;
    vector<double>  *patElecPfIsoRho_;
    vector<double>  *patElecScEta_;
    vector<double>  *patElecIsPF_;

    vector<double>  *patMuonPt_;
    vector<double>  *patMuonEta_;
    vector<double>  *patMuonPhi_;
    vector<double>  *patMuonVtxZ_;
    vector<double>  *patMuonEn_;
    vector<double>  *patMuonCharge_;
    vector<double>  *patMuonDxy_;
    vector<double>  *patMuonCombId_;
    vector<double>  *patMuonTrig_;
    vector<double>  *patMuonPfIsoDbeta_;
    // -- JET ---
    vector<double>  *patJetPfAk05En_;
    vector<double>  *patJetPfAk05Pt_;
    vector<double>  *patJetPfAk05Eta_;
    vector<double>  *patJetPfAk05Phi_;
    vector<double>  *patJetPfAk05LooseId_;
    vector<double>  *patJetPfAk05jetBSZ_;
    vector<double>  *patJetPfAk05jetBZ_;
    vector<double>  *patJetPfAk05jetpuMVA_;
    vector<double>  *patJetPfAk05OCSV_;
    vector<double>  *patJetPfAk05PartonFlavour_;
    // -- MET ---
    vector<double>  *patMetPt_;
    vector<double>  *patMetPhi_;
    vector<double>  *patMetSig_;

    // List of branches
    TBranch        *b_mcEveWeight_;   //!
    TBranch        *b_mcSherpaSumWeight3_;   //!
    TBranch        *b_mcSherpaWeights_;   //!
    TBranch        *b_nup_;   //!
    TBranch        *b_pdfInfo_;   //!
    TBranch        *b_PU_npT;   //!
    TBranch        *b_EvtInfo_NumVtx;   //!
    TBranch        *b_EvtInfo_RunNum;   //!
    TBranch        *b_EvtInfo_EventNum;   //!
    TBranch        *b_genLepPt_;   //!
    TBranch        *b_genLepEta_;   //!
    TBranch        *b_genLepPhi_;   //!
    TBranch        *b_genLepE_;   //!
    TBranch        *b_genLepQ_;   //!
    TBranch        *b_genLepId_;   //!
    TBranch        *b_genLepSt_;   //!
    TBranch        *b_genPhoPt_;   //!
    TBranch        *b_genPhoEta_;   //!
    TBranch        *b_genPhoPhi_;   //!
    TBranch        *b_genJetPt_;   //!
    TBranch        *b_genJetEta_;   //!
    TBranch        *b_genJetPhi_;   //!
    TBranch        *b_genJetE_;   //!
    TBranch        *b_dpsParton_Pt;   //!
    TBranch        *b_dpsParton_Eta;   //!
    TBranch        *b_dpsParton_Phi;   //!
    TBranch        *b_dpsParton_E;   //!
    TBranch        *b_genMatchDPSpar;   //!
    TBranch        *b_dpsParton_dR;   //!

    TBranch        *b_gsfElecPt_;   //!
    TBranch        *b_gsfElecEta_;   //!
    TBranch        *b_gsfElecPhi_;   //!
    TBranch        *b_gsfElecEnergy_;   //!
    TBranch        *b_patElecPt_;   //!
    TBranch        *b_patElecEta_;   //!
    TBranch        *b_patElecPhi_;   //!
    TBranch        *b_patElecEn_;   //!
    TBranch        *b_patElecCharge_;   //!
    TBranch        *b_patElecID_;   //!
    TBranch        *b_patElecTrig_;   //!
    TBranch        *b_patElecDetIso_;   //!
    TBranch        *b_patElecPfIsoRho_;   //!
    TBranch        *b_patElecScEta_;   //!
    TBranch        *b_patElecIsPF_;   //!

    TBranch        *b_patMuonPt_;   //!
    TBranch        *b_patMuonEta_;   //!
    TBranch        *b_patMuonPhi_;   //!
    TBranch        *b_patMuonVtxZ_;   //!
    TBranch        *b_patMuonEn_;   //!
    TBranch        *b_patMuonCharge_;   //!
    TBranch        *b_patMuonDxy_;   //!
    TBranch        *b_patMuonCombId_;   //!
    TBranch        *b_patMuonTrig_;   //!
    TBranch        *b_patMuonPfIsoDbeta_;   //!

    TBranch        *b_patJetPfAk05En_;   //!
    TBranch        *b_patJetPfAk05Pt_;   //!
    TBranch        *b_patJetPfAk05Eta_;   //!
    TBranch        *b_patJetPfAk05Phi_;   //!
    TBranch        *b_patJetPfAk05LooseId_;   //!
    TBranch        *b_patJetPfAk05jetBSZ_;   //!
    TBranch        *b_patJetPfAk05jetBZ_;   //!
    TBranch        *b_patJetPfAk05jetpuMVA_;   //!
    TBranch        *b_patJetPfAk05OCSV_;   //!
    TBranch        *b_patJetPfAk05PartonFlavour_;
    TBranch        *b_patMetPt_;   //!
    TBranch        *b_patMetPhi_;   //!
    TBranch        *b_patMetSig_;   //!



    ZJetsAndDPS(string fileName_, float lumiScale_ = 1., float puScale_ = 1., bool useTriggerCorrection_ = 0, bool useEfficiencyCorrection_ = 0, int systematics_ = 0, int direction_ = 0, float xsecfactor_ = 1., int jetPtCutMin_ = 20, int jetPtCutMax_ = 0, int ZPtCutMin_ = 0 , int ZEtaCutMin_ = -999999, int ZEtaCutMax_ = 999999, int METcut_ = -30, bool nEvents_10000_ = 0, int jetEtaCutMin_ = -24, int jetEtaCutMax_ = 24) ; 
    ~ZJetsAndDPS();
    string   CreateOutputFileName(bool, bool, int, bool, int, int, bool, bool , string pdfSet = "", int aa = -1);
    //string   CreateOutputFileName(bool, bool, int, bool, int, int, bool, bool );
    //string   CreateOutputFileName(bool, bool, int, bool, bool, int, bool, bool, int );
    //string   CreateOutputFileName(bool useRoch, bool doFlat, int doPUStudy, bool doVarWidth, bool doBJets, int doQCD, bool doSSign, bool doInvMassCut, int doMETcut);
    Int_t    Cut(Long64_t entry);
    Int_t    GetEntry(Long64_t entry);
    Long64_t LoadTree(Long64_t entry);
    void     Init(bool hasRecoInfo, bool hasGenInfo, bool hasPartonInfo);
//    void     Loop(bool hasRecoInfo = 1, bool hasGenInfo = 0, int doQCD = 0, bool doSSign = 0, bool doInvMassCut = 1 , int doMETcut = 30, bool doBJets = 0, int doPUStudy = -10,bool doFlat = 0, bool useRoch = 0, bool doVarWidth = 1, bool hasPartonInfo = 0);
    //void     Loop(bool hasRecoInfo = 1, bool hasGenInfo = 0, int doQCD = 0, bool doSSign = 0, bool doInvMassCut = 1 ,   int doBJets = 0, int doPUStudy = -10,bool doFlat = 0, bool useRoch = 0, bool doVarWidth = 1, bool hasPartonInfo = 0);
    void     Loop(bool hasRecoInfo = 1, bool hasGenInfo = 0, int doQCD = 0, bool doSSign = 0, bool doInvMassCut = 1 ,   int doBJets = 0, int doPUStudy = -10,bool doFlat = 0, bool useRoch = 0, bool doVarWidth = 1, bool hasPartonInfo = 0, string pdfSet = "", int pdfMember = 0);
    Bool_t   Notify();
    void     Show(Long64_t entry = -1);

    bool nEvents_10000;
    string outputDirectory;
    string fileName; 
    float lumiScale;
    float puScale;
    bool useTriggerCorrection;
    bool useEfficiencyCorrection; 
    bool isData;
    int systematics;
    int direction;
    float xsecfactor;
    int jetPtCutMin;
    int jetPtCutMax;
    int jetEtaCutMin, jetEtaCutMax ;
    int ZPtCutMin;
    int ZEtaCutMin;
    int ZEtaCutMax;
    int METcut;
    string leptonFlavor;

    TH1D *FlatNVtxWeight;

    ClassDef(ZJetsAndDPS,2)
};
#endif

