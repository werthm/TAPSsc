// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerVeto                                                         //
//                                                                      //
// Veto TAPS server class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVERVETO_H
#define TTSERVERVETO_H

#include "TTServer.h"


class TTServerVeto : public TTServer
{

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerVeto() : TTServer() { }
    TTServerVeto(Int_t port, Int_t id);
    virtual ~TTServerVeto() { }

    ClassDef(TTServerVeto, 0) // Veto TAPS server
};

#endif

