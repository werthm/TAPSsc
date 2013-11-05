// SVN Info: $Id$

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
void CalibQAC(Int_t status)
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);
    
    // start/stop calibration
    Bool_t ret;
    if (status) ret =TTServerManager::GetManager()->StartCalibQAC();
    else ret = TTServerManager::GetManager()->StopCalibQAC();
    
    if (ret) printf("Operation successful!\n");
    else printf("Operation failed!\n");
 
    gSystem->Exit(0);
}

