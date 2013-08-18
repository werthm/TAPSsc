#!/bin/bash

#####################################################################
##                                                                 ##
## Script to restart all TAPSServer instances                      ##
##                                                                 ##
#####################################################################

# local ROOT
export ROOTSYS=/usr/kernph_x86_64/pkg/root/5.34/gcc4.2
export PATH="$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"

# TAPSsc
export TAPSSC="$HOME/src/ROOT/TAPSsc"
export LD_LIBRARY_PATH="$TAPSSC/lib:$LD_LIBRARY_PATH"
 
# kill old instance
killall -q TAPSServer

# start new instance
nohup $TAPSSC/bin/TAPSServer &> /dev/null &

