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
#include "TTUtils.h"


class TTClient : public TTNetClient, public TObject
{

public:
    TTClient() : TTNetClient(), TObject() { }
    TTClient(const Char_t* server, Int_t port);
    virtual ~TTClient();
    
    TServerType_t GetType();

    Bool_t ReadHV(TTDataTypePar* d, Int_t elem, Int_t* outHV);
    Bool_t WriteHV(TTDataTypePar* d, Int_t elem);

    ClassDef(TTClient, 0) // TAPS client
};

#endif

