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
Bool_t TTServerHV::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // WRITE_HV command: write HV to hardware
    if (TTUtils::IndexOf(cmd, "WRITE_HV") == 0)
    {
        // check if LeCroy is connected
        if (!fLeCroy)
        {
            Error("ProcessCommand", "No connection to HV mainframe!");
            s->Send("WRITE_HV_FAILED");
            return kTRUE;
        }

        // extract the parameter data type and the element number
        Char_t type[32];
        Int_t elem;
        sscanf(cmd, "WRITE_HV %s %d", type, &elem);
        
        printf("type: '%s'   elem: %d\n", type, elem);

        // try to get data type
        TTDataTypePar* d = (TTDataTypePar*) TTMySQLManager::GetManager()->GetDataTypes()->FindObject(type);
        if (!d)
        {
            Error("ProcessCommand", "Could not find data type '%s'!", type);
            s->Send("WRITE_HV_FAILED");
            return kTRUE;
        }

        // try to read values
        Double_t par;
        if (!TTMySQLManager::GetManager()->ReadParameters(type, 1, &elem, &par))
        {
            Error("ProcessCommand", "Could not read parameter data type '%s' for element %d!", type, elem);
            s->Send("WRITE_HV_FAILED");
            return kTRUE;
        }

        printf("DB value: %.f\n", par);
  
        // find mainframe and channel number
        Int_t crate, channel;
        if (!TTMySQLManager::GetManager()->ReadMaps(d->GetMap()->GetName(), 1, &elem, &crate, 0, &channel))
        {
            Error("ProcessCommand", "Could not read map data type '%s' for element %d!", d->GetMap()->GetName(), elem);
            s->Send("WRITE_HV_FAILED");
            return kTRUE;
        }

        printf("crate: %d   channel: %d\n", crate, channel);
 
        //Bool_t ReadHV(Int_t mf, Int_t c, Int_t* outDem, Int_t* outBack = 0, Int_t* outAc = 0);
        //Bool_t WriteHV(Int_t mf, Int_t c, Int_t val);

        s->Send("WRITE_HV_SUCCESS");
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

