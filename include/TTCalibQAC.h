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


#ifndef TTVMECALIBQAC_H
#define TTVMECALIBQAC_H


#include "Rtypes.h"


class TTCalibQAC
{

private:
    Int_t fNModule;             // number of modules
    Int_t fNADC;                // number of ADCs
    Double_t** fDataMean;       //[fNModule][fNADC] channel sum/mean
    Int_t** fDataRead;          //[fNModule][fNADC] number of channel read events

public:
    TTCalibQAC() : fNModule(0), fNADC(0),
                   fDataMean(0), fDataRead(0) { }
    TTCalibQAC(Int_t nMod, Int_t nADC);
    virtual ~TTCalibQAC();

    void AddData(Int_t mod, Int_t adc, Int_t val);
    void ClearData();
    
    ClassDef(TTCalibQAC, 0) // BaF2/Veto board QAC calibration
};

#endif

