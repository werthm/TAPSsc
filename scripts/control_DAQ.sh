#!/bin/bash

#####################################################################
##                                                                 ##
## Script to control all AcquDAQ instances                         ##
##                                                                 ##
#####################################################################

LOC="/opt/TAPSsc"

if [ $# -eq 1 ]
then
    if [ "$1" == "restart" ] || [ "$1" == "stop" ]
    then
        CMD=$1
        
        IS_VME=`hostname | grep vme | wc -l`
        if [ $IS_VME -eq 0 ] 
        then
            HOSTS=`grep vme-taps $TAPSSC/config/config.rootrc | cut -f2 -d: | tr -d ' '`
            for i in $HOSTS
            do
                printf "Performing AcquDAQ %s on %s\n" $CMD $i 
                ssh -Y root@$i $LOC/scripts/control_DAQ.sh $CMD
            done
        else
            # ROOT
            #export ROOTSYS=/opt/root
            #export PATH="$ROOTSYS/bin:$PATH"
            #export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"

            # TAPSsc
            #export TAPSSC=$LOC
            #export LD_LIBRARY_PATH="$TAPSSC/lib:$LD_LIBRARY_PATH"
            
            # kill old instance
            if [ "$CMD" == "stop" ]
            then
                killall -q AcquDAQ
            elif [ "$CMD" == "restart" ]
            then
                killall -q AcquDAQ
                cd $HOME/acqu
                DISPLAY=localhost:10.0 nohup /opt/acqu/build/bin/AcquDAQ &> /dev/null &
            fi
        fi
    else
        echo "Unknown argument"
    fi
else
    echo "Usage: control_DAQ.sh restart|stop"
fi

