// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTCalibQAC                                                           //
//                                                                      //
// QAC pedestal calibration for TAPS BaF2/Veto boards.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTCalibQAC.h"

ClassImp(TTCalibQAC)


//______________________________________________________________________________
TTCalibQAC::TTCalibQAC(Int_t nMod, Int_t nADC)
{
    // Constructor.
    
    // init members
    fNModule = nMod;
    fNADC = nADC;
    fDataMean = new Double_t*[fNModule];
    fDataRead = new Int_t*[fNModule];
    for (Int_t i = 0; i < fNModule; i++)
    {
        fDataMean[i] = new Double_t[fNADC];
        fDataRead[i] = new Int_t[fNADC];
    }
    
    // clear data
    ClearData();
}

//______________________________________________________________________________
TTCalibQAC::~TTCalibQAC()
{
    // Destructor.
    
    if (fDataMean)
    {
        for (Int_t i = 0; i < fNModule; i++)
        {
            delete [] fDataMean[i];
            delete [] fDataRead[i];
        }
        delete [] fDataMean;
        delete [] fDataRead;
    }
}

//______________________________________________________________________________
void TTCalibQAC::AddData(Int_t mod, Int_t adc, Int_t val)
{
    // Add the data 'val' from ADC 'adc' of module 'mod'.

    fDataMean[mod][adc] =+ val;
    fDataRead[mod][adc]++;
}

//______________________________________________________________________________
void TTCalibQAC::ClearData()
{
    // Clear the recorded data.

    for (Int_t i = 0; i < fNModule; i++)
    {
        for (Int_t j = 0; j < fNADC; j++)
        {
            fDataMean[i][j] = 0.;
            fDataRead[i][j] = 0;
        }
    }
}

