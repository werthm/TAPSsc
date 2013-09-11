#!/bin/bash

#####################################################################
##                                                                 ##
## Script to check the times on all VME controllers                ##
##                                                                 ##
#####################################################################

HOSTS=`grep vme-taps $TAPSSC/config/config.rootrc | cut -f2 -d: | tr -d ' '`

for i in $HOSTS
do
    printf "%-20s : " $i 
    ssh root@$i date
done

