#ifndef _DataDrivenQCD_h_
#define _DataDrivenQCD_h_

#include <iostream>

using namespace std;

void DataDrivenQCD(string leptonFlavor, int charge, int systematics, int direction, int JetPtCutMin, int doBJets, int MET, int mT, string type);

void FuncOpenAllFiles(TFile *fData[], TFile *fMC[][14], string leptonFlavor,  int charge, int systematics, int direction, int JetPtCutMin, int doBJets, int MET, int mT, string type);

vector<string> getVectorOfHistoNames(TFile *fData[]);

void FuncDataDrivenQCD(string variable, TFile *fData[], TFile *fMC[][14], TFile*);

#endif

