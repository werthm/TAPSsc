#!/bin/bash

#####################################################################
##                                                                 ##
## Script to check the software on all VME controllers             ##
##                                                                 ##
#####################################################################

HOSTS=`grep vme-taps $TAPSSC/config/config.rootrc | cut -f2 -d: | tr -d ' '`

for i in $HOSTS
do
    printf "\n\n%s\n\n\n" $i 
    ssh a2cb@$i ls -l -R TAPSsc
done

