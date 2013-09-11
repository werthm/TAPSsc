// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClient                                                             //
//                                                                      //
// TAPS client class.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClient.h"

ClassImp(TTClient)


//______________________________________________________________________________
TTClient::TTClient(const Char_t* host, Int_t port)
    : TTNetClient(host, port), TObject()
{
    // Constructor.
    
}

//______________________________________________________________________________
TTClient::~TTClient()
{
    // Destructor.

}

//______________________________________________________________________________
TServerType_t TTClient::GetType()
{
    // Return the type of the connected server.
    
    // check the socket
    if (!fSocket) return kNoServer;
    else
    {
        // check the connection
        if (!fSocket->IsValid()) return kNoServer;
        else
        {
            // get the server type
            fSocket->Send("TYPE");
            
            // wait for the response
            if (fSocket->Select(TSocket::kRead, 100) == 1)
            {
                // get response
                Char_t res[256];
                fSocket->Recv(res, 256);
                
                // check response
                TServerType_t t = (TServerType_t) atoi(res);
                if (t == kNoServer) return t;
                else if (t == kBaF2Server) return t;
                else if (t == kVetoServer) return t;
                else if (t == kPWOServer) return t;
                else if (t == kHVServer) return t;
                else return kNoServer;
            }
            else return kNoServer;
        }
    }
}

//______________________________________________________________________________
Bool_t TTClient::WriteHV(TTDataTypePar* d, Int_t elem)
{
    // Write the high voltage values of the parameter data type 'd' for the
    // element 'elem'.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kHVServer)
    {
        Error("WriteHV", "Cannot execute high voltage writing on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // check if the data type allows HV writing
    const Int_t nType = 3;
    const Char_t type[nType][32] = { "Par.BaF2.HV", "Par.Veto.HV", "Par.PWO.HV" };
    Bool_t tFound = kFALSE;
    for (Int_t i = 0; i < nType; i++)
    {
        if (!strcmp(d->GetName(), type[i]))
        {
            tFound = kTRUE;
            break;
        }
    }
    if (!tFound)
    {
        Error("WriteHV", "Data type '%s' does not allow HV writing!", d->GetName());
        return kFALSE;
    }

    // send HV write command
    sprintf(tmp, "WRITE_HV %s %d", d->GetName(), elem);
    fSocket->Send(tmp);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, 5000) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        if (!strcmp(tmp, "WRITE_HV_SUCCESS")) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

