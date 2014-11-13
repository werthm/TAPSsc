/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientHV                                                           //
//                                                                      //
// HV TAPS client class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClientHV.h"
#include "TTDataTypePar.h"
#include "TTUtils.h"
#include "TTConfig.h"

ClassImp(TTClientHV)

//______________________________________________________________________________
TTClientHV::TTClientHV(const Char_t* host, Int_t port)
    : TTClient(host, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
Bool_t TTClientHV::ReadHV(TTDataTypePar* d, Int_t elem, Int_t* outHV)
{
    // Read the high voltage value of the parameter data type 'd' for the
    // element 'elem' and save it to 'outHV'.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kHVServer)
    {
        Error("ReadHV", "Cannot execute high voltage reading on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // check if the data type allows HV reading
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
        Error("ReadHV", "Data type '%s' does not allow HV reading!", d->GetName());
        return kFALSE;
    }

    // send HV read command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCReadHV, d->GetName(), elem);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, TTConfig::kHVNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCReadHVSuccess) 
        {
            // extract and set the read value
            Int_t val;
            sscanf(tmp, "%*d %d", &val);
            *outHV = val;

            return kTRUE;
        }
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

//______________________________________________________________________________
Bool_t TTClientHV::WriteHV(TTDataTypePar* d, Int_t elem)
{
    // Write the high voltage value of the parameter data type 'd' for the
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
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCWriteHV, d->GetName(), elem);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, TTConfig::kHVNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCWriteHVSuccess) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

//______________________________________________________________________________
Bool_t TTClientHV::GetStatusHV(Int_t mainframe, Bool_t* outSt)
{
    // Get the status of the HV mainframe 'mainframe' and save it to 'outSt'.
    // Return kTRUE on success, otherwise kFALSE.
  
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kHVServer)
    {
        Error("GetStatusHV", "Cannot check high voltage status on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send HV get status command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCStatusHV, mainframe);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, TTConfig::kHVNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCStatusHVRet)
        {
            // get and write status
            Int_t st;
            sscanf(tmp, "%*d %d", &st);
            *outSt = st;

            return kTRUE;
        }
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

//______________________________________________________________________________
Bool_t TTClientHV::SetStatusHV(Int_t mainframe, Bool_t status)
{
    // Set the status of the HV mainframe 'mainframe' to 'status'.
    // Return kTRUE on success, otherwise kFALSE.
  
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kHVServer)
    {
        Error("SetStatusHV", "Cannot check high voltage status on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send HV get status command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCSetStatusHV, mainframe, status);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, 2*TTConfig::kHVNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCSetStatusHVSuccess) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

