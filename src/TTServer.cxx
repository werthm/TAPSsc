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
TTServer::TTServer(TServerType_t type, Int_t port, Int_t id)
    : TTNetServer(port)
{
    // Constructor.
    
    // init members
    fID = id;
    fType = type;
    fVMERange = 0;
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
    
    // get the network command
    Int_t nc = TTUtils::GetNetworkCmd(cmd);
    
    // type command: return the server type
    if (nc == TTConfig::kNCType)
    {
        Char_t tmp[8];
        sprintf(tmp, "%d %d", TTConfig::kNCTypeRet, fType);
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

