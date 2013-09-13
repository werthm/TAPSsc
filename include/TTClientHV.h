// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientHV                                                           //
//                                                                      //
// HV TAPS client class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENTHV_H
#define TTCLIENTHV_H

#include "TTClient.h"
#include "TTDataTypePar.h"


class TTClientHV : public TTClient
{

public:
    TTClientHV() : TTClient() { }
    TTClientHV(const Char_t* server, Int_t port);
    virtual ~TTClientHV() { }
    
    Bool_t ReadHV(TTDataTypePar* d, Int_t elem, Int_t* outHV);
    Bool_t WriteHV(TTDataTypePar* d, Int_t elem);
    Bool_t GetStatusHV(Int_t mainframe, Bool_t* outSt);
    Bool_t SetStatusHV(Int_t mainframe, Bool_t status);

    ClassDef(TTClientHV, 0) // HV TAPS client
};

#endif

