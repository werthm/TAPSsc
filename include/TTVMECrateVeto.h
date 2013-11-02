// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMECrateVeto                                                       //
//                                                                      //
// TAPS Veto VME crate class.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMECRATEVETO_H
#define TTVMECRATEVETO_H

#include "TTVMECrate.h"
#include "TTVMEVeto.h"


class TTVMECrateVeto : public TTVMECrate
{

private:
    static const Long_t fgBoardBase[];

public:
    TTVMECrateVeto() : TTVMECrate() { }
    TTVMECrateVeto(Int_t id, Long_t vmeRange);
    virtual ~TTVMECrateVeto() { }
    
    void Init();
    
    ClassDef(TTVMECrateVeto, 0) // TAPS Veto VME crate
};

#endif

