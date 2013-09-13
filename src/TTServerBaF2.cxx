// SVN Info: $Id$

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


//______________________________________________________________________________
TTServerBaF2::TTServerBaF2(Int_t port)
    : TTServer(kBaF2Server, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
Bool_t TTServerBaF2::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // get the network command
    Int_t nc = TTUtils::GetNetworkCmd(cmd);
    
    // write AcquDAQ command: write AcquDAQ config files
    if (nc == TTConfig::kNCWriteAR)
    {
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

