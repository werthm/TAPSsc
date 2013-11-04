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
#include "TTCalibQAC.h"


class TTClientBaF2 : public TTClient
{

public:
    TTClientBaF2() : TTClient() { }
    TTClientBaF2(const Char_t* server, Int_t port);
    virtual ~TTClientBaF2() { }
    
    Bool_t WriteADConfig();
    Bool_t StartCalibQAC();
    Bool_t StopCalibQAC();

    ClassDef(TTClientBaF2, 0) // BaF2 TAPS client
};

#endif

