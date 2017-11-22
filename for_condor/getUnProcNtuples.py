
#//////////////////////////////////////////////////
#                                                 #
# Get total Ntuples tobeprocessed form condor dir #
# Get processed Ntuples from condor output        #
# Get paths of the unprocessed Ntuples            #
#                                                 #
#//////////////////////////////////////////////////


import os
import sys
import datetime


#Sample paths at T2
processedPaths = open("procNtuples.log", "w")

#condor output histo files
print ""
print"----------------------------------------------------"
print" Fetching the un-processed ntuples, please wait ... "
print"----------------------------------------------------"
for line_err in open("outHistoFiles.log"):
    line_err = line_err.strip()
    split_line_err = line_err.split("/")
    line_found = split_line_err[1].replace("_Anal", "")
    #print line_found
    for line_ntuple in open("allNtuples.log"):
        line_ntuple = line_ntuple.strip()
	if line_found in line_ntuple:
	    #print line_ntuple
	    processedPaths.write(line_ntuple+"\n")

##------------------------------------------------
# compare the two file, output the unmatched lines
##------------------------------------------------
def compare(File1,File2):
    with open(File1,'r') as f:
        d=set(f.readlines())

    with open(File2,'r') as f:
        e=set(f.readlines())
    open('unProcNtuples.log','w').close() #Create the file
    with open('unProcNtuples.log','a') as f:
        for line in list(d-e):
           f.write(line)
	   print line

compare("allNtuples.log", "procNtuples.log")


