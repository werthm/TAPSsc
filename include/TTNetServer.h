// SVN Info: $Id: TTNetServer.h 1715 2013-09-13 20:38:21Z werthm $

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
#include "TMonitor.h"
#include "TMessage.h"
#include "TError.h"
#include "TList.h"

#include "TTConfig.h"
#include "TTUtils.h"


class TTNetServer
{

protected:
    TServerSocket* fServer;                 // server socket
    Bool_t fIsRunning;                      // running flag
    
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTNetServer() : fServer(0), fIsRunning(kFALSE) { }
    TTNetServer(Int_t port);
    virtual ~TTNetServer();

    void Listen();
    void StopListening();

    ClassDef(TTNetServer, 0) // Network server
};

#endif

