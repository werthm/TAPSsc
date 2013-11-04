// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMEBaF2                                                            //
//                                                                      //
// TAPS CAEN VME baseboard with BaF2 piggyback.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMEBAF2_H
#define TTVMEBAF2_H

#include "TTVMEV874.h"


class TTVMEBaF2 : public TTVMEV874
{

private:
    UInt_t* fPedLG;                 // array of LG pedestals
    UInt_t* fPedLGS;                // array of LG sensitive pedestals
    UInt_t* fPedSG;                 // array of SG pedestals
    UInt_t* fPedSGS;                // array of SG sensitive pedestals
    UInt_t* fThrCFD;                // array of CFD thresholds
    UInt_t* fThrLED1;               // array of LED1 thresholds
    UInt_t* fThrLED2;               // array of LED2 thresholds
    
    // registers
    static const Long_t fgRegPedLG[];
    static const Long_t fgRegPedLGS[];
    static const Long_t fgRegPedSG[];
    static const Long_t fgRegPedSGS[];
    static const Long_t fgRegThrCFD[];
    static const Long_t fgRegThrLED1[];
    static const Long_t fgRegThrLED2[];
    
protected:
    virtual void SetDefaultThresholds();
    virtual void InitPiggyback();

public:
    TTVMEBaF2() : TTVMEV874(),
                  fPedLG(0), fPedLGS(0), fPedSG(0), fPedSGS(0),
                  fThrCFD(0), fThrLED1(0), fThrLED2(0) { }
    TTVMEBaF2(Long_t adr, Int_t len);
    virtual ~TTVMEBaF2();
    
    void SetPedestalAll(UInt_t p);
    void SetPedestalChannel(Int_t c, UInt_t* p);
    void SetThresholdCFD(UInt_t t);
    void SetThresholdLED1(UInt_t t);
    void SetThresholdLED2(UInt_t t);
    
    void WritePed();

    ClassDef(TTVMEBaF2, 0) // TAPS CAEN VME BaF2 board
};

#endif

