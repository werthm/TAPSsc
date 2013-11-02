// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMECrate                                                           //
//                                                                      //
// Parent TAPS VME crate class.                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMECRATE_H
#define TTVMECRATE_H

#include "TTVMEKPh.h"


// crate type enum
enum ETCrateType {
    kNoCrate,
    kBaF2Crate,
    kVetoCrate,
    kPWOCrate,
};
typedef ETCrateType TCrateType_t;


class TTVMECrate
{

protected:
    Int_t fID;                      // crate ID
    TCrateType_t fType;             // TAPS crate type
    TTVMEKPh* fCtrl;                // VME controller

public:
    TTVMECrate() : fID(-1),
                   fType(kNoCrate),
                   fCtrl(0) { }
    TTVMECrate(Int_t id, TCrateType_t type, Long_t vmeRange);
    virtual ~TTVMECrate();
    
    TTVMEKPh* GetCtrl() const { return fCtrl; }

    ClassDef(TTVMECrate, 0) // TAPS VME crate
};

#endif

