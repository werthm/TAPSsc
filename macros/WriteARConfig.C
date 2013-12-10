// SVN Info: $Id: WriteARConfig.C 1747 2013-10-31 23:27:28Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// WriteARConfig.C                                                      //
//                                                                      //
// Write the AcquDAQ configuration files to the VME controllers.        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void WriteARConfig()
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);

    // write BaF2 config files
    if (TTServerManager::GetManager()->WriteADConfigBaF2())
        Info("WriteARConfig", "BaF2 AcquDAQ config files were successfully written.");
    else
        Error("WriteARConfig", "Could not write all BaF2 AcquDAQ config files!");

    // write Veto config files
    if (TTServerManager::GetManager()->WriteADConfigVeto())
        Info("WriteARConfig", "Veto AcquDAQ config files were successfully written.");
    else
        Error("WriteARConfig", "Could not write all Veto AcquDAQ config files!");

    gSystem->Exit(0);
}

