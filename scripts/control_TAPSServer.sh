#!/bin/bash

#####################################################################
##                                                                 ##
## Script to restart all TAPSServer instances                      ##
##                                                                 ##
#####################################################################

if [ $# -eq 1 ]
then
    if [ "$1" == "restart" ] || [ "$1" == "stop" ]
    then
        CMD=$1
        
        IS_VME=`hostname | grep vme | wc -l`
        if [ $IS_VME -eq 0 ] 
        then
            for i in {0..10}
            do
                printf "Performing TAPSServer %s on taps-vme-%d\n" $CMD $i 
                ssh a2cb@taps-vme-$i /home/a2cb/TAPSsc/scripts/control_TAPSServer.sh $CMD
            done
        else
            # ROOT
            export ROOTSYS=/opt/root
            export PATH="$ROOTSYS/bin:$PATH"
            export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"

            # TAPSsc
            export TAPSSC="$HOME/TAPSsc"
            export LD_LIBRARY_PATH="$TAPSSC/lib:$LD_LIBRARY_PATH"
            
            # kill old instance
            if [ "$CMD" == "restart" ] || [ "$CMD" == "stop" ]
            then
                killall -q TAPSServer
            fi
            
            # set server type
            HOST=`hostname -s`
            if [ "$HOST" == "taps-vme-0" ] 
            then 
                ST="HV"
            elif [ "$HOST" == "taps-vme-1" ] 
            then 
                ST="BaF2"
            elif [ "$HOST" == "taps-vme-2" ] 
            then 
                ST="BaF2"
            elif [ "$HOST" == "taps-vme-3" ] 
            then 
                ST="BaF2"
            elif [ "$HOST" == "taps-vme-4" ] 
            then 
                ST="BaF2"
            elif [ "$HOST" == "taps-vme-5" ] 
            then 
                ST="BaF2"
            elif [ "$HOST" == "taps-vme-6" ] 
            then 
                ST="BaF2"
            elif [ "$HOST" == "taps-vme-7" ] 
            then 
                ST="Veto"
            elif [ "$HOST" == "taps-vme-8" ] 
            then 
                ST="Veto"
            elif [ "$HOST" == "taps-vme-9" ] 
            then 
                ST="Veto"
            elif [ "$HOST" == "taps-vme-10" ] 
            then 
                ST="PWO"
            fi

            # start new instance
            if [ "$CMD" == "restart" ]
            then
                nohup $TAPSSC/bin/TAPSServer -t $ST &> /dev/null &
            fi
        fi
    else
        echo "Unknown argument"
    fi
else
    echo "Usage: control_TAPSServer.sh restart|stop"
fi

