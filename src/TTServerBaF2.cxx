// SVN Info: $Id: TTServerBaF2.cxx 1751 2013-11-05 02:02:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerBaF2                                                         //
//                                                                      //
// BaF2 TAPS server class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServerBaF2.h"

ClassImp(TTServerBaF2)


// init static class members
const Long_t TTServerBaF2::fgVMEBase = 0x30000000;


//______________________________________________________________________________
TTServerBaF2::TTServerBaF2(Int_t port, Int_t id)
    : TTServer(kBaF2Server, port, id)
{
    // Constructor.
        
    // init members
    fWriter = new TTWriteADConfigBaF2(GetID());
    fCrate = new TTVMECrateBaF2(GetID(), fgVMEBase);
}

//______________________________________________________________________________
TTServerBaF2::~TTServerBaF2()
{
    // Destructor.

    if (fWriter) delete fWriter;
    if (fCrate) delete fCrate;
}

//______________________________________________________________________________
Bool_t TTServerBaF2::WriteADConfig(const Char_t* cmd, TSocket* s)
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
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteARCfgBaF2Success);
        return kTRUE;
    }
    else
    {
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteARCfgBaF2Failed);
        return kTRUE;
    }
}

//______________________________________________________________________________
Bool_t TTServerBaF2::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // get the network command
    Int_t nc = TTUtils::GetNetworkCmd(cmd);
    
    // write AcquDAQ config command: write AcquDAQ config files
    if (nc == TTConfig::kNCWriteARCfgBaF2) return WriteADConfig(cmd, s);
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

