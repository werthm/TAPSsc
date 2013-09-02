// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClient                                                             //
//                                                                      //
// TAPS client class.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENT_H
#define TTCLIENT_H

#include <cstdlib>

#include "TTNetClient.h"
#include "TTServer.h"


class TTClient : public TTNetClient
{

public:
    TTClient() : TTNetClient() { }
    TTClient(const Char_t* server, Int_t port);
    virtual ~TTClient();
    
    TServerType_t GetType();

    ClassDef(TTClient, 0) // TAPS client
};

#endif

