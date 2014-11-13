/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMEVeto                                                            //
//                                                                      //
// TAPS CAEN VME baseboard with Veto piggyback.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMEVETO_H
#define TTVMEVETO_H

#include "TTVMEV874.h"

class TTVMEVeto : public TTVMEV874
{

private:
    UInt_t* fPed;                   // array of pedestals
    UInt_t* fThr;                   // array of LED thresholds
    
    // registers
    static const Long_t fgRegPed[];
    static const Long_t fgRegThr[];

protected:
    virtual void SetDefaultThresholds();
    virtual void InitPiggyback();

public:
    TTVMEVeto() : TTVMEV874(),
                  fPed(0), fThr(0) { }
    TTVMEVeto(Long_t adr, Int_t len);
    virtual ~TTVMEVeto();
    
    void SetPedestal(UInt_t p);
    void SetPedestalChannel(Int_t c, UInt_t* p);
    void SetThreshold(UInt_t t);
    
    void WritePed();
    
    ClassDef(TTVMEVeto, 0) // TAPS CAEN VME Veto board
};

#endif

