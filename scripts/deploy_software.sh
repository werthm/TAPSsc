#!/bin/bash

#####################################################################
##                                                                 ##
## Script to deploy the TAPSsc software on all VME controllers     ##
##                                                                 ##
#####################################################################

SYNC_HOST="vme-taps-trigger"
LOC="/opt"

echo "Deploying software to common VME /opt"
ssh root@$SYNC_HOST mount /opt
ssh root@$SYNC_HOST rm -rf $LOC/TAPSsc 
ssh root@$SYNC_HOST mkdir $LOC/TAPSsc 
ssh root@$SYNC_HOST mkdir $LOC/TAPSsc/bin 
ssh root@$SYNC_HOST mkdir $LOC/TAPSsc/scripts 
ssh root@$SYNC_HOST mkdir $LOC/TAPSsc/config 
ssh root@$SYNC_HOST mkdir $LOC/TAPSsc/lib
scp $HOME/TAPSsc/bin/TAPSServer root@$SYNC_HOST:$LOC/TAPSsc/bin 
scp $HOME/TAPSsc/scripts/control_TAPSServer.sh root@$SYNC_HOST:$LOC/TAPSsc/scripts 
scp $HOME/TAPSsc/config/config.rootrc root@$SYNC_HOST:$LOC/TAPSsc/config
scp $HOME/TAPSsc/config/map.def root@$SYNC_HOST:$LOC/TAPSsc/config
scp $HOME/TAPSsc/config/par.def root@$SYNC_HOST:$LOC/TAPSsc/config
scp $HOME/TAPSsc/bin/HVTalk root@$SYNC_HOST:$LOC/TAPSsc/bin
scp $HOME/TAPSsc/lib/libTAPSsc.so root@$SYNC_HOST:$LOC/TAPSsc/lib
ssh root@$SYNC_HOST umount /opt

