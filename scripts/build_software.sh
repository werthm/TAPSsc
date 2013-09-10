#!/bin/bash

#####################################################################
##                                                                 ##
## Script to build the TAPSsc software on all VME controllers      ##
##                                                                 ##
#####################################################################

IS_VME=`hostname | grep vme | wc -l`

if [ $IS_VME -eq 0 ] 
then
    for i in {0..10}
    do
        printf "\nBuilding software on taps-vme-%d\n\n" $i 
        ssh root@taps-vme-$i /root/TAPSsc/scripts/build_software.sh
    done
else
    # ROOT
    export ROOTSYS=/opt/root
    export PATH="$ROOTSYS/bin:$PATH"
    export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"

    # TAPSsc
    export TAPSSC="$HOME/TAPSsc"
    export LD_LIBRARY_PATH="$TAPSSC/lib:$LD_LIBRARY_PATH"

    cd $HOME/TAPSsc
    make clean
    make
fi

