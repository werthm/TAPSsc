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
    fServerBaF2 = new TList();
    fServerVeto = new TList();
    fServerPWO = new TList();
    fServerHV = 0;
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
    if (fServerBaF2) delete fServerBaF2;
    if (fServerVeto) delete fServerVeto;
    if (fServerPWO) delete fServerPWO;
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
        
        // get type hostname
        sprintf(tmp, "Server-%d.Type", i);
        const Char_t* sType = gEnv->GetValue(tmp, "null");
        if (!strcmp(sType, "null")) 
        {
            Error("RegisterServers", "Configuration key '%s' was not found!", tmp);
            return kFALSE;
        }

        // register server and classify according to server type
        if (!strcmp(sType, "BaF2")) 
        {
            fServer[i] = new TTClientBaF2(sHost, TTConfig::kTAPSServerPort);
            fServerBaF2->Add(fServer[i]);
        }
        else if (!strcmp(sType, "Veto"))
        {
            fServer[i] = new TTClientVeto(sHost, TTConfig::kTAPSServerPort);
            fServerVeto->Add(fServer[i]);
        }
        else if (!strcmp(sType, "PWO")) 
        {
            fServer[i] = new TTClientPWO(sHost, TTConfig::kTAPSServerPort);
            fServerPWO->Add(fServer[i]);
        }
        else if (!strcmp(sType, "HV")) 
        {   
            fServer[i] = new TTClientHV(sHost, TTConfig::kTAPSServerPort);
            if (!fServerHV) fServerHV = (TTClientHV*) fServer[i];
            else 
            {
                Error("RegisterServers", "Only 1 HV server can be defined at the moment!");
                return kFALSE;
            }
        }
        else
        {
            Error("RegisterServers", "Unknown server type '%s'!", sType);
            return kFALSE;
        }
 
    }

    Info("RegisterServers", "Successfully registered %d servers", fNServer);

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

//______________________________________________________________________________
Bool_t TTServerManager::ReadHV(TTDataTypePar* d, Int_t elem, Int_t* outHV)
{
    // Read the high voltage value of the parameter data type 'd' for the
    // element 'elem' using the HV server client and save it to 'outHV'.
    // Return kTRUE on success, otherwise kFALSE.
    
    // check for HV server 
    if (!fServerHV)
    {
        Error("ReadHV", "Could not find connection to HV server!");
        return kFALSE;
    }

    // check of HV server connection
    if (fServerHV->GetStatus() != TTNetClient::kReady)
    {
        Error("ReadHV", "No connection to HV server '%s'!", 
              fServerHV->GetHost().GetHostName());
        return kFALSE;
    }

    // read HV and return return-value
    return fServerHV->ReadHV(d, elem, outHV);
}

//______________________________________________________________________________
Bool_t TTServerManager::WriteHV(TTDataTypePar* d, Int_t elem)
{
    // Write the high voltage value of the parameter data type 'd' for the
    // element 'elem' using the HV server client.
    // Return kTRUE on success, otherwise kFALSE.
    
    // check for HV server 
    if (!fServerHV)
    {
        Error("WriteHV", "Could not find connection to HV server!");
        return kFALSE;
    }

    // check of HV server connection
    if (fServerHV->GetStatus() != TTNetClient::kReady)
    {
        Error("WriteHV", "No connection to HV server '%s'!",
              fServerHV->GetHost().GetHostName());
        return kFALSE;
    }

    // write HV and return return-value
    return fServerHV->WriteHV(d, elem);
}

//______________________________________________________________________________
Bool_t TTServerManager::IsConnectedToHV()
{
    // Check if the connection to the HV server is ready.
    // If yes, return kTRUE, otherwise kFALSE.

    // check for HV server 
    if (!fServerHV) return kFALSE;

    // check of HV server connection
    if (fServerHV->GetStatus() == TTNetClient::kReady) return kTRUE;
    else return kFALSE;
}

//______________________________________________________________________________
Bool_t TTServerManager::GetStatusHV(Int_t mainframe, Bool_t* outSt)
{
    // Get the status of the HV mainframe 'mainframe' and save it to 'outSt'.
    // Return kTRUE on success, otherwise kFALSE.
    
    // check for HV server 
    if (!fServerHV)
    {
        Error("GetStatusHV", "Could not find connection to HV server!");
        return kFALSE;
    }

    // check of HV server connection
    if (fServerHV->GetStatus() != TTNetClient::kReady)
    {
        Error("GetStatusHV", "No connection to HV server '%s'!", 
              fServerHV->GetHost().GetHostName());
        return kFALSE;
    }

    // return return-value
    return fServerHV->GetStatusHV(mainframe, outSt);
}

//______________________________________________________________________________
Bool_t TTServerManager::SetStatusHV(Int_t mainframe, Bool_t status)
{
    // Set the status of the HV mainframe 'mainframe' to 'status'.
    // Return kTRUE on success, otherwise kFALSE.
    
    // check for HV server 
    if (!fServerHV)
    {
        Error("SetStatusHV", "Could not find connection to HV server!");
        return kFALSE;
    }

    // check of HV server connection
    if (fServerHV->GetStatus() != TTNetClient::kReady)
    {
        Error("SetStatusHV", "No connection to HV server '%s'!",
              fServerHV->GetHost().GetHostName());
        return kFALSE;
    }

    // return return-value
    return fServerHV->SetStatusHV(mainframe, status);
}

//______________________________________________________________________________
Bool_t TTServerManager::WriteADConfigBaF2()
{
    // Command all BaF2 servers to write the AcquDAQ configuration files.
    // Return kTRUE on success, otherwise kFALSE.
    
    // loop over BaF2 servers
    for (Int_t i = 0; i < fServerBaF2->GetSize(); i++)
    {
        // get server
        TTClientBaF2* s = (TTClientBaF2*) fServerBaF2->At(i);

        // check of server connection
        if (s->GetStatus() != TTNetClient::kReady)
        {
            Error("WriteADConfigBaF2", "No connection to BaF2 server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        // command config file writing
        if (!s->WriteADConfig())
        {
            Error("WriteADConfigBaF2", "Error during configuration file writing on server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        Info("WriteADConfigBaF2", "Wrote configuration files on server '%s'",
             s->GetHost().GetHostName());
    }

    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerManager::WriteADConfigVeto()
{
    // Command all Veto servers to write the AcquDAQ configuration files.
    // Return kTRUE on success, otherwise kFALSE.
    
    // loop over Veto servers
    for (Int_t i = 0; i < fServerVeto->GetSize(); i++)
    {
        // get server
        TTClientVeto* s = (TTClientVeto*) fServerVeto->At(i);

        // check of server connection
        if (s->GetStatus() != TTNetClient::kReady)
        {
            Error("WriteADConfigVeto", "No connection to Veto server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        // command config file writing
        if (!s->WriteADConfig())
        {
            Error("WriteADConfigVeto", "Error during configuration file writing on server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        Info("WriteADConfigVeto", "Wrote configuration files on server '%s'",
             s->GetHost().GetHostName());
    }

    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerManager::StartCalibQAC()
{
    // Command all TAPS servers to start a QAC pedestal calibration.
    // Return kTRUE on success, otherwise kFALSE.
    
    // loop over BaF2 servers
    for (Int_t i = 0; i < fServerBaF2->GetSize(); i++)
    {
        // get server
        TTClientBaF2* s = (TTClientBaF2*) fServerBaF2->At(i);

        // check of server connection
        if (s->GetStatus() != TTNetClient::kReady)
        {
            Error("StartCalibQAC", "No connection to BaF2 server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        // command start of QAC calibration
        if (!s->StartCalibQAC())
        {
            Error("StartCalibQAC", "Error during starting of QAC calibration on server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        Info("StartCalibQAC", "Started QAC calibration on server '%s'",
             s->GetHost().GetHostName());
    }

    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerManager::StopCalibQAC()
{
    // Command all TAPS servers to stop the QAC pedestal calibration.
    // Return kTRUE on success, otherwise kFALSE.
    
    // loop over BaF2 servers
    for (Int_t i = 0; i < fServerBaF2->GetSize(); i++)
    {
        // get server
        TTClientBaF2* s = (TTClientBaF2*) fServerBaF2->At(i);

        // check of server connection
        if (s->GetStatus() != TTNetClient::kReady)
        {
            Error("StopCalibQAC", "No connection to BaF2 server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        // command stop of QAC calibration
        if (!s->StopCalibQAC())
        {
            Error("StopCalibQAC", "Error during stopping of QAC calibration on server '%s'!",
                  s->GetHost().GetHostName());
            return kFALSE;
        }

        Info("StopCalibQAC", "Stopped QAC calibration on server '%s'",
             s->GetHost().GetHostName());
    }

    return kTRUE;
}

