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


#include "TTVMEBaF2.h"

ClassImp(TTVMEBaF2)


// init static class members
const Long_t TTVMEBaF2::fgRegPedLG[]   = { 0x0f, 0x0e, 0x09, 0x08 };
const Long_t TTVMEBaF2::fgRegPedLGS[]  = { 0x10, 0x11, 0x16, 0x17 };
const Long_t TTVMEBaF2::fgRegPedSG[]   = { 0x1f, 0x1e, 0x19, 0x18 };
const Long_t TTVMEBaF2::fgRegPedSGS[]  = { 0x20, 0x21, 0x26, 0x27 };
const Long_t TTVMEBaF2::fgRegThrCFD[]  = { 0x24, 0x1c, 0x14, 0x0c };
const Long_t TTVMEBaF2::fgRegThrLED1[] = { 0x22, 0x1a, 0x12, 0x0a };
const Long_t TTVMEBaF2::fgRegThrLED2[] = { 0x23, 0x1b, 0x13, 0x0b };


//______________________________________________________________________________
TTVMEBaF2::TTVMEBaF2(Long_t adr, Int_t len)
    : TTVMEV874(adr, len)
{
    // Constructor.
    
    // overwrite parent members
    fVOff = 165;

    // init members
    fPedLG   = new UInt_t[4];
    fPedLGS  = new UInt_t[4];
    fPedSG   = new UInt_t[4];
    fPedSGS  = new UInt_t[4];
    fThrCFD  = new UInt_t[4];
    fThrLED1 = new UInt_t[4];
    fThrLED2 = new UInt_t[4];

    // set default thresholds
    SetDefaultThresholds();
}

//______________________________________________________________________________
TTVMEBaF2::~TTVMEBaF2()
{
    // Destructor.

    if (fPedLG) delete [] fPedLG;
    if (fPedLGS) delete [] fPedLGS;
    if (fPedSG) delete [] fPedSG;
    if (fPedSGS) delete [] fPedSGS;
    if (fThrCFD) delete [] fThrCFD;
    if (fThrLED1) delete [] fThrLED1;
    if (fThrLED2) delete [] fThrLED2;
}

//______________________________________________________________________________
void TTVMEBaF2::SetDefaultThresholds()
{
    // Set default thresholds.
    
    for (Int_t i = 0; i < 32; i++)
    {
        if (i >=  0 && i <=  7) fThres[i] = 0x1ff;
        if (i >=  8 && i <= 27) fThres[i] = 0x1;
        if (i >= 28 && i <= 29) fThres[i] = 0x0;
        if (i >= 30 && i <= 31) fThres[i] = 0x1ff;
    }
}

//______________________________________________________________________________
void TTVMEBaF2::WritePed()
{
    // Write the pedestal values to the piggyback board.
    
    // write pedestals
    for (Int_t i = 0; i < 4; i++)
    {
        WritePiggyback(fgRegPedLG[i], fPedLG[i]);
        WritePiggyback(fgRegPedLGS[i], fPedLGS[i]);
        WritePiggyback(fgRegPedSG[i], fPedSG[i]);
        WritePiggyback(fgRegPedSGS[i], fPedSGS[i]);
    }
}

//______________________________________________________________________________
void TTVMEBaF2::InitPiggyback()
{
    // Init the BaF2 piggyback board.

    // write pedestals
    WritePed();
    
    // write discriminator thresholds
    for (Int_t i = 0; i < 4; i++)
    {
        WritePiggyback(fgRegThrCFD[i], fThrCFD[i]);
        WritePiggyback(fgRegThrLED1[i], fThrLED1[i]);
        WritePiggyback(fgRegThrLED2[i], fThrLED2[i]);
    }
}

