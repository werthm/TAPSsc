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


//______________________________________________________________________________
TTServerVeto::TTServerVeto(Int_t port)
    : TTServer(kVetoServer, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
Bool_t TTServerVeto::ProcessCommand(const Char_t* cmd, TSocket* s)
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

