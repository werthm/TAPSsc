// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServer                                                             //
//                                                                      //
// Parent TAPS server class.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServer.h"

ClassImp(TTServer)


//______________________________________________________________________________
TTServer::TTServer(TServerType_t type, Int_t port)
    : TTNetServer(port)
{
    // Constructor.
    
    fType = type;
}

//______________________________________________________________________________
TTServer::~TTServer()
{
    // Destructor.

}

//______________________________________________________________________________
Bool_t TTServer::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    // Return kTRUE if the command was accepted, otherwise kFALSE.
    
    // TYPE command: return the server type
    if (!strcmp(cmd, "TYPE"))
    {
        Char_t tmp[8];
        sprintf(tmp, "%d", fType);
        s->Send(tmp);
        return kTRUE;
    }
    else
    {
        // call parent method
        if (TTNetServer::ProcessCommand(cmd, s)) return kTRUE;
        else
        {
            Warning("ProcessCommand", "Unknown command '%s'", cmd);
            return kFALSE;
        }
    }
}

