#!/bin/bash

#//////////////////////////////////////////////////#
#                                                  #
# Execute based on the arguments passed            #
# arguments : LineNum doQCD                        #
#           : LineNum doQCD systematics            #  
#           : LineNum doQCD systematics direction  #
#//////////////////////////////////////////////////#

#------------------------------------------------#
LineNum=$1
doQCD=$2
systematics=$3
direction=$4

if [ "$#" -ne 2 ] && [ "$#" -ne 3 ] && [ "$#" -ne 4 ] 
then
  echo "Illegal number of parameters"
  exit
fi


echo "Line Number : " $LineNum "  doQCD : " $doQCD "  systematics : " $systematics "  direction : " $direction

allSystematics=100

if [ -z "$systematics" ] && [ -z "$direction" ]
then
  echo $LineNum $doQCD $allSystematics  #default systematics = 100
  ./exec_runDYJets $LineNum $doQCD $allSystematics

elif [ -z "$direction" ]
then
  echo $LineNum $doQCD $systematics
  ./exec_runDYJets $LineNum $doQCD $systematics

else 
  echo $LineNum $doQCD $systematics $direction
  ./exec_runDYJets $LineNum $doQCD $systematics $direction
fi




####for lineNum in {1..34}
####do
####    for doQCD in {0..3}
####    do
####        for i in {0..18}
####        do  
####            echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
####            #echo "Line Number : " $lineNum "  doQCD : " $doQCD "  systematics : " ${systematics[$i]} "  direction : " ${direction[$i]}
####            ./toRead_Input_DB_Summary $lineNum $doQCD ${systematics[$i]} ${direction[$i]}
####        done
####    done
####done
####

#------------------------------------------------
#------------------------------------------------
