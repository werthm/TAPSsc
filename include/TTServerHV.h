// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerHV                                                           //
//                                                                      //
// HV TAPS server class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVERHV_H
#define TTSERVERHV_H

#include "TTServer.h"


class TTServerHV : public TTServer
{

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerHV() : TTServer() { }
    TTServerHV(Int_t port);
    virtual ~TTServerHV() { }

    ClassDef(TTServerHV, 0) // HV TAPS server
};

#endif

