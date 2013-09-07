// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerBaF2                                                         //
//                                                                      //
// BaF2 TAPS server class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVERBAF2_H
#define TTSERVERBAF2_H

#include "TTServer.h"


class TTServerBaF2 : public TTServer
{

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerBaF2() : TTServer() { }
    TTServerBaF2(Int_t port);
    virtual ~TTServerBaF2() { }

    ClassDef(TTServerBaF2, 0) // BaF2 TAPS server
};

#endif

