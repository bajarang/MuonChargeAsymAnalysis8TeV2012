#!/bin/bash
# short.sh: a short discovery job
printf "Start time: "; /bin/date
printf "Job is running on node: "; /bin/hostname
printf "Job running as user: "; /usr/bin/id
printf "Job is running in directory: "; /bin/pwd

cd /afs/cern.ch/work/b/bsutar/private/NEWWJETS/WJETS/CMSSW_5_3_11/src/ 
source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
echo $_CONDOR_SCRATCH_DIR
cd $_CONDOR_SCRATCH_DIR

cp -r /afs/cern.ch/work/b/bsutar/private/NEWWJETS/WJETS/CMSSW_5_3_11/src/TreeAnalysis2012/WJetsp_CENTRAL_QCD0 .
cd WJetsp_CENTRAL_QCD0
ls -ltrh runDYJets.cc
root -b -q runDYJets.cc++

echo "DONE"
ls ${_CONDOR_SCRATCH_DIR}
cp -p $_CONDOR_SCRATCH_DIR/WJetsp_CENTRAL_QCD0/Results/HistoFiles/MySetOfCuts/MuPt25/wplus/FinerBins/Systematics/Trial/* /afs/cern.ch/work/b/bsutar/private/NEWWJETS/WJETS/CMSSW_5_3_11/src/Results/HistoFiles/MySetOfCuts/MuPt25/wplus/FinerBins/Systematics/Trial/Set0000/
cp -p $_CONDOR_SCRATCH_DIR/WJetsp_CENTRAL_QCD0/ProcessVariables.txt /afs/cern.ch/work/b/bsutar/private/NEWWJETS/WJETS/CMSSW_5_3_11/src/Results/HistoFiles/MySetOfCuts/MuPt25/wplus/FinerBins/Systematics/Trial/Set0000/
cp -p $_CONDOR_SCRATCH_DIR/WJetsp_CENTRAL_QCD0/KinematicVariables.txt /afs/cern.ch/work/b/bsutar/private/NEWWJETS/WJETS/CMSSW_5_3_11/src/Results/HistoFiles/MySetOfCuts/MuPt25/wplus/FinerBins/Systematics/Trial/Set0000/
ls -d $_CONDOR_SCRATCH_DIR/WJetsp_CENTRAL_QCD0
rm -rf $_CONDOR_SCRATCH_DIR/WJetsp_CENTRAL_QCD0 
  
