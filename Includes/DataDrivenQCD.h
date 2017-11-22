#ifndef _DataDrivenQCD_h_
#define _DataDrivenQCD_h_

#include <iostream>

using namespace std;

void DataDrivenQCD( string leptonFlavor = "SMu", int METcut = 0,  int doBJets = 0);

void FuncOpenAllFiles(TFile *fData[], TFile *fMC[][14], string leptonFlavor = "SMu",  int METcut = 0, bool doFlat = false, bool doVarWidth = true , int doBJets = 0, string syst = "0" );

vector<string> getVectorOfHistoNames(TFile *fData[]);

void FuncDataDrivenQCD(string variable, TFile *fData[], TFile *fMC[][14], TFile*);

#endif

