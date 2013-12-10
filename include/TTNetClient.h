// SVN Info: $Id: TTNetClient.h 1715 2013-09-13 20:38:21Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTNetClient                                                          //
//                                                                      //
// Network client class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTNETCLIENT_H
#define TTNETCLIENT_H

#include "TSocket.h"
#include "TError.h"

#include "TTConfig.h"
#include "TTUtils.h"


class TTNetClient
{

protected:
    TSocket* fSocket;                       // client socket
    
public:
    TTNetClient() : fSocket(0) { }
    TTNetClient(const Char_t* server, Int_t port);
    virtual ~TTNetClient();
    
    TInetAddress GetHost() const { return fSocket->GetInetAddress(); }
    Int_t GetStatus();
    
    enum { kNoConn, kNoResp, kBadResp, kReady };

    ClassDef(TTNetClient, 0) // Network client
};

#endif

