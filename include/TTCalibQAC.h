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


#include "TObject.h"
#include "TMath.h"

#include "TTMySQLManager.h"


class TTCalibQAC : public TObject
{

private:
    Int_t fCrateID;             // crate ID
    Bool_t fIsVeto;             // Veto flag
    Int_t fNModule;             // number of modules
    Int_t fNADC;                // number of ADCs
    Int_t fNCh;                 // number of channels per module
    Int_t fNPed;                // number of pedestal values per channel
    Double_t** fDataSum;        //[fNModule][fNADC] channel sum
    Int_t** fDataRead;          //[fNModule][fNADC] number of channel read events
    UInt_t*** fPed;             //[fNModule][fNCh][fNPed] current pedestal values
    UInt_t*** fPedPos;          //[fNModule][fNCh][fNPed] current pedestal values
    
    static const Int_t fgPedPos;
    static const Int_t fgEvMinStat;
    static const Int_t fgADCBaF2[][4];
    static const Int_t fgADCVeto[][1];
    static const Char_t fgMapBaF2[];
    static const Char_t fgMapVeto[];
    static const Char_t* fgParPedBaF2[];
    static const Char_t* fgParPedVeto[];
 
    void CalculatePed();
    void ClearData();

public:
    TTCalibQAC() : TObject(),
                   fCrateID(-1),
                   fIsVeto(kFALSE), 
                   fNModule(0), fNADC(0), fNCh(0), fNPed(0),
                   fDataSum(0), fDataRead(0), fPed(0), fPedPos(0) { }
    TTCalibQAC(Int_t crateID, Bool_t isVeto, UInt_t pedInit);
    virtual ~TTCalibQAC();

    UInt_t* GetPedestal(Int_t mod, Int_t ch) const { return fPed[mod][ch]; }

    void AddData(Int_t mod, Int_t adc, Int_t val);
    Bool_t UpdateCalib();
    Bool_t InitPedFromDB();
    void SavePedToDB();
    void PrintPedPos();
    void Print();

    ClassDef(TTCalibQAC, 1) // BaF2/Veto board QAC calibration
};

#endif

