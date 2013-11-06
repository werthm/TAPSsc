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
#include "TTWriteADConfigVeto.h"
#include "TTVMECrateVeto.h"


class TTServerVeto : public TTServer
{

private:
    TTWriteADConfigVeto* fWriter;           // AcquDAQ config file writer
    TTVMECrateVeto* fCrate;                 // VME crate

    Bool_t WriteADConfig(const Char_t* cmd, TSocket* s);
    
    static const Long_t fgVMEBase;

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServerVeto() : TTServer(),
                     fWriter(0) { }
    TTServerVeto(Int_t port, Int_t id);
    virtual ~TTServerVeto();

    ClassDef(TTServerVeto, 0) // Veto TAPS server
};

#endif

