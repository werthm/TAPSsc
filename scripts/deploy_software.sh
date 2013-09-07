#!/bin/bash

#####################################################################
##                                                                 ##
## Script to deploy the TAPSsc software on all VME controllers     ##
##                                                                 ##
#####################################################################

HOSTS=`grep vme-taps $TAPSSC/config/config.rootrc | cut -f2 -d: | tr -d ' '`

parallel ' \
    printf "\nDeploying software to %s\n" {} 
    ssh a2cb@{} rm -rf TAPSsc 
    ssh a2cb@{} mkdir TAPSsc 
    ssh a2cb@{} mkdir TAPSsc/bin 
    ssh a2cb@{} mkdir TAPSsc/scripts 
    ssh a2cb@{} mkdir TAPSsc/config 
    scp $HOME/TAPSsc/bin/TAPSServer a2cb@{}:TAPSsc/bin 
    scp $HOME/TAPSsc/scripts/control_TAPSServer.sh a2cb@{}:TAPSsc/scripts 
    scp $HOME/TAPSsc/config/config.rootrc a2cb@{}:TAPSsc/config
    if [ "{}" == "vme-taps-trigger" ]
    then
        scp $HOME/TAPSsc/bin/HVTalk a2cb@{}:TAPSsc/bin
    fi
' ::: $HOSTS

