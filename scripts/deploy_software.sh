#!/bin/bash

#####################################################################
##                                                                 ##
## Script to deploy the TAPSsc software on all VME controllers     ##
##                                                                 ##
#####################################################################

for i in {0..10} 
do
    printf "\nDeploying software to taps-vme-%d\n\n" $i 
    ssh a2cb@taps-vme-$i rm -rf TAPSsc
    ssh a2cb@taps-vme-$i mkdir TAPSsc
    ssh a2cb@taps-vme-$i mkdir TAPSsc/bin
    ssh a2cb@taps-vme-$i mkdir TAPSsc/scripts
    ssh a2cb@taps-vme-$i mkdir TAPSsc/config
    scp $HOME/TAPSsc/bin/TAPSServer a2cb@taps-vme-$i:TAPSsc/bin
    scp $HOME/TAPSsc/scripts/control_TAPSServer.sh a2cb@taps-vme-$i:TAPSsc/scripts
    scp $HOME/TAPSsc/config/config.rootrc a2cb@taps-vme-$i:TAPSsc/config

    if [ $i -eq 0 ]
    then
        scp $HOME/TAPSsc/bin/HVTalk a2cb@taps-vme-$i:TAPSsc/bin
    fi
done

