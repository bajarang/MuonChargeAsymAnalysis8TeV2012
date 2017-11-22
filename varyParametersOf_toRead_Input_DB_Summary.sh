#!/bin/bash

#///////////////////////////////////////////
#                                          #
#  Iterate over different argument values  #
#  that we pass to toRead_Input_DB_Summary #
#                                          #
#//////////////////////////////////////////

#------------------------------------------------
systematics=( "0" "1" "1"  "2" "2"  "3" "3"  "4" "4"  "5" "5"  "6" "6"  "7" "7"  "8" "8"  "12" "12" )
direction=( "0" "1" "-1" "1" "-1" "1" "-1" "1" "-1" "1" "-1" "1" "-1" "1" "-1" "1" "-1" "1"  "-1" )

for lineNum in {1..34}
do
    for doQCD in {0..3}
    do
        for i in {0..18}
        do  
            echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
            #echo "Line Number : " $lineNum "  doQCD : " $doQCD "  systematics : " ${systematics[$i]} "  direction : " ${direction[$i]}
            ./toRead_Input_DB_Summary $lineNum $doQCD ${systematics[$i]} ${direction[$i]}
        done
    done
done


#------------------------------------------------
#------------------------------------------------
