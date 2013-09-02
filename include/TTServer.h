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


#ifndef TTSERVER_H
#define TTSERVER_H

#include "TTNetServer.h"


enum ETServerType {
    kNoServer,
    kBaF2Server,
    kVetoServer,
    kPWOServer,
    kHVServer
};
typedef ETServerType TServerType_t;


class TTServer : public TTNetServer
{

private:
    TServerType_t fType;                // TAPS server type

protected:
    virtual void ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServer() : TTNetServer(),
                 fType(kNoServer) { }
    TTServer(TServerType_t type, Int_t port);
    virtual ~TTServer();

    ClassDef(TTServer, 0) // TAPS server
};

#endif

