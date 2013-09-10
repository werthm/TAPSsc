#!/bin/bash

#####################################################################
##                                                                 ##
## Script to deploy the TAPSsc software on all VME controllers     ##
##                                                                 ##
#####################################################################

HOSTS=`grep vme-taps $TAPSSC/config/config.rootrc | cut -f2 -d: | tr -d ' '`

parallel ' \
    printf "\nDeploying software to %s\n" {} 
    ssh root@{} rm -rf TAPSsc 
    ssh root@{} mkdir TAPSsc 
    ssh root@{} mkdir TAPSsc/bin 
    ssh root@{} mkdir TAPSsc/scripts 
    ssh root@{} mkdir TAPSsc/config 
    scp $HOME/TAPSsc/bin/TAPSServer root@{}:TAPSsc/bin 
    scp $HOME/TAPSsc/scripts/control_TAPSServer.sh root@{}:TAPSsc/scripts 
    scp $HOME/TAPSsc/config/config.rootrc root@{}:TAPSsc/config
    if [ "{}" == "vme-taps-trigger" ]
    then
        scp $HOME/TAPSsc/bin/HVTalk root@{}:TAPSsc/bin
    fi
' ::: $HOSTS

