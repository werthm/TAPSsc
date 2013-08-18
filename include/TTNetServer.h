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


#ifndef TTNETSERVER_H
#define TTNETSERVER_H

#include "TServerSocket.h"
#include "TMessage.h"
#include "TSystem.h"
#include "TError.h"


class TTNetServer
{

protected:
    TServerSocket* fServer;                 // server socket
    Bool_t fIsRunning;                      // running flag
    
    virtual void ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTNetServer() : fServer(0), fIsRunning(kFALSE) { }
    TTNetServer(Int_t port);
    virtual ~TTNetServer();

    void StartListening();
    void StopListening();

    ClassDef(TTNetServer, 0) // Network server
};

#endif

