// SVN Info: $Id: CalibQAC.C 1752 2013-11-06 16:49:35Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CalibQAC.C                                                           //
//                                                                      //
// Perform the QAC pedestal calibration.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void CalibQAC(Bool_t isVeto, Int_t status)
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);
    
    // start/stop calibration
    Bool_t ret;
    if (status) 
    {
        if (isVeto) ret =TTServerManager::GetManager()->StartCalibQACVeto();
        else ret =TTServerManager::GetManager()->StartCalibQACBaF2();
    }
    else 
    {
        if (isVeto) ret = TTServerManager::GetManager()->StopCalibQACVeto();
        else ret = TTServerManager::GetManager()->StopCalibQACBaF2();
    }

    if (ret) printf("Operation successful!\n");
    else printf("Operation failed!\n");
 
    gSystem->Exit(0);
}

