// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MainTAPSServer                                                       //
//                                                                      //
// Contains the main method for the TAPSServer application.             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include <signal.h>

#include "TTServer.h"
#include "TTConfig.h"


static TTServer* gTAPSServer = 0;


//______________________________________________________________________________
static void server_shutdown(int signal)
{
    // Proper shutdown of the TAPSServer.

    if (gTAPSServer)
    {
        gTAPSServer->StopListening();   
        delete gTAPSServer;
        gTAPSServer = 0;
    }
}

//______________________________________________________________________________
Int_t main(Int_t argc, Char_t* argv[])
{
    // Main method.
    
    // catch termination signals for proper server shutdown
    signal(SIGINT, server_shutdown);
    signal(SIGTERM, server_shutdown);

    // create network server and start listening
    gTAPSServer = new TTServer(TTConfig::kTAPSServerPort);
    gTAPSServer->StartListening();
    
    // clean-up
    if (gTAPSServer) delete gTAPSServer;

    return 0;
}

