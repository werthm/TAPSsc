// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientVeto                                                         //
//                                                                      //
// Veto TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENTVETO_H
#define TTCLIENTVETO_H

#include "TTClient.h"


class TTClientVeto : public TTClient
{

public:
    TTClientVeto() : TTClient() { }
    TTClientVeto(const Char_t* server, Int_t port);
    virtual ~TTClientVeto() { }
    
    ClassDef(TTClientVeto, 0) // Veto TAPS client
};

#endif

