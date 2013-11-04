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


#include "TTVMECrateVeto.h"

ClassImp(TTVMECrateVeto)


// init static class members
const Long_t TTVMECrateVeto::fgBoardBase[] = { 0x50000000, 0x41000000, 0x42000000,
                                               0x43000000, 0x44000000, 0x45000000,
                                               0x46000000, 0x47000000, 0x48000000,
                                               0x49000000, 0x4a000000, 0x4b000000,
                                               0x4c000000, 0x4d000000, 0x4e000000,
                                               0x4f000000 };


//______________________________________________________________________________
TTVMECrateVeto::TTVMECrateVeto(Int_t id, Long_t vmeRange)
    : TTVMECrate(id, kVetoCrate, vmeRange, 16, 8)
{
    // Constructor.
    
    // add modules to the crate
    for (Int_t i = 0; i < 16; i++)
    {
        // create new module
        TTVMEVeto* m = new TTVMEVeto(fgBoardBase[i], 0x4000);
        
        // add to controller -> memory mapping
        fCtrl->AddModule(m);
    }
}

//______________________________________________________________________________
void TTVMECrateVeto::Init()
{
    // Init this crate with default settings.

    // init modules
    for (Int_t i = 0; i < 16; i++)
    {
        ((TTVMEVeto*)fCtrl->GetModule(i))->Init();
    }
}

