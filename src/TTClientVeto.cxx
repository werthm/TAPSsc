// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientVeto                                                         //
//                                                                      //
// Veto TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClientVeto.h"

ClassImp(TTClientVeto)


//______________________________________________________________________________
TTClientVeto::TTClientVeto(const Char_t* host, Int_t port)
    : TTClient(host, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
Bool_t TTClientVeto::WriteADConfig()
{
    // Write the AcquDAQ configuration files.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kVetoServer)
    {
        Error("WriteADConfig", "Cannot write Veto AcquDAQ configuration files on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send HV get status command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCWriteARCfgVeto);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, 60000) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCWriteARCfgVetoSuccess) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

