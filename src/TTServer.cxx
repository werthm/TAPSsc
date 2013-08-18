// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTNetServer                                                          //
//                                                                      //
// Network server class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTServer.h"

ClassImp(TTServer)


//______________________________________________________________________________
TTServer::TTServer(Int_t port)
    : TTNetServer(port)
{
    // Constructor.
    
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

    if (!strcmp(cmd, "SPECIAL"))
    {
        printf("Special operation\n");
    }
    else
    {
        Warning("ProcessCommand", "Unknown command '%s'", cmd);
    }
}

