// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerManager                                                      //
//                                                                      //
// This class handles all the communication with the TAPS servers.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServerManager.h"

ClassImp(TTServerManager)


// init static class members
TTServerManager* TTServerManager::fgServerManager = 0;


//______________________________________________________________________________
TTServerManager::TTServerManager()
{
    // Constructor.
    
    // init members
    fNServer = 0;
    fServer = 0;
    fSilence = kFALSE;

    // register servers
    if (!RegisterServers())
    {
        if (!fSilence) Error("TTServerManager", "An error occurred during the server registration!");
        return;
    }
}

//______________________________________________________________________________
TTServerManager::~TTServerManager()
{
    // Destructor.

    if (fServer)
    {
        for (Int_t i = 0; i < fNServer; i++) delete fServer[i];
        delete [] fServer;
    }
}

//______________________________________________________________________________
Bool_t TTServerManager::RegisterServers()
{
    // Register all configured servers.
    // Return kFALSE if an error occurred, otherwise kTRUE.
    
    Char_t tmp[256];

    // get number of servers
    fNServer = gEnv->GetValue("Server.Number", 0);
    if (!fNServer)
    {
        if (!fSilence) Error("RegisterServers", "Number of registered servers is zero or unknown!");
        return kFALSE;
    }
    
    // create server array
    fServer = new TTClient*[fNServer];

    // loop over servers
    for (Int_t i = 0; i < fNServer; i++)
    {
        // get server hostname
        sprintf(tmp, "Server-%d.Host", i);
        const Char_t* sHost = gEnv->GetValue(tmp, "null");
        if (!strcmp(sHost, "null")) 
        {
            Error("RegisterServers", "Configuration key '%s' was not found!", tmp);
            return kFALSE;
        }

        // register new server
        fServer[i] = new TTClient(sHost, TTConfig::kTAPSServerPort);
    }

    return kTRUE;
}

//______________________________________________________________________________
void TTServerManager::PrintStatus()
{
    // Print the status of the server manager.
    
    Char_t tmp[256];

    printf("\n");
    printf("### TTServerManager Status ###\n");

    // check if servers were registered
    if (fServer)
    {
        printf("=> %d servers registered\n", fNServer);

        // loop over registered servers
        for (Int_t i = 0; i < fNServer; i++)
        {
            // show hostname
            printf("=> Server %2d on %-40s", i, fServer[i]->GetHost().GetHostName());
            
            // show status
            switch (fServer[i]->GetStatus())
            {
                case TTNetClient::kNoConn:
                    strcpy(tmp, "NOT CONNECTED");
                    break;
                case TTNetClient::kNoResp:
                    strcpy(tmp, "NO RESPONSE");
                    break;
                case TTNetClient::kBadResp:
                    strcpy(tmp, "BAD RESPONSE");
                    break;
                case TTNetClient::kReady:
                    strcpy(tmp, "READY");
                    break;
                default:
                    strcpy(tmp, "UNKNOWN");
                    break;
            }
            printf("  Status: %-14s", tmp);
            
            // show type
            switch (fServer[i]->GetType())
            {
                case kNoServer:
                    strcpy(tmp, "UNKNOWN");
                    break;
                case kBaF2Server:
                    strcpy(tmp, "BaF2");
                    break;
                case kVetoServer:
                    strcpy(tmp, "Veto");
                    break;
                case kPWOServer:
                    strcpy(tmp, "PWO");
                    break;
                case kHVServer:
                    strcpy(tmp, "HV");
                    break;
                default:
                    strcpy(tmp, "UNKNOWN");
                    break;
            }
            printf("  Type: %-7s", tmp);

            printf("\n");
        }
    }
    else
    {
        printf("=> No servers configured\n");
    }
    
    printf("\n");
}

