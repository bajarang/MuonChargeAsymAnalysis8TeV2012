# MuonChargeAsymAnalysis8TeV2012
The central repo for Muon Charge Asymmetry @ 8TeV

Instructions to follow for setting up the framework  
---------------------------------------------------  

A. Get this CMSSW release :  
   $ cmsrel CMSSW_5_3_11  
   $ cd CMSSW_5_3_11/src  
   $ cmsenv  

B. Clone this git repo  :  
   $ git clone https://github.com/bajarang/MuonChargeAsymAnalysis8TeV2012.git  

C. Set up the RooUnfold package :  
   $ cd RooUnfold-1.1.1  
   $ make clean   
   $ make  

D. Run it from the condor framework  
