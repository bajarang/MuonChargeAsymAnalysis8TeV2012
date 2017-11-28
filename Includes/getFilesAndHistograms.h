#ifndef _getFilesAndhistograms_h_
#define _getFilesAndhistograms_h_

#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <RooUnfoldResponse.h>
#include <string>
#include <vector>
using namespace std;

string getEnergy();
TFile* getFile(string histoFilesDirectory, string leptonFlavor, string energy, string Name, int charge, int systematics, int direction, int doBJets, int doQCD, int MET, int mT, string type, int JetPtMin = 30, int JetPtMax = 0, bool doFlat = false, bool doVarWidth = false, bool doSSign = false , bool doInvMassCut = false , string closureTest = "", bool useRoch = false, bool dodR = false, bool useUnfoldingFiles = false ); //22 arguments
void getFiles(string, TFile *Files[], string, string, string, int JetPtMin = 20, int JetPtMax = 0, bool doFlat = false ,  bool doVarWidth = false , int doQCD = 0,  bool doSSign = false , bool doInvMassCut = false, int MET = 0  , int doBJets = 0 , bool useUnfoldingFiles = false );
void closeFile(TFile*);
void closeFiles(TFile *Files[]);
void closeFiles(TFile *Files[], int nFiles);
TH1D* getHisto(TFile*, string);
void getHistos(TH1D *histograms[], TFile *Files[], string, bool isDoubleLep = true );
void getResp(RooUnfoldResponse*, TFile*, string);
RooUnfoldResponse* getResp(TFile*, string);
void getResps(RooUnfoldResponse *responses[], TFile *Files[], string);
void getStatistics(string leptonFlavor = "Muons",  int JetPtMin = 30, int JetPtMax = 0,  bool doFlat = false , bool doVarWidth = true, int doQCD = 0 , bool doSSign = false ,  bool doInvMassCut = false , int MET = 0 , int doBjets = 0 );
#endif

