// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerPWO                                                          //
//                                                                      //
// PWO TAPS server class.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServerPWO.h"

ClassImp(TTServerPWO)


//______________________________________________________________________________
TTServerPWO::TTServerPWO(Int_t port)
    : TTServer(kPWOServer, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
Bool_t TTServerPWO::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // WRITE_ARDAQ command: write AcquDAQ config files
    if (!strcmp(cmd, "WRITE_ARDAQ"))
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

