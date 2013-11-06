// SVN Info: $Id$

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


#include "TTVMEVeto.h"

ClassImp(TTVMEVeto)


// init static class members
const Long_t TTVMEVeto::fgRegPed[] = { 0x10, 0x11, 0x12, 0x13, 
                                       0x14, 0x15, 0x16, 0x17 };
const Long_t TTVMEVeto::fgRegThr[] = { 0x08, 0x09, 0x0A, 0x0B, 
                                       0x0C, 0x0D, 0x0E, 0x0F };


//______________________________________________________________________________
TTVMEVeto::TTVMEVeto(Long_t adr, Int_t len)
    : TTVMEV874(adr, len, 8)
{
    // Constructor.
    
    // overwrite parent members
    fVOff = 190;

    // init members
    fPed = new UInt_t[fNCh];
    fThr = new UInt_t[fNCh];

    // set default thresholds
    SetDefaultThresholds();
}

//______________________________________________________________________________
TTVMEVeto::~TTVMEVeto()
{
    // Destructor.

    if (fPed) delete [] fPed;
    if (fThr) delete [] fThr;
}

//______________________________________________________________________________
void TTVMEVeto::SetDefaultThresholds()
{
    // Set default thresholds.
    
    // baseboard thresholds
    for (Int_t i = 0; i < fNADC; i++)
    {
        if (i >=  0 && i <= 11) fThres[i] = 0x1ff;
        if (i >= 12 && i <= 27) fThres[i] = 0x1;
        if (i >= 28 && i <= 29) fThres[i] = 0x0;
        if (i >= 30 && i <= 31) fThres[i] = 0x1ff;
    }

    // piggyback thresholds
    for (Int_t i = 0; i < fNCh; i++)
    {
        fPed[i] = 4000;
        fThr[i] = 7820;
    }
}

//______________________________________________________________________________
void TTVMEVeto::WritePed()
{
    // Write the pedestal values to the piggyback board.
    
    // write pedestals
    for (Int_t i = 0; i < fNCh; i++)
    {
        WritePiggyback(fgRegPed[i], fPed[i]);
    }
}

//______________________________________________________________________________
void TTVMEVeto::InitPiggyback()
{
    // Init the Veto piggyback board.

    // write pedestals
    WritePed();
    
    // write discriminator thresholds
    for (Int_t i = 0; i < fNCh; i++)
    {
        WritePiggyback(fgRegThr[i], fThr[i]);
    }
}

//______________________________________________________________________________
void TTVMEVeto::SetPedestal(UInt_t p)
{
    // Set the pedestals of all channels to 'p'.

    for (Int_t i = 0; i < fNCh; i++) fPed[i] = p;
}

//______________________________________________________________________________
void TTVMEVeto::SetPedestalChannel(Int_t c, UInt_t* p)
{
    // Set the pedestal of the channel 'c' to the values stored in the array 'p'.

    fPed[c]  = p[0];
}

//______________________________________________________________________________
void TTVMEVeto::SetThreshold(UInt_t t)
{
    // Set the LED thresholds of all channels to 't'.

    for (Int_t i = 0; i < fNCh; i++) fThr[i] = t;
}

