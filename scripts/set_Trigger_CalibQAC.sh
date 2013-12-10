#!/bin/bash

#####################################################################
##                                                                 ##
## Script to control all AcquDAQ instances                         ##
##                                                                 ##
#####################################################################

LOC="/opt/TAPSsc"

IS_VME=`hostname | grep vme | wc -l`
if [ $IS_VME -eq 0 ] 
then
    ssh root@vme-taps-trigger $LOC/scripts/set_Trigger_CalibQAC.sh
else
    # ROOT
    export ROOTSYS=/opt/root
    export PATH="$ROOTSYS/bin:$PATH"
    export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"
    
    # kill all AcquDAQ
    killall -q AcquDAQ

    # program TAPS standalone trigger with high pulser rate
    cd $HOME/acqu
    #AcquDAQ AcquDAQ.TestQAC &> /dev/null &
    /opt/acqu/build/bin/AcquDAQ AcquDAQ.CalibQAC &
    PN=`jobs -p`
    sleep 5
    kill $PN
    wait $PN 2>/dev/null

    # set TAPS busy to 0
    /opt/a2vme/build/bin/vmeext 0x0a002410 0 w
fi

