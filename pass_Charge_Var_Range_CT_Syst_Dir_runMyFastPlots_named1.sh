#!/bin/bash

#//////////////////////////////////////////////////////////////////////#
#                                                                      #
# Execute based on the arguments passed                                #
# arguments : WCHARGE VARIABLE RANGE CLOSURETEST SYSTEMATICS DIRECTION #
#//////////////////////////////////////////////////////////////////////#

#------------------------------------------------#
WCharge=$1
VARIABLE=$2
RANGE=$3
CLOSURETEST=$4
SYSTEMATICS=$5
DIRECTION=$6

if [ "$#" -ne 6 ]  
then
  echo "Illegal number of arguments"
  echo "Please provide 6 arguments"
  exit
fi


echo "WCHARGE : " $WCharge "  VARIABLE : " $VARIABLE "  RANGE : " $RANGE "  CLOSURETEST : " $CLOSURETEST "  SYSTEMATICS : " $SYSTEMATICS "  DIRECTION : " $DIRECTION

root -b -q 'runMyFastPlots_named1.cc("'$WCHARGE'","'$VARIABLE'","'$RANGE'","'$CLOSURETEST'","'$SYSTEMATICS'","'$DIRECTION'")'
