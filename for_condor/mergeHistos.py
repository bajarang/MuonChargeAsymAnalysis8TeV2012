
#//////////////////////////////////////////////////
#                                                 #
# Copy Ntuple root files from T2_IN_TIFR to T3.   #
# Merge them into a single root file at T3.       #
# Send the merged file back to T2_IN_TIFR.        #
# Store the full path of merged file for Analysis #
#                                                 #
#//////////////////////////////////////////////////

import os
import sys
import datetime

#USERS INPUTS
isMuData = True
isEleData = False

isMC = True
mc = [
    "TTJetsM",
    "TTJetsP",
    "HplusM120",
    "ST_tW",
    "ST_t_",
    "ST_s",
    "WJetsToLNu",
    "W1JetsToLNu",
    "W2JetsToLNu",
    "W3JetsToLNu",
    "W4JetsToLNu",
    "DYJetsToLL",
    "DY1JetsToLL",
    "DY2JetsToLL",
    "DY3JetsToLL",
    "DY4JetsToLL",
    "WW",
    "WZ",
    "ZZ",
    "QCD_Pt-15to20_Mu",
    "QCD_Pt-20to30_Mu",
    "QCD_Pt-30to50_Mu",
    "QCD_Pt-50to80_Mu",
    "QCD_Pt-80to120_Mu",
    "QCD_Pt-120to170_Mu",
    "QCD_Pt-170to300_Mu",
    "QCD_Pt-300to470_Mu",
    "HplusM80",
    "HplusM90",
    "HplusM100",
    "HplusM120",
    "HplusM140",
    "HplusM150",
    "HplusM155",
    "HplusM160"]

if(isMuData):
	data = ["MuRunB2v2", "MuRunCv1", "MuRunDv1", "MuRunEv1", "MuRunFv1", "MuRunGv1", "MuRunH2v1", "MuRunH3v1"]

if(isEleData): 
	data = ["EleRunBver2v2", "EleRunCv1", "EleRunDv1", "EleRunEv1", "EleRunFv1", "EleRunGv1", "EleRunHver2v1", "EleRunHver3v1"]

#-------------------------------
def execme(command):
    print ""
    print "\033[01;32m"+ "Excecuting: "+ "\033[00m",  command
    print ""
    os.system(command)


if isMC:
    for samp in range(len(mc)):
        execme("hadd -k "+str(mc[samp])+"_Merged.root "+str(mc[samp])+"*")

if(isMuData):
    for samp in range(len(data)):
        execme("hadd -k "+str(data[samp])+"_Merged.root "+str(data[samp])+"*")
    execme("hadd -k all_muData.root MuRun*_Merged.root")

if(isEleData):
    for samp in range(len(data)):
        execme("hadd -k "+str(data[samp])+"_Merged.root "+str(data[samp])+"*")
    execme("hadd -k all_EleData.root EleRun*_Merged.root")

execme("hadd -k all_ST.root ST*_Merged.root")
execme("hadd -k all_TTJetsM.root TTJetsM_Merged.root")
execme("hadd -k all_TTJetsP.root TTJetsP_Merged.root")
execme("hadd -k all_Hplus80.root HplusM80*_Merged.root")
execme("hadd -k all_Hplus90.root HplusM90*_Merged.root")
execme("hadd -k all_Hplus100.root HplusM100*_Merged.root")
execme("hadd -k all_Hplus120.root HplusM120*_Merged.root")
execme("hadd -k all_Hplus140.root HplusM140*_Merged.root")
execme("hadd -k all_Hplus150.root HplusM150*_Merged.root")
execme("hadd -k all_Hplus155.root HplusM155*_Merged.root")
execme("hadd -k all_Hplus160.root HplusM160*_Merged.root")
execme("hadd -k all_QCD.root QCD*_Merged.root")
execme("hadd -k all_VV.root WW_Merged.root WZ_Merged.root ZZ_Merged.root")
execme("hadd -k all_WJets.root W1*_Merged.root W2*_Merged.root W3*_Merged.root W4*_Merged.root WJ*_Merged.root")
execme("hadd -k all_DY.root DY1*_Merged.root DY2*_Merged.root DY3*_Merged.root DY4*_Merged.root DYJ*_Merged.root")

execme("mkdir merged_histos")
execme("cp *_Merged.root merged_histos")
execme("cp all* merged_histos")
execme("tar -czvf merged_histos.tar.gz merged_histos")


