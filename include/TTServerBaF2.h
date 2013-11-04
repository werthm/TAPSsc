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
#include "TTWriteADConfigBaF2.h"
#include "TTVMECrateBaF2.h"


class TTServerBaF2 : public TTServer
{

private:
    TTWriteADConfigBaF2* fWriter;           // AcquDAQ config file writer
    TTVMECrateBaF2* fCrate;                 // VME crate

    Bool_t WriteADConfig(const Char_t* cmd, TSocket* s);
    
    static const Long_t fgVMEBase;

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerBaF2() : TTServer(),
                     fWriter(0), fCrate(0) { }
    TTServerBaF2(Int_t port, Int_t id);
    virtual ~TTServerBaF2();

    ClassDef(TTServerBaF2, 0) // BaF2 TAPS server
};

#endif

