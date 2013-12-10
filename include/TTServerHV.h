// SVN Info: $Id: TTServerHV.h 1718 2013-09-14 13:28:56Z werthm $

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
#include "TTMySQLManager.h"
#include "TTLeCroy1445.h"


class TTServerHV : public TTServer
{

private:
    TTLeCroy1445* fLeCroy;          // LeCroy 1445 communication
    
    Bool_t GetStatusHV(const Char_t* cmd, TSocket* s);
    Bool_t SetStatusHV(const Char_t* cmd, TSocket* s);
    Bool_t ReadHV(const Char_t* cmd, TSocket* s);
    Bool_t WriteHV(const Char_t* cmd, TSocket* s);

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerHV() : TTServer(),
                   fLeCroy(0) { }
    TTServerHV(Int_t port, Int_t id);
    virtual ~TTServerHV();

    ClassDef(TTServerHV, 0) // HV TAPS server
};

#endif

