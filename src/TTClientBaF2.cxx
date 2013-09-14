// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientBaF2                                                         //
//                                                                      //
// BaF2 TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClientBaF2.h"

ClassImp(TTClientBaF2)


//______________________________________________________________________________
TTClientBaF2::TTClientBaF2(const Char_t* host, Int_t port)
    : TTClient(host, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
Bool_t TTClientBaF2::WriteADConfig()
{
    // Write the AcquDAQ configuration files.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kBaF2Server)
    {
        Error("WriteADConfig", "Cannot write BaF2 AcquDAQ configuration files on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send HV get status command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCWriteARCfgBaF2);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, 60000) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCWriteARCfgBaF2Success) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

