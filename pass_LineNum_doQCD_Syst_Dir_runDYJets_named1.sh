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

allSystematics="100"
allDirection="100"

if [ -z "$systematics" ] && [ -z "$direction" ]
then
  echo $LineNum $doQCD $allSystematics $allDirection #default systematics = 100
  root -b -q 'runDYJets_named1.cc("'$LineNum'","'$doQCD'","'$allSystematics'","'$allDirection'")'
elif [ -z "$direction" ]
then
  echo $LineNum $doQCD $systematics
  root -b -q 'runDYJets_named1.cc("'$LineNum'","'$doQCD'","'$systematics'","'$direction'")'
else 
  echo $LineNum $doQCD $systematics $direction
  root -b -q 'runDYJets_named1.cc("'$LineNum'","'$doQCD'","'$systematics'","'$direction'")'
fi
