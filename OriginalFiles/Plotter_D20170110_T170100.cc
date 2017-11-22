// History
//---- 2015_05_17
// Add, as a commented line, the one to use if you do not want to plot QCD contribution in the Signal region.

#include <iostream>
#include <sstream>
#include <vector>
#include <TFile.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <TLegend.h>
#include "getFilesAndHistograms.h"

//--  Setting global variables --------------------------------------------------------------
#include "fileNames.h"
//-------------------------------------------------------------------------------------------

using namespace std;

void Plotter(string leptonFlavor = "Muons", int JetPtMin = 30,
        int doQCD = 0, bool doSSign = 0, bool doInvMassCut = 0, int MET = 0 , int doBJets = -1 , 
        int JetPtMax = 0, int ZEtaMin = -999999, int ZEtaMax = 999999, 
        bool doRoch = 0, bool doFlat = 0, bool doVarWidth = 1)
{
    string energy = getEnergy();

    cout << endl << "Running the Plotter with the following files as input: " <<doQCD << "   " << MET << "   " << doBJets << endl;
    TH1::SetDefaultSumw2();
    gStyle->SetOptStat(0);

    ostringstream JetPtMinStr;  JetPtMinStr << JetPtMin;
    ostringstream JetPtMaxStr;  JetPtMaxStr << JetPtMax;
    ostringstream ZEtaMinStr;   ZEtaMinStr << abs(ZEtaMin); 
    ostringstream ZEtaMaxStr;   ZEtaMaxStr << abs(ZEtaMax); 
    ostringstream doQCDStr;     doQCDStr << doQCD ;
    ostringstream METStr;   METStr << MET ; 
    string year = "2012"; 
    if (energy == "7TeV") year = "2011";
    int Colors[] = {kBlack, kSpring+5, kOrange, kOrange-3, kRed+1, kPink-6, kViolet+5, kPink, kAzure+4, kBlue, kCyan+1, kCyan+1, kCyan+1}; 
    string legendNames[] = {
        " #mu#mu ", " ZZJets2L2Nu", " ZZJets4L", " ZZJets2L2Q", 
        " WZJets3LNu", " WZJets2L2Q", " WWJets2L2Nu", " Single Top", 
        " DYtautau", " TTJets", " DYJets MD", " DYJets Po", " DYJets Sh"
    };
    if (leptonFlavor == "Electrons") legendNames[0] = " ee ";
    else if  (leptonFlavor == "Muons") legendNames[0] = " #mu#mu ";
    else if  (leptonFlavor == "SMuE") legendNames[0] = " #mue ";
    else if  (leptonFlavor == "SMu" || leptonFlavor == "Muon" ) legendNames[0] = " #mu#nu ";
    else if  (leptonFlavor == "SE" || leptonFlavor == "Electron") legendNames[0] = " e#nu ";

    //    legendNames[0] += "Data";

    //int ColorsTTbar[] = {kBlack, kOrange,  kRed+1,  kViolet+5, kPink, kBlue, kCyan+1, kCyan+1, kCyan+1,kGreen};

    //-- get the files ------------------------------------
    int nFiles = NFILESDYJETS;
    bool isDoubleLep(1);
    if ( leptonFlavor == "SMuE" || leptonFlavor == "SMu" || leptonFlavor == "Muon" || leptonFlavor == "Electron") {
        isDoubleLep = 0;
        nFiles = NFILESTTBAR; 
    }
    TFile *file[nFiles];
    int countFiles = 0 ;
    for (unsigned short i = 0; i < nFiles; i++){
        int fileSelect = FilesDYJets[i] ;
        if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
        if ( leptonFlavor == "SMuE"  ) fileSelect = FilesTTbar[i] ;

        //if (!isDoubleLep) fileSelect = FilesTTbarWJets[i] ;
        string fileNameTemp =  ProcessInfo[fileSelect].filename ; 
        cout << "Is double lepton:" << isDoubleLep << "   " << leptonFlavor <<"   " << fileNameTemp << endl;
        if ((doQCD > 0 || doInvMassCut || doSSign ) && fileNameTemp.find("QCD") != string::npos) continue;
        //if (fileNameTemp.find("QCD") != string::npos) continue; // use this line if you do not want to plot QCD
        file[countFiles] = getFile(FILESDIRECTORY, leptonFlavor, energy, fileNameTemp, JetPtMin, JetPtMax, doFlat, doVarWidth, doQCD, doSSign,    doInvMassCut, MET, doBJets);

        if ( i == 0 ){
            if (leptonFlavor == "Electrons") legendNames[0] = " ee ";
            else if  (leptonFlavor == "Muons") legendNames[0] = " #mu#mu ";
            else if  (leptonFlavor == "SMuE") legendNames[0] = " #mue ";
            else if  (leptonFlavor == "SMu" || leptonFlavor == "Muon" ) legendNames[0] = " #mu#nu ";
            else if  (leptonFlavor == "SE" || leptonFlavor == "Electron") legendNames[0] = " e#nu ";
            legendNames[0] += "Data";
        }
        else legendNames[countFiles] = ProcessInfo[fileSelect].legend; 
        Colors[countFiles] = ProcessInfo[fileSelect].color;    
        countFiles++;
    }
    nFiles = countFiles ;
    //-----------------------------------------------------

    string outputFileName = "../../PNGFiles/wplus/Systematics/Comparison_" + leptonFlavor + "_" + energy + "_Data_All_MC_";

    outputFileName += "JetPtMin_" + JetPtMinStr.str();
    if (JetPtMax > JetPtMin) outputFileName += "_JetPtMax_" + JetPtMaxStr.str();
    if (ZEtaMin > -999999 && ZEtaMin <  0 ) outputFileName += "_ZEtaMin_m" + ZEtaMinStr.str();
    if (ZEtaMin > -999999 && ZEtaMin >= 0 ) outputFileName += "_ZEtaMin_" + ZEtaMinStr.str();
    if (ZEtaMax <  999999 && ZEtaMax >= 0 ) outputFileName += "_ZEtaMax_" + ZEtaMaxStr.str();
    if (ZEtaMax <  999999 && ZEtaMax <  0 ) outputFileName += "_ZEtaMax_m" + ZEtaMaxStr.str();
    if (doRoch) outputFileName += "_rochester";
    if (doFlat) outputFileName += "_Flat";
    if (doVarWidth) outputFileName += "_VarWidth";
    if (doInvMassCut) outputFileName +=  "_InvMass";
    if (doSSign )   outputFileName += "_SS";
    if (doBJets > 0 ) outputFileName += "_BJets";
    if (doBJets < 0 ) outputFileName += "_BVeto";

    if (doQCD>0) outputFileName += "_QCD" + doQCDStr.str();
    if ( MET > 0 ) outputFileName += "_MET"+METStr.str();

    //if (doBJets) nameStr << "_BJets";

    /*    if (doQCD > 0 ){
          if ( leptonFlavor == "SMuE") outputFileName +="_SS";
          else  outputFileName += "_QCD" + doQCDStr.str();
          }
          */
    outputFileName += "_SFInvers";
    if (doInvMassCut) outputFileName += "_InvMass";
    // create the directory if it doesn't exist
    string command = "mkdir -p " + outputFileName;
    system(command.c_str());
    string outputFileRoot = outputFileName + ".root";

    cout << "Output directory is: " << outputFileName << endl;
    cout << "Output root file is: " << outputFileRoot << endl;

    TFile *outputFile = new TFile(outputFileRoot.c_str(), "RECREATE");
    outputFile->cd();

    unsigned short nHist = file[0]->GetListOfKeys()->GetEntries();
    //vector<string> histoName(nHist);
    //vector<string> histoTitle(nHist);
    vector<string> histoName;
    vector<string> histoTitle;
    string histoNameTemp;
    TCanvas *canvas[nHist];
    TPad *pad1[nHist];
    TPad *pad2[nHist];

    TH1D *hist[25][nHist];
    TH1D *histTemp;
    THStack *histSumMC[nHist];
    TLegend *legend[nHist];
    TLatex *cmsColl[nHist];
    TLatex *jetAlgo[nHist];
    TLatex *jetCuts[nHist];
    TLatex *intLumi[nHist];
    int nHistNoGen=0;
    for (unsigned short i(0); i < nHist; i++) {
        histoNameTemp = file[0]->GetListOfKeys()->At(i)->GetName();
        if (histoNameTemp.find("gen") != string::npos) continue;
        histTemp = (TH1D*) file[0]->Get(histoNameTemp.c_str());
        if (histTemp->GetEntries() < 1) continue;
        if (!histTemp->InheritsFrom(TH1D::Class())) continue;

        //        histoName[nHistNoGen] = file[0]->GetListOfKeys()->At(i)->GetName();
        //        histoTitle[nHistNoGen] = file[0]->GetListOfKeys()->At(i)->GetTitle();
        histoName.push_back(file[0]->GetListOfKeys()->At(i)->GetName());
        histoTitle.push_back(file[0]->GetListOfKeys()->At(i)->GetTitle());
        histSumMC[nHistNoGen] = new THStack(histoName[nHistNoGen].c_str(), histoTitle[nHistNoGen].c_str());
        double xLowLeg(0.74), xHighLeg(0.78);
        legend[nHistNoGen] = new TLegend(xLowLeg, 0.54, xHighLeg, 0.91);
        legend[nHistNoGen]->SetFillStyle(0);
        legend[nHistNoGen]->SetBorderSize(0);
        legend[nHistNoGen]->SetTextSize(0.032);
        legend[nHistNoGen]->SetTextFont(42);

        cmsColl[nHistNoGen] = new TLatex();
        cmsColl[nHistNoGen]->SetTextSize(0.05);
        cmsColl[nHistNoGen]->SetTextFont(62);
        cmsColl[nHistNoGen]->SetLineWidth(2);
        cmsColl[nHistNoGen]->SetTextColor(kBlack);
        cmsColl[nHistNoGen]->SetNDC();
        cmsColl[nHistNoGen]->SetTextAlign(11);

        intLumi[nHistNoGen] = new TLatex();
        intLumi[nHistNoGen]->SetTextSize(0.035);
        intLumi[nHistNoGen]->SetTextFont(42);
        intLumi[nHistNoGen]->SetLineWidth(2);
        intLumi[nHistNoGen]->SetTextColor(kBlack);
        intLumi[nHistNoGen]->SetNDC();
        intLumi[nHistNoGen]->SetTextAlign(11);

        jetAlgo[nHistNoGen] = new TLatex();
        jetAlgo[nHistNoGen]->SetTextSize(0.035);
        jetAlgo[nHistNoGen]->SetTextFont(42);
        jetAlgo[nHistNoGen]->SetLineWidth(2);
        jetAlgo[nHistNoGen]->SetTextColor(kBlack);
        jetAlgo[nHistNoGen]->SetNDC();
        jetAlgo[nHistNoGen]->SetTextAlign(11);

        jetCuts[nHistNoGen] = new TLatex();
        jetCuts[nHistNoGen]->SetTextSize(0.035);
        jetCuts[nHistNoGen]->SetTextFont(42);
        jetCuts[nHistNoGen]->SetLineWidth(2);
        jetCuts[nHistNoGen]->SetTextColor(kBlack);
        jetCuts[nHistNoGen]->SetNDC();
        jetCuts[nHistNoGen]->SetTextAlign(11);

        nHistNoGen++;
    } 
    nHist=nHistNoGen; 
    cout <<"Number of histograms:" << nHist << " and we plot :" << nHistNoGen << endl;
    //nHist=4;
    for (int i = 0; i < nFiles; i++) {
        cout << i <<"  "<<legendNames[i]  << "   "<<  endl;
        for (int j = 0; j < nHistNoGen ; j++) {
            // cout << i <<"  "<<legendNames[i]  << "   "<< j << "   " <<  histoName[j] << endl;
            hist[i][j] = getHisto(file[i], histoName[j]);
            hist[i][j]->SetTitle(histoTitle[j].c_str());
            if ( i == 0) {
                hist[i][j]->SetMarkerStyle(20);
                hist[i][j]->SetMarkerColor(Colors[i]);
                hist[i][j]->SetLineColor(Colors[i]);
            }
            else {
                hist[i][j]->SetFillColor(Colors[i]);
                hist[i][j]->SetLineColor(Colors[i]);
                legend[j]->AddEntry(hist[i][j], legendNames[i].c_str(), "f");
            }
        }
    }
    
    for (int i = 1; i < nFiles; i++) {
        for (int j = 0; j < nHistNoGen ; j++) {
            if (doBJets <= 0 ){
                histSumMC[j]->Add(hist[i][j]);
            }
            else {
                if (i == nFiles - 2) histSumMC[j]->Add(hist[nFiles - 1][j]);
                else if (i == nFiles - 1) histSumMC[j]->Add(hist[nFiles - 2][j]);
                else histSumMC[j]->Add(hist[i][j]);
            }
        }
    }
    
    cout << " added all histograms " << endl;

    for (unsigned short i(0); i < nHistNoGen; i++) {
        if (!file[0]->Get(histoName[i].c_str())->InheritsFrom(TH1D::Class())) continue;
        //cout << histoName[i] << endl;
        unsigned short nBins(hist[0][i]->GetNbinsX());
        legend[i]->AddEntry(hist[0][i], legendNames[0].c_str(), "ep");
        canvas[i] = new TCanvas(histoName[i].c_str(), histoName[i].c_str(), 700, 900);
        pad1[i] = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
        pad1[i]->SetTopMargin(0.055);
        pad1[i]->SetBottomMargin(0.);
        pad1[i]->SetRightMargin(0.04);
        pad1[i]->SetLeftMargin(0.12);
        pad1[i]->SetTicks();
        pad1[i]->SetLogy();
        pad1[i]->Draw();
        pad1[i]->cd();

        // Need to draw MC Stack first other wise
        // cannot access Xaxis !!!
        histSumMC[i]->Draw("HIST"); 
        if ( (leptonFlavor == "Muons" || leptonFlavor == "DMu" || leptonFlavor == "Electrons" ) && !doInvMassCut ) {  
            if (histoName[i].find("ZMass_Z") != string::npos){
                hist[0][i]->GetXaxis()->SetRangeUser(71,111);
                histSumMC[i]->GetXaxis()->SetRangeUser(71,111);
            }
            if (histoName[i].find("JetEta") != string::npos){
                hist[0][i]->GetXaxis()->SetRangeUser(-2.4,2.4);
                histSumMC[i]->GetXaxis()->SetRangeUser(-2.4,2.4);

            }
        }

        hist[0][i]->SetTitle("");
        histSumMC[i]->SetTitle(""); 
        histSumMC[i]->GetYaxis()->SetLabelSize(0.05); 
        histSumMC[i]->GetYaxis()->SetLabelOffset(0.002); 
        histSumMC[i]->GetYaxis()->SetLabelFont(42); 
        histSumMC[i]->GetYaxis()->SetTitle("# Events"); 
        histSumMC[i]->GetYaxis()->SetTitleFont(42); 
        histSumMC[i]->GetYaxis()->SetTitleSize(0.05); 
        histSumMC[i]->GetYaxis()->SetTitleOffset(1.2); 
        histSumMC[i]->SetMinimum(8);
        histSumMC[i]->SetMaximum(110*histSumMC[i]->GetMaximum()); 

        /// first pad plots
        hist[0][i]->DrawCopy("e same");
        legend[i]->Draw();
        cmsColl[i]->DrawLatex(0.2,0.87, "CMS Preliminary");
        if (energy == "7TeV") intLumi[i]->DrawLatex(0.16,0.82, "#int L dt = 5.05 fb^{-1},  #sqrt{s} = 7 TeV");
        if (energy == "8TeV") intLumi[i]->DrawLatex(0.16,0.82, "#int L dt = 19.6 fb^{-1},  #sqrt{s} = 8 TeV");
        if ( histoName[i].find("inc0") == string::npos){
            ostringstream ptLegend;
            ptLegend << "p_{T}^{jet} > " << JetPtMin << "GeV,  |#eta^{jet}| < 2.4";
            jetCuts[i]->DrawLatex(0.16,0.72, ptLegend.str().c_str());
            jetAlgo[i]->DrawLatex(0.16,0.77, "anti-k_{t} jets,  R = 0.5");
            pad1[i]->Draw();
        }
        canvas[i]->cd();
        pad2[i] = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
        pad2[i]->SetTopMargin(0.);
        pad2[i]->SetBottomMargin(0.26);
        pad2[i]->SetRightMargin(0.04);
        pad2[i]->SetLeftMargin(0.12);
        pad2[i]->SetGridy();
        pad2[i]->SetTicks();
        pad2[i]->Draw();
        pad2[i]->cd();

        hist[0][i]->SetStats(0);
        hist[0][i]->SetTitle("");

        hist[0][i]->GetXaxis()->SetTickLength(0.03);
        hist[0][i]->GetXaxis()->SetTitleSize(0.11);
        hist[0][i]->GetXaxis()->SetTitleOffset(1.05);
        hist[0][i]->GetXaxis()->SetTitleFont(42); 
        hist[0][i]->GetXaxis()->SetLabelSize(0.09);
        hist[0][i]->GetXaxis()->SetLabelOffset(0.018);
        hist[0][i]->GetXaxis()->SetLabelFont(42); 

        hist[0][i]->GetYaxis()->SetRangeUser(0.51,1.49);
        hist[0][i]->GetYaxis()->SetNdivisions(5,5,0);
        hist[0][i]->GetYaxis()->SetTitle("MC / Data");
        hist[0][i]->GetYaxis()->SetTitleFont(42);
        hist[0][i]->GetYaxis()->SetTitleSize(0.12);
        hist[0][i]->GetYaxis()->SetTitleOffset(0.5);
        hist[0][i]->GetYaxis()->CenterTitle();
        hist[0][i]->GetYaxis()->SetLabelSize(0.1);
        hist[0][i]->GetYaxis()->SetLabelFont(42); 

        hist[0][i]->Divide((TH1D*) histSumMC[i]->GetStack()->Last());
        for (unsigned short j(1); j <= nBins; j++){
            double content(hist[0][i]->GetBinContent(j));
            if (content > 0){
                hist[0][i]->SetBinContent(j, 1./content);
            }
        }

        hist[0][i]->DrawCopy("EP");
        canvas[i]->cd();

        string outputFilePNG = outputFileName + "/" + histoName[i] + ".pdf";
        canvas[i]->Print(outputFilePNG.c_str());
        outputFile->cd();
        canvas[i]->Write();

        histSumMC[i]->SetMaximum(1.5*histSumMC[i]->GetMaximum());
        TCanvas *tmpCanvas = (TCanvas*) canvas[i]->Clone();
        tmpCanvas->cd();
        tmpCanvas->Draw();
        TPad *tmpPad = (TPad*) tmpCanvas->GetPrimitive("pad1");
        tmpPad->SetLogy(0);
        histoName[i] += "_Lin";
        tmpCanvas->SetTitle(histoName[i].c_str());
        tmpCanvas->SetName(histoName[i].c_str());
        string outputFileLinPNG = outputFileName + "/" + histoName[i] + ".pdf";
        tmpCanvas->Print(outputFileLinPNG.c_str());
        outputFile->cd();
        tmpCanvas->Write();
    }
    outputFile->cd();
    outputFile->Close();

    cout << "I m closing the files" << endl;
    //-- Close all the files ------------------------------
    for (unsigned short i(0); i < nFiles; i++) closeFile(file[i]);
    //-----------------------------------------------------
    cout << "Everything went fine" << endl;

}
