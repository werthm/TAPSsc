// SVN Info: $Id: TTClient.h 1715 2013-09-13 20:38:21Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClient                                                             //
//                                                                      //
// Parent TAPS client class.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENT_H
#define TTCLIENT_H

//#include <cstdlib>

#include "TTNetClient.h"
#include "TTServer.h"


class TTClient : public TTNetClient, public TObject
{

public:
    TTClient() : TTNetClient(), TObject() { }
    TTClient(const Char_t* server, Int_t port);
    virtual ~TTClient();
    
    TServerType_t GetType();

    ClassDef(TTClient, 0) // Parent TAPS client
};

#endif

