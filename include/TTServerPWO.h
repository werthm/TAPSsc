// SVN Info: $Id: TTServerPWO.h 1718 2013-09-14 13:28:56Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerPWO                                                          //
//                                                                      //
// PWO TAPS server class.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVERPWO_H
#define TTSERVERPWO_H

#include "TTServer.h"


class TTServerPWO : public TTServer
{

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerPWO() : TTServer() { }
    TTServerPWO(Int_t port, Int_t id);
    virtual ~TTServerPWO() { }

    ClassDef(TTServerPWO, 0) // PWO TAPS server
};

#endif

