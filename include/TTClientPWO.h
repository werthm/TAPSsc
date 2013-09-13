// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientPWO                                                          //
//                                                                      //
// PWO TAPS client class.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENTPWO_H
#define TTCLIENTPWO_H

#include "TTClient.h"


class TTClientPWO : public TTClient
{

public:
    TTClientPWO() : TTClient() { }
    TTClientPWO(const Char_t* server, Int_t port);
    virtual ~TTClientPWO() { }
    
    ClassDef(TTClientPWO, 0) // PWO TAPS client
};

#endif

