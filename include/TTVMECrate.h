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

#include "Rtypes.h"

class TTVMEKPh;

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
    Int_t fNMod;                    // number of modules
    Int_t fNCh;                     // number of channels per module

public:
    TTVMECrate() : fID(-1),
                   fType(kNoCrate),
                   fCtrl(0),
                   fNMod(0), fNCh(0) { }
    TTVMECrate(Int_t id, TCrateType_t type, Long_t vmeRange,
               Int_t nMod, Int_t nCh);
    virtual ~TTVMECrate();
    
    TTVMEKPh* GetCtrl() const { return fCtrl; }
    Int_t GetNModule() const { return fNMod; }
    Int_t GetNChannel() const { return fNCh; }

    ClassDef(TTVMECrate, 0) // TAPS VME crate
};

#endif

