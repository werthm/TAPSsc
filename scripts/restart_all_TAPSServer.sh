#!/bin/bash

#####################################################################
##                                                                 ##
## Script to restart all TAPSServer instances                      ##
##                                                                 ##
#####################################################################

# ROOT
export ROOTSYS=/opt/root
export PATH="$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"

# TAPSsc
export TAPSSC="$HOME/TAPSsc"
export LD_LIBRARY_PATH="$TAPSSC/lib:$LD_LIBRARY_PATH"
 
# kill old instance
killall -q TAPSServer

# start new instance
nohup $TAPSSC/bin/TAPSServer &> /dev/null &

