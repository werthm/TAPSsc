#!/bin/bash

#####################################################################
##                                                                 ##
## Script to reboot all VME controllers                            ##
##                                                                 ##
#####################################################################

for i in {0..10} 
do
    printf "taps-vme-%d : \n" $i 
    ssh a2cb@taps-vme-$i sudo reboot
done

