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
    : TTVMEV874(adr, len, 4)
{
    // Constructor.
    
    // overwrite parent members
    fVOff = 165;

    // init members
    fPedLG   = new UInt_t[fNCh];
    fPedLGS  = new UInt_t[fNCh];
    fPedSG   = new UInt_t[fNCh];
    fPedSGS  = new UInt_t[fNCh];
    fThrCFD  = new UInt_t[fNCh];
    fThrLED1 = new UInt_t[fNCh];
    fThrLED2 = new UInt_t[fNCh];

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
        
    // baseboard thresholds
    for (Int_t i = 0; i < fNADC; i++)
    {
        if (i >=  0 && i <=  7) fThres[i] = 0x1ff;
        if (i >=  8 && i <= 27) fThres[i] = 0x1;
        if (i >= 28 && i <= 29) fThres[i] = 0x0;
        if (i >= 30 && i <= 31) fThres[i] = 0x1ff;
    }

    // piggyback thresholds
    for (Int_t i = 0; i < fNCh; i++)
    {
        fPedLG[i]   = 4000;
        fPedLGS[i]  = 4000;
        fPedSG[i]   = 4000;
        fPedSGS[i]  = 4000;
        fThrCFD[i]  = 7820;
        fThrLED1[i] = 7820;
        fThrLED2[i] = 7820;
    }
}

//______________________________________________________________________________
void TTVMEBaF2::WritePed()
{
    // Write the pedestal values to the piggyback board.
    
    // write pedestals
    for (Int_t i = 0; i < fNCh; i++)
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
    for (Int_t i = 0; i < fNCh; i++)
    {
        WritePiggyback(fgRegThrCFD[i], fThrCFD[i]);
        WritePiggyback(fgRegThrLED1[i], fThrLED1[i]);
        WritePiggyback(fgRegThrLED2[i], fThrLED2[i]);
    }
}

//______________________________________________________________________________
void TTVMEBaF2::SetPedestalAll(UInt_t p)
{
    // Set all pedestals (LG/LGS/SG/SGS) of all channels to 'p'.

    for (Int_t i = 0; i < fNCh; i++) 
    {
        fPedLG[i]  = p;
        fPedLGS[i] = p;
        fPedSG[i]  = p;
        fPedSGS[i] = p;
    }
}

//______________________________________________________________________________
void TTVMEBaF2::SetPedestalChannel(Int_t c, UInt_t* p)
{
    // Set all pedestals (LG/LGS/SG/SGS) of the channel 'c' to the values 
    // stored in the array 'p'.

    fPedLG[c]  = p[0];
    fPedLGS[c] = p[1];
    fPedSG[c]  = p[2];
    fPedSGS[c] = p[3];
}

//______________________________________________________________________________
void TTVMEBaF2::SetThresholdCFD(UInt_t t)
{
    // Set the CFD thresholds of all channels to 't'.

    for (Int_t i = 0; i < fNCh; i++) fThrCFD[i] = t;
}

//______________________________________________________________________________
void TTVMEBaF2::SetThresholdLED1(UInt_t t)
{
    // Set the LED1 thresholds of all channels to 't'.

    for (Int_t i = 0; i < fNCh; i++) fThrLED1[i] = t;
}

//______________________________________________________________________________
void TTVMEBaF2::SetThresholdLED2(UInt_t t)
{
    // Set the LED2 thresholds of all channels to 't'.

    for (Int_t i = 0; i < fNCh; i++) fThrLED2[i] = t;
}

