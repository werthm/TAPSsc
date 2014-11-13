/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMECrateVeto                                                       //
//                                                                      //
// TAPS Veto VME crate class.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TError.h"
#include "TThread.h"

#include "TTVMECrateVeto.h"
#include "TTVMEVeto.h"
#include "TTVMEKPh.h"
#include "TTCalibQAC.h"

ClassImp(TTVMECrateVeto)

// init static class members
const UShort_t TTVMECrateVeto::fgPedInit = 0x1000;
const Long_t TTVMECrateVeto::fgBoardBase[] = { 0x50000000, 0x41000000, 0x42000000,
                                               0x43000000, 0x44000000, 0x45000000,
                                               0x46000000, 0x47000000, 0x48000000,
                                               0x49000000, 0x4a000000, 0x4b000000,
                                               0x4c000000, 0x4d000000, 0x4e000000,
                                               0x4f000000 };

//______________________________________________________________________________
TTVMECrateVeto::TTVMECrateVeto(Int_t id, Long_t vmeRange)
    : TTVMECrate(id, kVetoCrate, vmeRange, 16, 8)
{
    // Constructor.
    
    // init members
    fIsCalQACRunning = kFALSE;
    fCalibQAC = 0;

    // add modules to the crate
    for (Int_t i = 0; i < fNMod; i++)
    {
        // create new module
        TTVMEVeto* m = new TTVMEVeto(fgBoardBase[i], 0x4000);
        
        // add to controller -> memory mapping / object owner
        fCtrl->AddModule(m);
    }
}

//______________________________________________________________________________
TTVMECrateVeto::~TTVMECrateVeto()
{
    // Destructor.

    if (fCalibQAC) delete fCalibQAC;
}

//______________________________________________________________________________
Bool_t TTVMECrateVeto::StartCalibQAC(Bool_t initFromDB)
{
    // Start the calibration of the QAC pedestals.
    // If 'initFromDB' is kTRUE, initialize the pedestals using the database.
    // Return kTRUE on success, otherwise kFALSE.

    // exit if QAC calibration is already running
    if (fIsCalQACRunning) return kFALSE;
    
    // create QAC calibration
    if (fCalibQAC) delete fCalibQAC;
    fCalibQAC = new TTCalibQAC(fID, kTRUE, fgPedInit);

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
        TTVMEVeto* m = (TTVMEVeto*)fCtrl->GetModule(i);

        // set thresholds to 0
        m->SetThreshold(0x0);

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
void TTVMECrateVeto::StopCalibQAC()
{
    // Stop the calibration of the QAC pedestals.

    TThread::Lock();
    fIsCalQACRunning = kFALSE;
    TThread::UnLock();
}
 
//______________________________________________________________________________
void* TTVMECrateVeto::RunCalibQAC(void* arg)
{
    // QAC calibration thread method.
    
    // get the crate
    TTVMECrateVeto* crate = (TTVMECrateVeto*)arg;
    
    // get controller
    TTVMEKPh* ctrl = crate->GetCtrl();
  
    // get modules
    TTVMEVeto* m[crate->GetNModule()];
    for (Int_t i = 0; i < crate->GetNModule(); i++) m[i] = (TTVMEVeto*)ctrl->GetModule(i);
    
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

