// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMECrate                                                           //
//                                                                      //
// TAPS VME crate class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTVMECrate.h"

ClassImp(TTVMECrate)


//______________________________________________________________________________
TTVMECrate::TTVMECrate(Int_t id, TCrateType_t type, Long_t vmeRange,
                       Int_t nMod, Int_t nCh)
{
    // Constructor.
    
    // init members
    fID = id;
    fType = type;
    fCtrl = 0;
    fNMod = nMod;
    fNCh = nCh;

    // create VME controller
    fCtrl = new TTVMEKPh(vmeRange);
}

//______________________________________________________________________________
TTVMECrate::~TTVMECrate()
{
    // Destructor.
    
    if (fCtrl) delete fCtrl;
}

