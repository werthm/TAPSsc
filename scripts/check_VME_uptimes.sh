#!/bin/bash

#####################################################################
##                                                                 ##
## Script to check the times on all VME controllers                ##
##                                                                 ##
#####################################################################

for i in {0..10} 
do
    printf "taps-vme-%d : " $i 
    ssh a2cb@taps-vme-$i uptime
done

