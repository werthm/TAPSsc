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


#include "TError.h"
#include "TThread.h"

#include "TTVMECrateBaF2.h"
#include "TTVMEBaF2.h"
#include "TTVMEKPh.h"
#include "TTCalibQAC.h"

ClassImp(TTVMECrateBaF2)

// init static class members
const UShort_t TTVMECrateBaF2::fgPedInit = 0x1000;
const Long_t TTVMECrateBaF2::fgBoardBase[] = { 0x30000000, 0x21000000, 0x22000000,
                                               0x23000000, 0x24000000, 0x25000000,
                                               0x26000000, 0x27000000, 0x28000000,
                                               0x29000000, 0x2a000000, 0x2b000000,
                                               0x2c000000, 0x2d000000, 0x2e000000,
                                               0x2f000000 };

//______________________________________________________________________________
TTVMECrateBaF2::TTVMECrateBaF2(Int_t id, Long_t vmeRange)
    : TTVMECrate(id, kBaF2Crate, vmeRange, 16, 4)
{
    // Constructor.
    
    // init members
    fIsCalQACRunning = kFALSE;
    fCalibQAC = 0;

    // add modules to the crate
    for (Int_t i = 0; i < fNMod; i++)
    {
        // create new module
        TTVMEBaF2* m = new TTVMEBaF2(fgBoardBase[i], 0x4000);
        
        // add to controller -> memory mapping / object owner
        fCtrl->AddModule(m);
    }
}

//______________________________________________________________________________
TTVMECrateBaF2::~TTVMECrateBaF2()
{
    // Destructor.

    if (fCalibQAC) delete fCalibQAC;
}

//______________________________________________________________________________
Bool_t TTVMECrateBaF2::StartCalibQAC(Bool_t initFromDB)
{
    // Start the calibration of the QAC pedestals.
    // If 'initFromDB' is kTRUE, initialize the pedestals using the database.
    // Return kTRUE on success, otherwise kFALSE.

    // exit if QAC calibration is already running
    if (fIsCalQACRunning) return kFALSE;
    
    // create QAC calibration
    if (fCalibQAC) delete fCalibQAC;
    fCalibQAC = new TTCalibQAC(fID, kFALSE, fgPedInit);

    // try to init from database
    if (initFromDB) 
    {
        if (!fCalibQAC->InitPedFromDB())
        {
            Error("StartCalibQAC", "Could not initialize pedestal values from database - aborting!");
            return kFALSE;
        }
    }

    // configure modules
    for (Int_t i = 0; i < fNMod; i++)
    {
        // get the module
        TTVMEBaF2* m = (TTVMEBaF2*)fCtrl->GetModule(i);

        // set CFD thresholds to 0
        m->SetThresholdCFD(0x0);

        // set LED1/LED2 thresholds to some higher value
        m->SetThresholdLED1(0x1000);
        m->SetThresholdLED2(0x1000);

        // set the initial pedestal values for all channels
        for (Int_t j = 0; j < fNCh; j++)
            m->SetPedestalChannel(j, fCalibQAC->GetPedestal(i, j));
        
        // enable digital test puls
        m->SetDigitalTestPuls(kTRUE);

        // init the board 
        m->Init();
    }
    
    // start QAC calibration thread
    fIsCalQACRunning = kTRUE;
    TThread* thread = new TThread("CalibQAC", (void(*)(void*))&RunCalibQAC, (void*) this);
    thread->Run();

    return kTRUE;
}

//______________________________________________________________________________
void TTVMECrateBaF2::StopCalibQAC()
{
    // Stop the calibration of the QAC pedestals.

    TThread::Lock();
    fIsCalQACRunning = kFALSE;
    TThread::UnLock();
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
    TTVMEBaF2* m[crate->GetNModule()];
    for (Int_t i = 0; i < crate->GetNModule(); i++) m[i] = (TTVMEBaF2*)ctrl->GetModule(i);
    
    // get QAC calibration
    TTCalibQAC* calib = crate->GetCalibQAC();

    // main loop
    for (;;)
    {
        // check if calibration should stop
        TThread::Lock();
        Bool_t st = crate->IsCalQACRunning();
        TThread::UnLock();
        if (!st) break;
        
        // loop over modules
        for (Int_t i = 0; i < crate->GetNModule(); i++)
        {
            // readout the module 
            m[i]->Readout();

            // read the data
            Int_t nData = m[i]->GetNData();
            for (Int_t j = 0; j < nData; j++)
            {
                // add data to calibration
                calib->AddData(i, m[i]->GetADC()[j], m[i]->GetData()[j]);
            }
        }

        // try to update the calibration and re-init the board if new pedestal
        // values were calculated
        if (calib->UpdateCalib())
        {
            // loop over modules
            for (Int_t i = 0; i < crate->GetNModule(); i++)
            {
                // loop over channels
                for (Int_t j = 0; j < crate->GetNChannel(); j++)
                {
                    m[i]->SetPedestalChannel(j, calib->GetPedestal(i, j));
                }

                // write the new pedestal values to the hardware
                m[i]->WritePed();
            }
        }
    }

    return 0;
}

