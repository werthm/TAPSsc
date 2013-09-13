// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientBaF2                                                         //
//                                                                      //
// BaF2 TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENTBAF2_H
#define TTCLIENTBAF2_H

#include "TTClient.h"


class TTClientBaF2 : public TTClient
{

public:
    TTClientBaF2() : TTClient() { }
    TTClientBaF2(const Char_t* server, Int_t port);
    virtual ~TTClientBaF2() { }
    
    ClassDef(TTClientBaF2, 0) // BaF2 TAPS client
};

#endif

