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


#include "TTVMECrateBaF2.h"

ClassImp(TTVMECrateBaF2)


// init static class members
const Long_t TTVMECrateBaF2::fgBoardBase[] = { 0x30000000, 0x21000000, 0x22000000,
                                               0x23000000, 0x24000000, 0x25000000,
                                               0x26000000, 0x27000000, 0x28000000,
                                               0x29000000, 0x2a000000, 0x2b000000,
                                               0x2c000000, 0x2d000000, 0x2e000000,
                                               0x2f000000 };


//______________________________________________________________________________
TTVMECrateBaF2::TTVMECrateBaF2(Int_t id, Long_t vmeRange)
    : TTVMECrate(id, kBaF2Crate, vmeRange)
{
    // Constructor.
    
    // init members
    fIsCalQACRunning = kFALSE;

    // add modules to the crate
    for (Int_t i = 0; i < 16; i++)
    {
        // create new module
        TTVMEBaF2* m = new TTVMEBaF2(fgBoardBase[i], 0x4000);
        
        // add to controller -> memory mapping / object owner
        fCtrl->AddModule(m);
    }
}

//______________________________________________________________________________
void TTVMECrateBaF2::Init()
{
    // Init this crate with default settings.

    // init modules
    for (Int_t i = 0; i < 16; i++)
    {
        ((TTVMEBaF2*)fCtrl->GetModule(i))->Init();
    }
}

//______________________________________________________________________________
void TTVMECrateBaF2::StartCalibQAC(Int_t pedCh)
{
    // Start the calibration of the QAC pedestals.
    // All pedestal values will be calibrated to be in channel 'pedCh'.
    
    // configure modules
    for (Int_t i = 0; i < 16; i++)
    {
        // get the module
        TTVMEBaF2* m = (TTVMEBaF2*)fCtrl->GetModule(i);

        // set CFD thresholds to 0
        m->SetThresholdsCFD(0x0);

        // set LED1/LED2 thresholds to some higher value
        m->SetThresholdsLED1(0x1000);
        m->SetThresholdsLED2(0x1000);

        // set all pedestals to an initial value of 0x1000
        m->SetAllPedestals(0x1000);
        
        // enable digital test puls
        m->SetDigitalTestPuls(kTRUE);

        // init the board 
        m->Init();
    }
    
    // start QAC calibration thread
    fIsCalQACRunning = kTRUE;
    TThread* thread = new TThread("CalibQAC", (void(*)(void*))&RunCalibQAC, (void*) this);
    thread->Run();
}

//______________________________________________________________________________
void* TTVMECrateBaF2::RunCalibQAC(void* arg)
{
    // QAC calibration thread method.
    
    // get the crate
    TTVMECrateBaF2* crate = (TTVMECrateBaF2*)arg;
    
    // get controller
    TTVMEKPh* ctrl = crate->GetCtrl();
  
    // get modules
    TTVMEBaF2* m[16];
    for (Int_t i = 0; i < 16; i++) m[i] = (TTVMEBaF2*)ctrl->GetModule(i);
    
    // create QAC calibrations
    TTCalibQAC calib(16, 32);

    // main loop
    while (crate->IsCalQACRunning())
    {
        // loop over modules
        for (Int_t i = 0; i < 16; i++)
        {
            // readout the module 
            m[i]->Readout();

            // read the data
            Int_t nData = m[i]->GetNData();
            for (Int_t j = 0; j < nData; j++)
            {
                calib.AddData(i, m[i]->GetADC()[j], m[i]->GetData()[j]);
            }
        }
    }

    return 0;
}

