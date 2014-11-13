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

