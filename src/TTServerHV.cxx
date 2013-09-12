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
Bool_t TTServerHV::ReadHV(const Char_t* cmd, TSocket* s)
{
    // Handler method for the READ_HV command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().

    // check if LeCroy is connected
    if (!fLeCroy)
    {
        Error("ReadHV", "No connection to HV mainframe!");
        s->Send("READ_HV_FAILED");
        return kTRUE;
    }

    // extract the parameter data type and the element number
    Char_t type[32];
    Int_t elem;
    sscanf(cmd, "READ_HV %s %d", type, &elem);
    
    // try to get data type
    TTDataTypePar* d = (TTDataTypePar*) TTMySQLManager::GetManager()->GetDataTypes()->FindObject(type);
    if (!d)
    {
        Error("ReadHV", "Could not find data type '%s'!", type);
        s->Send("READ_HV_FAILED");
        return kTRUE;
    }

    // find mainframe and channel number
    Int_t crate, channel;
    if (!TTMySQLManager::GetManager()->ReadMaps(d->GetMap()->GetName(), 1, &elem, &crate, 0, &channel))
    {
        Error("ReadHV", "Could not read map data type '%s' for element %d!", d->GetMap()->GetName(), elem);
        s->Send("READ_HV_FAILED");
        return kTRUE;
    }

    // try to read high voltage to hardware
    Int_t val;
    if (!fLeCroy->ReadHV(crate, channel, &val))
    {
        Error("ReadHV", "Could not read HV '%s' of element %d from hardware!", d->GetMap()->GetName(), elem);
        s->Send("READ_HV_FAILED");
        return kTRUE;
    }

    // all should be ok here
    Char_t answ[32];
    sprintf(answ, "READ_HV_SUCCESS %d", val);
    s->Send(answ);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerHV::WriteHV(const Char_t* cmd, TSocket* s)
{
    // Handler method for the WRITE_HV command found in 'cmd' sent via 's'.
    // Return value as in ProcessCommand().

    // check if LeCroy is connected
    if (!fLeCroy)
    {
        Error("WriteHV", "No connection to HV mainframe!");
        s->Send("WRITE_HV_FAILED");
        return kTRUE;
    }

    // extract the parameter data type and the element number
    Char_t type[32];
    Int_t elem;
    sscanf(cmd, "WRITE_HV %s %d", type, &elem);
    
    // try to get data type
    TTDataTypePar* d = (TTDataTypePar*) TTMySQLManager::GetManager()->GetDataTypes()->FindObject(type);
    if (!d)
    {
        Error("WriteHV", "Could not find data type '%s'!", type);
        s->Send("WRITE_HV_FAILED");
        return kTRUE;
    }

    // try to read values
    Double_t par;
    if (!TTMySQLManager::GetManager()->ReadParameters(type, 1, &elem, &par))
    {
        Error("WriteHV", "Could not read parameter data type '%s' for element %d!", type, elem);
        s->Send("WRITE_HV_FAILED");
        return kTRUE;
    }
    
    // find mainframe and channel number
    Int_t crate, channel;
    if (!TTMySQLManager::GetManager()->ReadMaps(d->GetMap()->GetName(), 1, &elem, &crate, 0, &channel))
    {
        Error("WriteHV", "Could not read map data type '%s' for element %d!", d->GetMap()->GetName(), elem);
        s->Send("WRITE_HV_FAILED");
        return kTRUE;
    }

    // try to write high voltage to hardware
    if (!fLeCroy->WriteHV(crate, channel, (Int_t)par))
    {
        Error("WriteHV", "Could not write HV '%s' of element %d to hardware!", d->GetMap()->GetName(), elem);
        s->Send("WRITE_HV_FAILED");
        return kTRUE;
    }

    // all should be ok here
    s->Send("WRITE_HV_SUCCESS");
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTServerHV::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // WRITE_HV command: write HV to hardware
    if (TTUtils::IndexOf(cmd, "READ_HV") == 0) return ReadHV(cmd, s);
    else if (TTUtils::IndexOf(cmd, "WRITE_HV") == 0) return WriteHV(cmd, s);
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

