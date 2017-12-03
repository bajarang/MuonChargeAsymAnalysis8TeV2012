#!/bin/bash

#///////////////////////////////////////////////////////////////////////////////////////////////////////#
#                                                                                                       #
# Execute based on the arguments passed                                                                 #
# arguments : WCHARGE VARIABLE RANGE CLOSURETEST SYSTEMATICS DIRECTION                                  #
# Eg. : source pass_Charge_Var_Range_CT_Syst_Dir_runMyFastPlots_named1.sh 1 lepEta_Zinc0jet 25-30 1 0 0 # 
#///////////////////////////////////////////////////////////////////////////////////////////////////////#

#------------------------------------------------#
echo $#
if [ "$#" -ne 5 ] && [ "$#" -ne 6 ] 
then
  echo "Illegal number of arguments"
  echo "Please provide 6 arguments"
  exit
fi

WCHARGE=$1
VARIABLE=$2
CLOSURETEST=$3
SYSTEMATICS=$4
DIRECTION=$5
RANGE=$6

echo "WCHARGE : " $WCHARGE "  VARIABLE : " $VARIABLE "  CLOSURETEST : " $CLOSURETEST "  SYSTEMATICS : " $SYSTEMATICS "  DIRECTION : " $DIRECTION "  RANGE : " $RANGE  

root -b -q 'runMyFastPlots_named1.cc("'$WCHARGE'","'$VARIABLE'","'$CLOSURETEST'","'$SYSTEMATICS'","'$DIRECTION'","'$RANGE'")'
