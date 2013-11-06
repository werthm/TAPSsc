// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerVeto                                                         //
//                                                                      //
// Veto TAPS server class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServerVeto.h"

ClassImp(TTServerVeto)


// init static class members
const Long_t TTServerVeto::fgVMEBase = 0x50000000;


//______________________________________________________________________________
TTServerVeto::TTServerVeto(Int_t port, Int_t id)
    : TTServer(kVetoServer, port, id)
{
    // Constructor.
        
    // init members
    fWriter = new TTWriteADConfigVeto(GetID());
    fCrate = new TTVMECrateVeto(GetID(), fgVMEBase);
}

//______________________________________________________________________________
TTServerVeto::~TTServerVeto()
{
    // Destructor.

    if (fWriter) delete fWriter;
    if (fCrate) delete fCrate;
}

//______________________________________________________________________________
Bool_t TTServerVeto::WriteADConfig(const Char_t* cmd, TSocket* s)
{
    // Handler method for the write AcquDAQ config command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().
    
    // mount /opt read-write
    // workaround for busy /opt when unmounting again -> pre-load MySQL libraries 
    TTMySQLManager::GetManager();
    system("mount /opt");

    // try to write all configuration files
    Bool_t ret = kTRUE;
    for (Int_t i = 0; i < 16; i++)
    {
        if (!fWriter->Write(i)) ret = kFALSE;
    }

    // unmount /opt
    system("sync");
    system("umount /opt");
    
    // check result
    if (ret)
    {
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteARCfgVetoSuccess);
        return kTRUE;
    }
    else
    {
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteARCfgVetoFailed);
        return kTRUE;
    }
}

//______________________________________________________________________________
Bool_t TTServerVeto::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // get the network command
    Int_t nc = TTUtils::GetNetworkCmd(cmd);
    
    // write AcquDAQ config command: write AcquDAQ config files
    if (nc == TTConfig::kNCWriteARCfgVeto) return WriteADConfig(cmd, s);
    else if (nc == TTConfig::kNCStartCalibQAC)
    {
        // start QAC calibration
        if (fCrate->StartCalibQAC())
            TTUtils::SendNetworkCmd(s, TTConfig::kNCStartCalibQACSuccess);
        else
            TTUtils::SendNetworkCmd(s, TTConfig::kNCStartCalibQACFailed);
        
        return kTRUE;
    }
    else if (nc == TTConfig::kNCStopCalibQAC)
    {
        // check if QAC calibration is running
        if (!fCrate->IsCalQACRunning())
        {
            // send failed because calibration cannot be stopped
            TTUtils::SendNetworkCmd(s, TTConfig::kNCStopCalibQACFailed);
            return kTRUE;
        }

        // stop QAC calibration
        fCrate->StopCalibQAC();
    
        // send Ok
        TTUtils::SendNetworkCmd(s, TTConfig::kNCStopCalibQACSuccess);
        
        // send QAC calibration
        TMessage mes(kMESS_OBJECT | kMESS_ACK);
        mes.WriteObject(fCrate->GetCalibQAC());
        s->Send(mes);
        
        return kTRUE;
    }
    else
    {
        // call parent method
        if (TTServer::ProcessCommand(cmd, s)) return kTRUE;
        else
        {
            Warning("ProcessCommand", "Unknown command '%s'", cmd);
            return kFALSE;
        }
    }
}

