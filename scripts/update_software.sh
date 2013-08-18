#!/bin/bash

#####################################################################
##                                                                 ##
## Script to update the TAPSsc software on all VME controllers     ##
##                                                                 ##
#####################################################################

for i in {0..10} 
do
    printf "\nUpdating software on taps-vme-%d\n\n" $i 
    rsync -vauh --exclude=".svn" $HOME/TAPSsc a2cb@taps-vme-$i:
done
