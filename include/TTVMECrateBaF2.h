// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMECrateBaF2                                                       //
//                                                                      //
// TAPS BaF2 VME crate class.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMECRATEBAF2_H
#define TTVMECRATEBAF2_H

#include "TThread.h"

#include "TTVMECrate.h"
#include "TTVMEBaF2.h"
#include "TTCalibQAC.h"


class TTVMECrateBaF2 : public TTVMECrate
{

private:
    Bool_t fIsCalQACRunning;            // flag for QAC calibration run state

    static const Long_t fgBoardBase[];

public:
    TTVMECrateBaF2() : TTVMECrate(),
                       fIsCalQACRunning(kFALSE) { }
    TTVMECrateBaF2(Int_t id, Long_t vmeRange);
    virtual ~TTVMECrateBaF2() { }
    
    void Init();
    void StartCalibQAC(Int_t pedCh);
    void StopCalibQAC(Int_t pedCh) { fIsCalQACRunning = kFALSE; }
    
    Bool_t IsCalQACRunning() const { return fIsCalQACRunning; }

    static void* RunCalibQAC(void* arg);

    ClassDef(TTVMECrateBaF2, 0) // TAPS BaF2 VME crate
};

#endif

