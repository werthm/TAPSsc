// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServer                                                             //
//                                                                      //
// TAPS server class.                                                   //
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
void TTServer::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    
    // call parent method
    TTNetServer::ProcessCommand(cmd, s);
    
    // TYPE command: return the server type
    if (!strcmp(cmd, "TYPE"))
    {
        Char_t tmp[8];
        sprintf(tmp, "%d", fType);
        s->Send(tmp);
    }
    else
    {
        Warning("ProcessCommand", "Unknown command '%s'", cmd);
    }
}

