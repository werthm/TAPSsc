// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerHV                                                           //
//                                                                      //
// HV TAPS server class  .                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServerHV.h"

ClassImp(TTServerHV)


//______________________________________________________________________________
TTServerHV::TTServerHV(Int_t port)
    : TTServer(kHVServer, port)
{
    // Constructor.
    
    // init members
    fLeCroy = 0;

    // get serial device
    const Char_t* device = gEnv->GetValue("HV.RS232.Device", "null");
    if (!strcmp(device, "null")) 
    {
        Error("TTServerHV", "Could not read serial device from the configuration file!");
        return;
    }

    // start communication with HV 
    fLeCroy = new TTLeCroy1445(device);
    if (!fLeCroy->Init())
    {
        Error("TTServerHV", "No connection to LeCroy 1445!");
        delete fLeCroy;
        fLeCroy = 0;
        return;
    }
}

//______________________________________________________________________________
TTServerHV::~TTServerHV()
{
    // Destructor.
    
    if (fLeCroy) delete fLeCroy;
}

//______________________________________________________________________________
Bool_t TTServerHV::GetStatusHV(const Char_t* cmd, TSocket* s)
{
    // Handler method for the get HV status command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().

    // check if LeCroy is connected
    if (!fLeCroy)
    {
        Error("GetStatusHV", "No connection to HV mainframe!");
        TTUtils::SendNetworkCmd(s, TTConfig::kNCStatusHVFailed);
        return kTRUE;
    }
    
    // extract the mainframe number
    Int_t mf;
    sscanf(cmd, "%*d %d", &mf);
 
    // try to read high voltage status from hardware
    Bool_t st;
    if (!fLeCroy->GetStatusHV(mf, &st))
    {
        Error("GetStatusHV", "Could not get the HV status of mainframe %d!", mf);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCStatusHVFailed);
        return kTRUE;
    }

    // all should be ok here
    Char_t answ[32];
    sprintf(answ, "%d %d", TTConfig::kNCStatusHVRet, st);
    s->Send(answ);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerHV::SetStatusHV(const Char_t* cmd, TSocket* s)
{
    // Handler method for the set HV status command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().

    // check if LeCroy is connected
    if (!fLeCroy)
    {
        Error("SetStatusHV", "No connection to HV mainframe!");
        TTUtils::SendNetworkCmd(s, TTConfig::kNCSetStatusHVFailed);
        return kTRUE;
    }
    
    // extract the mainframe number and the status
    Int_t mf, st;
    sscanf(cmd, "%*d %d %d", &mf, &st);
 
    // try to set high voltage status in hardware
    if (!fLeCroy->SetStatusHV(mf, st))
    {
        Error("SetStatusHV", "Could not set the HV status of mainframe %d!", mf);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCSetStatusHVFailed);
        return kTRUE;
    }

    // all should be ok here
    TTUtils::SendNetworkCmd(s, TTConfig::kNCSetStatusHVSuccess);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerHV::ReadHV(const Char_t* cmd, TSocket* s)
{
    // Handler method for the read HV command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().

    // check if LeCroy is connected
    if (!fLeCroy)
    {
        Error("ReadHV", "No connection to HV mainframe!");
        TTUtils::SendNetworkCmd(s, TTConfig::kNCReadHVFailed);
        return kTRUE;
    }

    // extract the parameter data type and the element number
    Char_t type[32];
    Int_t elem;
    sscanf(cmd, "%*d %s %d", type, &elem);
    
    // try to get data type
    TTDataTypePar* d = (TTDataTypePar*) TTMySQLManager::GetManager()->GetDataTypes()->FindObject(type);
    if (!d)
    {
        Error("ReadHV", "Could not find data type '%s'!", type);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCReadHVFailed);
        return kTRUE;
    }

    // find mainframe and channel number
    Int_t crate, channel;
    if (!TTMySQLManager::GetManager()->ReadMaps(d->GetMap()->GetName(), 1, &elem, &crate, 0, &channel))
    {
        Error("ReadHV", "Could not read map data type '%s' for element %d!", d->GetMap()->GetName(), elem);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCReadHVFailed);
        return kTRUE;
    }

    // try to read high voltage to hardware
    Int_t val;
    if (!fLeCroy->ReadHV(crate, channel, &val))
    {
        Error("ReadHV", "Could not read HV '%s' of element %d from hardware!", d->GetMap()->GetName(), elem);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCReadHVFailed);
        return kTRUE;
    }

    // all should be ok here
    Char_t answ[32];
    sprintf(answ, "%d %d", TTConfig::kNCReadHVSuccess, val);
    s->Send(answ);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerHV::WriteHV(const Char_t* cmd, TSocket* s)
{
    // Handler method for the write HV command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().

    // check if LeCroy is connected
    if (!fLeCroy)
    {
        Error("WriteHV", "No connection to HV mainframe!");
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteHVFailed);
        return kTRUE;
    }

    // extract the parameter data type and the element number
    Char_t type[32];
    Int_t elem;
    sscanf(cmd, "%*d %s %d", type, &elem);
    
    // try to get data type
    TTDataTypePar* d = (TTDataTypePar*) TTMySQLManager::GetManager()->GetDataTypes()->FindObject(type);
    if (!d)
    {
        Error("WriteHV", "Could not find data type '%s'!", type);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteHVFailed);
        return kTRUE;
    }

    // try to read values
    Double_t par;
    if (!TTMySQLManager::GetManager()->ReadParameters(type, 1, &elem, &par))
    {
        Error("WriteHV", "Could not read parameter data type '%s' for element %d!", type, elem);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteHVFailed);
        return kTRUE;
    }
    
    // find mainframe and channel number
    Int_t crate, channel;
    if (!TTMySQLManager::GetManager()->ReadMaps(d->GetMap()->GetName(), 1, &elem, &crate, 0, &channel))
    {
        Error("WriteHV", "Could not read map data type '%s' for element %d!", d->GetMap()->GetName(), elem);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteHVFailed);
        return kTRUE;
    }

    // try to write high voltage to hardware
    if (!fLeCroy->WriteHV(crate, channel, (Int_t)par))
    {
        Error("WriteHV", "Could not write HV '%s' of element %d to hardware!", d->GetMap()->GetName(), elem);
        TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteHVFailed);
        return kTRUE;
    }

    // all should be ok here
    TTUtils::SendNetworkCmd(s, TTConfig::kNCWriteHVSuccess);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerHV::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // get the network command
    Int_t nc = TTUtils::GetNetworkCmd(cmd);
    
    // write HV command: write HV to hardware
    if (nc == TTConfig::kNCStatusHV) return GetStatusHV(cmd, s);
    else if (nc == TTConfig::kNCSetStatusHV) return SetStatusHV(cmd, s);
    else if (nc == TTConfig::kNCReadHV) return ReadHV(cmd, s);
    else if (nc == TTConfig::kNCWriteHV) return WriteHV(cmd, s);
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

