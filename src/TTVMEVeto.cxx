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
    : TTVMEV874(adr, len)
{
    // Constructor.
    
    // overwrite parent members
    fVOff = 190;

    // init members
    fPed = new UInt_t[8];
    fThr = new UInt_t[8];

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
    
    for (Int_t i = 0; i < 32; i++)
    {
        if (i >=  0 && i <= 11) fThres[i] = 0x1ff;
        if (i >= 12 && i <= 27) fThres[i] = 0x1;
        if (i >= 28 && i <= 29) fThres[i] = 0x0;
        if (i >= 30 && i <= 31) fThres[i] = 0x1ff;
    }
}

//______________________________________________________________________________
void TTVMEVeto::WritePed()
{
    // Write the pedestal values to the piggyback board.
    
    // write pedestals
    for (Int_t i = 0; i < 8; i++)
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
    for (Int_t i = 0; i < 4; i++)
    {
        WritePiggyback(fgRegThr[i], fThr[i]);
    }
}

