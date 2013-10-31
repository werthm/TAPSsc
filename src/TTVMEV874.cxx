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


#include "TTVMEV874.h"

ClassImp(TTVMEV874)


// init static class members
const Long_t TTVMEV874::fgReg[] = { 0x1000, 0x1006, 0x1008, 0x1032, 0x1034,
                                    0x102e, 0x1066, 0x1060, 0x1062, 0x1010,
                                    0x1080, 0x1200 };


//______________________________________________________________________________
TTVMEV874::TTVMEV874(Long_t adr, Int_t len)
    : TTVMEModule(adr, len)
{
    // Constructor.
    
    // init members
    fThres = new UInt_t[32];
    fVOff = 165;

    // set default thresholds
    SetDefaultThresholds();
}

//______________________________________________________________________________
TTVMEV874::~TTVMEV874()
{
    // Destructor.

    if (fThres) delete [] fThres;
}

//______________________________________________________________________________
void TTVMEV874::Reset()
{
    // Perform a software-reset of the board.

    Write(fgReg[kBitSet1], 0x8); 
    Write(fgReg[kBitClear1], 0x8);
}

//______________________________________________________________________________
void TTVMEV874::ResetData()
{
    // Reset the data buffers.

    Write(fgReg[kBitSet2], 0x4);
    Write(fgReg[kBitClear2], 0x4);
}

//______________________________________________________________________________
void TTVMEV874::WritePiggyback(Long_t reg, UShort_t val)
{
    // Write the 13-bit value 'val' to the piggyback board at register 'reg'.
    
    Write(fgReg[kBitClear2], 0x100);
    Write(fgReg[kAux], val & 0xff);
    Write(fgReg[kAux] + (0x38 << 1), ((val >> 8) & 0x1f));
    Write(fgReg[kAux] + ((0x40 | reg) << 1), 0);
    Write(fgReg[kAux] + ((0x40 | reg) << 1), 0);
    Write(fgReg[kBitSet2], 0x100);
}

//______________________________________________________________________________
void TTVMEV874::SetDefaultThresholds()
{
    // Set default thresholds.

    for (Int_t i = 0; i < 32; i++)
    {
        fThres[i] = 0x0;
    }
}

//______________________________________________________________________________
void TTVMEV874::Init()
{
    // Init the board.

    // software reset
    Reset();

    // ADC offline
    Write(fgReg[kBitSet2], 0x2);
        
    // set to setup mode
    Write(fgReg[kBitClear2], 0x100);
        
    // auto-increment data pointer
    Write(fgReg[kBitSet2], 0x800);
        
    // disable overflow suppression
    Write(fgReg[kBitSet2], 0x8);
        
    // enable below threshold suppression
    Write(fgReg[kBitClear2], 0x10);
        
    // enable common stop mode
    Write(fgReg[kBitSet2], 0x400);
        
    // set fast clear window
    Write(fgReg[kFastClrWin], 500);
        
    // set clear time
    Write(fgReg[kClrTime], 500);
        
    // set TDC gain
    Write(fgReg[kVSet], 43);
        
    // set TDC offset
    Write(fgReg[kVOff], fVOff);
        
    // no idea what this is
    Write(fgReg[kCtrlReg1], 0);
        
    // write channel thresholds
    for (Int_t i = 0; i < 32; i++)
    {
        Write(fgReg[kThres]+2*i, fThres[i]);
    }
    
    // init the piggyback board
    InitPiggyback();

    // set to readout mode
    Write(fgReg[kBitSet2], 0x100);

    // ADC online
    Write(fgReg[kBitClear2], 0x2);

    // clear data
    ResetData();
}

