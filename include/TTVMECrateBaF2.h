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
    TTCalibQAC* fCalibQAC;              // QAC pedestal calibration

    static const UShort_t fgPedInit;
    static const Long_t fgBoardBase[];

public:
    TTVMECrateBaF2() : TTVMECrate(),
                       fIsCalQACRunning(kFALSE), fCalibQAC(0) { }
    TTVMECrateBaF2(Int_t id, Long_t vmeRange);
    virtual ~TTVMECrateBaF2();
    
    void Init();
    void StartCalibQAC();
    void StopCalibQAC();
    
    Bool_t IsCalQACRunning() const { return fIsCalQACRunning; }
    TTCalibQAC* GetCalibQAC() const { return fCalibQAC; }

    static void* RunCalibQAC(void* arg);

    ClassDef(TTVMECrateBaF2, 0) // TAPS BaF2 VME crate
};

#endif

