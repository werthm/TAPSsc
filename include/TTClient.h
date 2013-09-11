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
#include "TTDataTypePar.h"


class TTClient : public TTNetClient, public TObject
{

public:
    TTClient() : TTNetClient(), TObject() { }
    TTClient(const Char_t* server, Int_t port);
    virtual ~TTClient();
    
    TServerType_t GetType();

    Bool_t WriteHV(TTDataTypePar* d, Int_t elem);

    ClassDef(TTClient, 0) // TAPS client
};

#endif

