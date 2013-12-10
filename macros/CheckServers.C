// SVN Info: $Id: CheckServers.C 1661 2013-08-18 21:32:15Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CheckServers.C                                                       //
//                                                                      //
// Check the status of the TAPS servers.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void CheckServers()
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);

    // check status
    TTServerManager::GetManager()->PrintStatus();

    gSystem->Exit(0);
}

