#!/bin/bash

#//////////////////////////////////////////////////#
#                                                  #
# Execute based on the arguments passed            #
# arguments : LineNum doQCD                        #
#           : LineNum doQCD systematics            #  
#           : LineNum doQCD systematics direction  #
#//////////////////////////////////////////////////#

#------------------------------------------------#
WCharge=$1
LineNum=$2
doQCD=$3
systematics=$4
direction=$5

if [ "$#" -ne 1 ] && [ "$#" -ne 2 ] && [ "$#" -ne 3 ] && [ "$#" -ne 4 ] && [ "$#" -ne 5 ]  
then
  echo "Illegal number of parameters"
  exit
fi


echo "WCharge : " $WCharge "  Line Number : " $LineNum "  doQCD : " $doQCD "  systematics : " $systematics "  direction : " $direction

allSystematics="100"
allDirection="100"

if [ -z "$systematics" ] && [ -z "$direction" ]
then
  echo $WCharge $LineNum $doQCD $allSystematics $allDirection #default systematics = 100
  root -b -q 'runDYJets_named1.cc("'$WCharge'",'$LineNum'","'$doQCD'","'$allSystematics'","'$allDirection'")'
elif [ -z "$direction" ]
then
  echo $WCharge $LineNum $doQCD $systematics
  root -b -q 'runDYJets_named1.cc("'$WCharge'","'$LineNum'","'$doQCD'","'$systematics'","'$direction'")'
else 
  echo $WCharge $LineNum $doQCD $systematics $direction
  root -b -q 'runDYJets_named1.cc("'$WCharge'","'$LineNum'","'$doQCD'","'$systematics'","'$direction'")'
fi
