// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMEV874                                                            //
//                                                                      //
// TAPS CAEN VME baseboard.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMEV874_H
#define TTVMEV874_H

#include "TTVMEModule.h"


class TTVMEV874 : public TTVMEModule
{

private:
    // registers
    enum EReg { kHWID = 0,   kBitSet1, kBitClear1, kBitSet2, kBitClear2,
                kFastClrWin, kClrTime, kVSet,      kVOff,    kCtrlReg1,
                kThres,      kAux };
    static const Long_t fgReg[];

protected:
    UInt_t* fThres;                 // array of thresholds
    UShort_t fVOff;                 // TDC offset

    void Reset();
    void ResetData();
    void WritePiggyback(Long_t reg, UShort_t val);
    virtual void SetDefaultThresholds();
    virtual void InitPiggyback() = 0;

public:
    TTVMEV874() : fThres(0), fVOff(0) { }
    TTVMEV874(Long_t adr, Int_t len);
    virtual ~TTVMEV874();
    
    void Init();
    UShort_t ReadHWID() { return Read(fgReg[kHWID]); }

    ClassDef(TTVMEV874, 0) // TAPS CAEN VME baseboard
};

#endif

