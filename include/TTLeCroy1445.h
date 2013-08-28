// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTLeCroy1445                                                         //
//                                                                      //
// RS232 communication class for LeCroy 1445 high voltage controller.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTLECROY1445_H
#define TTLECROY1445_H

#include "TTRS232.h"
#include "TTUtils.h"


class TTLeCroy1445 : public TTRS232
{

private:
    virtual Bool_t Configure();
    Int_t GetHVStatus(Int_t mf);
    Bool_t ChangeHVStatus(Int_t mf, Bool_t status);
    
    Bool_t ValidateMainframe(Int_t mf);
    Bool_t ValidateChannel(Int_t c);
    Bool_t ValidateHVValue(Int_t val);

    enum { kOn, kOff, kUndef };

public:
    TTLeCroy1445() : TTRS232() { }
    TTLeCroy1445(const Char_t* device);
    virtual ~TTLeCroy1445() { }
 
    Bool_t IsHVOn(Int_t mf);
    Bool_t ReadHV(Int_t mf, Int_t c, Int_t* outDem, 
                  Int_t* outBack = 0, Int_t* outAc = 0);

    Bool_t TurnHVOn(Int_t mf);
    Bool_t TurnHVOff(Int_t mf);
    Bool_t WriteHV(Int_t mf, Int_t c, Int_t val);

    void PrintCmdHelp();

    ClassDef(TTLeCroy1445, 0) // LeCroy 1445 communication
};

#endif

