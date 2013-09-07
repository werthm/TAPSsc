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
#include <fcntl.h>
#include <cstdlib>

#include "TSystem.h"
#include "TEnv.h"

#include "TTServerBaF2.h"
#include "TTServerVeto.h"
#include "TTServerPWO.h"
#include "TTServerHV.h"
#include "TTConfig.h"
#include "TTUtils.h"


// global variables
static TTServer* gTAPSServer = 0;
static Int_t gTAPSServerID = -1;
static Char_t gCfgFile[256] = "";


// enum for command line arguments
enum ETAPSServerArgs 
{
    kNoArgs,
    kPrintHelp,
    kBadServerID,
    kOk
};


//______________________________________________________________________________
void PrintHelp()
{
    // Print program usage.

    printf("\n");
    printf("TAPSServer\n");
    printf("Usage: TAPSServer -id N [-c config.rootrc] [-h]\n");
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
        
        // parse server configuration
        if (TTUtils::IndexOf(tmp, "-h") == 0) return kPrintHelp;
        else if (TTUtils::IndexOf(tmp, "-id") == 0) 
        {
            if (strcmp(tmp+3, "")) gTAPSServerID = atoi(tmp+3);
        }
        else if (TTUtils::IndexOf(tmp, "-c") == 0) 
        {
            strcpy(gCfgFile, tmp+3);
        }

        // set the new start to the current end value
        start = end;
    }

    // check server id
    if (gTAPSServerID == -1) return kBadServerID;

    return kOk;
}

//______________________________________________________________________________
Int_t main(Int_t argc, Char_t* argv[])
{
    // Main method.
    
    Char_t tmp[256];
    
    // decode command line arguments
    switch (DecodeArgs(argc, argv))
    {
        case kNoArgs:
            PrintHelp();
            return -1;
        case kPrintHelp:
            PrintHelp();
            return 0;
        case kBadServerID:
            printf("ERROR: server id has to be specified!\n");
            return -1;
        case kOk:
            break;
    }
    
    // try to load configuration file either via cmd-line argument or env. variable
    if (strcmp(gCfgFile, "")) strcpy(tmp, gCfgFile);
    else sprintf(tmp, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    if (gEnv->ReadFile(tmp, kEnvLocal))
    {
        printf("ERROR: Could not find configuration file!\n");
        printf("Check the command line arguments or the TAPSSC environment variable!\n");
        return -1;
    }

    // get server type
    TServerType_t serverType = kNoServer;
    sprintf(tmp, "Server-%d.Type", gTAPSServerID);
    const Char_t* sType = gEnv->GetValue(tmp, "null");
    if (!strcmp(sType, "null")) 
    {
        printf("ERROR: Could not find server type for server with ID %d!\n", gTAPSServerID);
        return -1;
    }
    else
    {
        if (!strcmp(sType, "BaF2")) serverType = kBaF2Server;
        else if (!strcmp(sType, "Veto")) serverType = kVetoServer;
        else if (!strcmp(sType, "PWO")) serverType = kPWOServer;
        else if (!strcmp(sType, "HV")) serverType = kHVServer;
        else
        {
            printf("ERROR: Unknown server type '%s'!\n", sType);
            return -1;
        }
    }

    //
    // locking (inspired by http://www.enderunix.org/docs/eng/daemon.php)
    //   

    // try to create lock file 
    Int_t lfp = open("/var/lock/TAPSServer.pid", O_RDWR | O_CREAT, 0644);

    // can not open lock-file
    if (lfp < 0) 
    {
        printf("ERROR: Cannot create lock-file! Check if an other instance of TAPSServer is already running!\n");
        printf("Delete /var/lock/TAPSServer.pid if you are ABSOLUTELY sure what you're doing!\n");
        return -1;
    }

    // can not lock lock-file
    if (lockf(lfp, F_TLOCK, 0) < 0) 
    {
        printf("ERROR: Cannot lock lock-file! Check if an other instance of TAPSServer is already running!\n");
        printf("Delete /var/lock/TAPSServer.pid if you are ABSOLUTELY sure what you're doing!\n");
        return -1;
    }

    // write pid to lockfile
    sprintf(tmp, "%d\n", getpid());
    write(lfp, tmp, strlen(tmp));

    // catch termination signals for proper server shutdown
    signal(SIGINT, server_shutdown);
    signal(SIGTERM, server_shutdown);

    // create network server and listen
    if (serverType == kBaF2Server) gTAPSServer = new TTServerBaF2(TTConfig::kTAPSServerPort);
    else if (serverType == kVetoServer) gTAPSServer = new TTServerVeto(TTConfig::kTAPSServerPort);
    else if (serverType == kPWOServer) gTAPSServer = new TTServerPWO(TTConfig::kTAPSServerPort);
    else if (serverType == kHVServer) gTAPSServer = new TTServerHV(TTConfig::kTAPSServerPort);
    gTAPSServer->Listen();
    
    // clean-up
    if (gTAPSServer) delete gTAPSServer;

    return 0;
}

