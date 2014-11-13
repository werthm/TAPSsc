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

class TTCalibQAC;

class TTVMECrateVeto : public TTVMECrate
{

private:
    Bool_t fIsCalQACRunning;            // flag for QAC calibration run state
    TTCalibQAC* fCalibQAC;              // QAC pedestal calibration

    static const UShort_t fgPedInit;
    static const Long_t fgBoardBase[];

public:
    TTVMECrateVeto() : TTVMECrate(),
                       fIsCalQACRunning(kFALSE), fCalibQAC(0) { }
    TTVMECrateVeto(Int_t id, Long_t vmeRange);
    virtual ~TTVMECrateVeto();
    
    Bool_t StartCalibQAC(Bool_t initFromDB = kTRUE);
    void StopCalibQAC();
    
    Bool_t IsCalQACRunning() const { return fIsCalQACRunning; }
    TTCalibQAC* GetCalibQAC() const { return fCalibQAC; }

    static void* RunCalibQAC(void* arg);

    ClassDef(TTVMECrateVeto, 0) // TAPS Veto VME crate
};

#endif

