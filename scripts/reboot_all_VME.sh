#!/bin/bash

#####################################################################
##                                                                 ##
## Script to reboot all VME controllers                            ##
##                                                                 ##
#####################################################################

HOSTS=`grep vme-taps $TAPSSC/config/config.rootrc | cut -f2 -d: | tr -d ' '`

for i in $HOSTS
do
    printf "%-20s : \n" $i 
    ssh root@$i reboot
done

