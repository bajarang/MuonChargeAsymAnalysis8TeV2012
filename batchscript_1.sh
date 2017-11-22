

#!/bin/sh
cd /afs/cern.ch/work/b/bhawan/private/Tom/Tom_new/CMSSW_5_3_0/src/TreeAnalysis2012/WJets
eval `scramv1 runtime -sh`
root -b -q runDYJets.cc++
rfcp *.root /afs/cern.ch/work/b/bhawan/private/Tom/Tom_new/CMSSW_5_3_0/src/TreeAnalysis2012/WJets/HistoFiles/

