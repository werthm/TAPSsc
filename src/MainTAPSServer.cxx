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
#include "TTUtils.h"


// global variables
static TTServer* gTAPSServer = 0;
static TServerType_t gTAPSServerType = kNoServer;


// enum for command line arguments
enum ETAPSServerArgs 
{
    kNoArgs,
    kPrintHelp,
    kBadServerType,
    kOk
};


//______________________________________________________________________________
void PrintHelp()
{
    // Print program usage.

    printf("\n");
    printf("TAPSServer\n");
    printf("Usage: TAPSServer -t [BaF2,Veto,PWO,HV]\n");
    printf("\n");
}

//______________________________________________________________________________
static void server_shutdown(int signal)
{
    // Proper shutdown of the TAPSServer.

    // check if TAPSServer was created
    if (gTAPSServer)
    {
        // stop listening
        gTAPSServer->StopListening();   

        // wait for listen loop to terminate
        usleep(1000*200);

        // clean-up
        delete gTAPSServer;
        gTAPSServer = 0;
    }
}

//______________________________________________________________________________
Int_t DecodeArgs(Int_t argc, Char_t* argv[])
{
    // Decode command line arguments.
    
    Char_t params[1024];
    Char_t tmp[256];
     
    // no arguments
    if (argc == 1) return kNoArgs;

    // concatenate all arguments into one string
    params[0] = '\0';
    for (Int_t i = 1; i < argc; i++)
    {
        strcat(params, argv[i]);
        strcat(params, " ");
    }
    
    // loop over all "-" argument tokens
    Int_t start = 0;
    Int_t end = 0;
    while ((start = TTUtils::IndexOf(params, "-", start)) >= 0)
    {
        // end is either the beginning of the next "-" token or the
        // end of the total argument string
        if ((end = TTUtils::IndexOf(params, "-", start+1)) == -1)
        end = strlen(params);
    
        // copy the token into a separate string, terminate it
        strncpy(tmp, params+start, end-start-1);
        tmp[end-start-1] = '\0';
        
        // parse server type
        if (TTUtils::IndexOf(tmp, "-h") == 0) return kPrintHelp;
        else if (TTUtils::IndexOf(tmp, "-t") == 0) 
        {
            if (!strcmp(tmp+3, "BaF2")) gTAPSServerType = kBaF2Server;
            else if (!strcmp(tmp+3, "Veto")) gTAPSServerType = kVetoServer;
            else if (!strcmp(tmp+3, "PWO")) gTAPSServerType = kPWOServer;
            else if (!strcmp(tmp+3, "HV")) gTAPSServerType = kHVServer;
            else return kBadServerType;
        }

        // set the new start to the current end value
        start = end;
    }

    // check server configuration
    if (gTAPSServerType == kNoServer) return kBadServerType;

    return kOk;
}

//______________________________________________________________________________
Int_t main(Int_t argc, Char_t* argv[])
{
    // Main method.
    
    // decode command line arguments
    switch (DecodeArgs(argc, argv))
    {
        case kNoArgs:
            PrintHelp();
            return -1;
        case kPrintHelp:
            PrintHelp();
            return 0;
        case kBadServerType:
            printf("ERROR: server type has to be BaF2/Veto/PWO/HV!\n");
            return -1;
        case kOk:
            break;
    }
    
    // catch termination signals for proper server shutdown
    signal(SIGINT, server_shutdown);
    signal(SIGTERM, server_shutdown);

    // create network server and listen
    gTAPSServer = new TTServer(gTAPSServerType, TTConfig::kTAPSServerPort);
    gTAPSServer->Listen();
    
    // clean-up
    if (gTAPSServer) delete gTAPSServer;

    return 0;
}

