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


#include <cstdlib>

#include "TObjString.h"
#include "TObjArray.h"
#include "THashList.h"

#include "TTCalibQAC.h"
#include "TTMySQLManager.h"
#include "TTDataTypePar.h"

ClassImp(TTCalibQAC)

// init static class members
const Int_t TTCalibQAC::fgPedPos = 100;
const Int_t TTCalibQAC::fgEvMinStat = 1500;
const Int_t TTCalibQAC::fgADCBaF2[][4] = { { 10,  8, 11,  9 }, { 14, 12, 15, 13 },
                                           { 22, 20, 23, 21 }, { 26, 24, 27, 25 } };
const Int_t TTCalibQAC::fgADCVeto[][1] = { { 20 }, { 21 }, { 22 }, { 23 }, 
                                           { 24 }, { 25 }, { 26 }, { 27 } };
const Char_t TTCalibQAC::fgMapBaF2[] = "Map.BaF2";
const Char_t TTCalibQAC::fgMapVeto[] = "Map.Veto";
const Char_t* TTCalibQAC::fgParPedBaF2[] = { "Par.BaF2.QAC.LG", "Par.BaF2.QAC.LGS", 
                                              "Par.BaF2.QAC.SG", "Par.BaF2.QAC.SGS" };
const Char_t* TTCalibQAC::fgParPedVeto[] = { "Par.Veto.QAC" };

//______________________________________________________________________________
TTCalibQAC::TTCalibQAC(Int_t crateID, Bool_t isVeto, UInt_t pedInit)
{
    // Constructor.
    
    // init members
    fCrateID = crateID;
    fIsVeto = isVeto;
    fNModule = 16;
    fNADC = 32;
    fNCh = fIsVeto ? 8 : 4;
    fNPed = fIsVeto ? 1 : 4;
    fDataSum = new Double_t*[fNModule];
    fDataRead = new Int_t*[fNModule];
    fPed = new UInt_t**[fNModule];
    fPedPos = new UInt_t**[fNModule];
    for (Int_t i = 0; i < fNModule; i++)
    {
        fDataSum[i] = new Double_t[fNADC];
        fDataRead[i] = new Int_t[fNADC];
        fPed[i] = new UInt_t*[fNCh];
        fPedPos[i] = new UInt_t*[fNCh];
        for (Int_t j = 0; j < fNCh; j++)
        {
            fPed[i][j] = new UInt_t[fNPed];
            fPedPos[i][j] = new UInt_t[fNPed];
            for (Int_t k = 0; k < fNPed; k++) 
            {   
                fPed[i][j][k] = pedInit;
                fPedPos[i][j][k] = 0;
            }
        }
    }
    
    // clear data
    ClearData();
}

//______________________________________________________________________________
TTCalibQAC::~TTCalibQAC()
{
    // Destructor.
    
    if (fDataSum)
    {
        for (Int_t i = 0; i < fNModule; i++)
        {
            delete [] fDataSum[i];
            delete [] fDataRead[i];
            for (Int_t j = 0; j < fNCh; j++) 
            {
                delete [] fPed[i][j];
                delete [] fPedPos[i][j];
            }
            delete [] fPed[i];
            delete [] fPedPos[i];
        }
        delete [] fDataSum;
        delete [] fDataRead;
        delete [] fPed;
        delete [] fPedPos;
    }
}

//______________________________________________________________________________
void TTCalibQAC::AddData(Int_t mod, Int_t adc, Int_t val)
{
    // Add the data 'val' from ADC 'adc' of module 'mod'.

    fDataSum[mod][adc] += val;
    fDataRead[mod][adc]++;
}

//______________________________________________________________________________
void TTCalibQAC::CalculatePed()
{
    // Calculate a new set of pedestal values based on the current values and
    // the added data.
    
    // loop over modules
    for (Int_t i = 0; i < fNModule; i++)
    {
        // loop over channels
        for (Int_t j = 0; j < fNCh; j++)
        {
            // loop over pedestal values
            for (Int_t k = 0; k < fNPed; k++)
            {
                // get the ADC number
                Int_t adc = fIsVeto ? fgADCVeto[j][k] : fgADCBaF2[j][k];

                // calculate the mean data channel
                Double_t pos = fDataRead[i][adc] ? fDataSum[i][adc]/(Double_t)fDataRead[i][adc] : 0;
                
                // calculate the new pedestal value
                if ((Int_t)pos != fgPedPos) 
                {
                    if (!pos) fPed[i][j][k] -= 50;
                    else if (pos > 100) fPed[i][j][k] += 1;
                    else fPed[i][j][k] -= 1;
                }

                // save current pedestal position
                fPedPos[i][j][k] = pos;
            }
        }
    }
}

//______________________________________________________________________________
void TTCalibQAC::ClearData()
{
    // Clear the recorded data.

    for (Int_t i = 0; i < fNModule; i++)
    {
        for (Int_t j = 0; j < fNADC; j++)
        {
            fDataSum[i][j] = 0.;
            fDataRead[i][j] = 0;
        }
    }
}

//______________________________________________________________________________
Bool_t TTCalibQAC::InitPedFromDB()
{
    // Init the pedestal values using the database.
    // Return kFALSE if an error occurred, otherwise kTRUE.

    // map type
    const Char_t* map = fIsVeto ? fgMapVeto : fgMapBaF2;

    // channel and element arrays
    Int_t elem[fNCh];
    Int_t ch[fNCh];

    // init channel array
    for (Int_t i = 0; i < fNCh; i++) ch[i] = i;

    // loop over modules
    for (Int_t i = 0; i < fNModule; i++)
    {
        // get elements in the module
        if (!TTMySQLManager::GetManager()->ReadElements(map, fNCh, ch, fCrateID, i, elem))
        {
            Error("InitPedFromDB", "Could not read elements from the database!");
            return kFALSE;
        }
        
        // get pedestal values
        for (Int_t j = 0; j < fNPed; j++)
        {
            Double_t val[fNCh];

            // get parameter key
            const Char_t* key = fIsVeto ? fgParPedVeto[j] : fgParPedBaF2[j];

            // read from database
            if (!TTMySQLManager::GetManager()->ReadParameters(key, fNCh, elem, val))
            {
                Error("InitPedFromDB", "Could not read the '%s' parameters from the database!", key);
                return kFALSE;
            }
            
            // set pedestal values for all channels
            for (Int_t k = 0; k < fNCh; k++) fPed[i][k][j] = val[k];
        }
    }

    return kTRUE;
}

//______________________________________________________________________________
void TTCalibQAC::SavePedToDB()
{
    // Save the saved pedestal positions to the database.
    
    // flat list of elements and pedestal values
    const Int_t nElem = fNModule*fNCh;
    Int_t elem[nElem];
    Double_t newPed[fNPed][nElem];

    // map type
    const Char_t* map = fIsVeto ? fgMapVeto : fgMapBaF2;
    
    // init channel array
    Int_t ch[fNCh];
    for (Int_t i = 0; i < fNCh; i++) ch[i] = i;
 
    // fill flat lists with element indices and pedestal values
    // loop over modules
    for (Int_t i = 0; i < fNModule; i++)
    {
        // get elements in the module
        if (!TTMySQLManager::GetManager()->ReadElements(map, fNCh, ch, fCrateID, i, elem+i*fNCh))
        {
            Error("SavePedToDB", "Could not read elements from the database!");
            return;
        }
         
        // loop over channels
        for (Int_t j = 0; j < fNCh; j++)
        {
            // loop over pedestal types
            for (Int_t k = 0; k < fNPed; k++)
            {
                // copy pedestal values to the flat list
                newPed[k][i*fNCh+j] = fPed[i][j][k];
            }
        }
    }

    // read old values from database for comparison
    Double_t oldPed[fNPed][nElem];
    for (Int_t i = 0; i < fNPed; i++)
    {
        // get parameter key
        const Char_t* key = fIsVeto ? fgParPedVeto[i] : fgParPedBaF2[i];

        // read from database
        if (!TTMySQLManager::GetManager()->ReadParameters(key, nElem, elem, oldPed[i]))
        {
            Error("SavePedToDB", "Could not read the '%s' parameters from the database!", key);
            return;
        }
    }
        
    // print old/new values and relative differences and check limits
    // loop over modules
    for (Int_t i = 0; i < nElem; i++)
    {
        printf("%03d:  ", elem[i]);
        for (Int_t j = 0; j < fNPed; j++)
        {
            // get parameter key
            const Char_t* key = fIsVeto ? fgParPedVeto[j] : fgParPedBaF2[j];
            
            // get the data type
            TTDataTypePar* d = (TTDataTypePar*) TTMySQLManager::GetManager()->GetDataTypes()->FindObject(key);
            
            // check and correct limits
            Bool_t corrected = kFALSE;
            if (newPed[j][i] < d->GetMin()) 
            {
                newPed[j][i] = d->GetMin();
                corrected = kTRUE;
            }
            if (newPed[j][i] > d->GetMax()) 
            {
                newPed[j][i] = d->GetMax();
                corrected = kTRUE;
            }

            printf("old: %4.0f   new: %4.0f   (%5.1f%%)  ", oldPed[j][i], newPed[j][i], 
                   100.*(newPed[j][i]-oldPed[j][i])/oldPed[j][i]);
            if (corrected) printf("[corrected to limits]  ");
        }
        printf("\n");
    }

    //
    // ask for user action
    //

    Char_t answer[256];
    printf("\nWould you like to write the new values to the database? (no/all/list of modules sep. by ',') : ");
    scanf("%s", answer);
    if (!strcmp(answer, "no"))
    {
        //
        // no database writing
        //

        printf("Aborted.\n");
        return;
    }
    else if (!strcmp(answer, "all"))
    {
        //
        // write values of all modules to the database
        //

        // loop over pedestal types
        for (Int_t i = 0; i < fNPed; i++)
        {
            // get parameter key
            const Char_t* key = fIsVeto ? fgParPedVeto[i] : fgParPedBaF2[i];

            // write to database
            if (!TTMySQLManager::GetManager()->WriteParameters(key, nElem, elem, newPed[i]))
            {
                Error("SavePedToDB", "Could not write the '%s' parameters to the database!", key);
                return;
            }
        }
        return;
    }
    else
    {
        //
        // try to create list of modules whose values should be written to the database
        //

        // tokenize string
        TString st(answer);
        TObjArray* list = st.Tokenize(",");
        Int_t nMod = list->GetEntries();

        // check number of modules
        if (nMod > fNModule)
        {
            Error("SavePedToDB", "Number of provided modules (%d) larger than number of modules in crate (%d)!",
                  nMod, fNModule);
            return;
        }

        // user info
        Info("SavePedToDB", "Saving values for %d modules", nMod);

        // module list
        Int_t mod[nMod];

        // loop over token
        for (Int_t i = 0; i < nMod; i++)
        {
            // get string
            TObjString* s = (TObjString*) list->At(i);

            // convert to double and save to edge array
            mod[i] = atoi(s->GetString().Data());
        }

        // clean-up
        delete list;

        // write values of selected modules
        for (Int_t i = 0; i < nMod; i++)
        {
            Info("SavePedToDB", "Saving values for board %d", mod[i]);

            // loop over pedestal types
            for (Int_t j = 0; j < fNPed; j++)
            {
                // get parameter key
                const Char_t* key = fIsVeto ? fgParPedVeto[j] : fgParPedBaF2[j];

                for (Int_t k = 0; k < fNCh; k++)
                {
                    printf("elem: %d   value: %4.0f\n", *(elem+mod[i]*fNCh+k), *(newPed[j]+mod[i]*fNCh+k));
                }

                // write to database
                if (!TTMySQLManager::GetManager()->WriteParameters(key, fNCh, elem+mod[i]*fNCh, newPed[j]+mod[i]*fNCh))
                {
                    Error("SavePedToDB", "Could not write the '%s' parameters to the database!", key);
                    return;
                }
            }

        }
        return;
    }
}

//______________________________________________________________________________
Bool_t TTCalibQAC::UpdateCalib()
{
    // Update the pedestal calibration if sufficient data has been added.
    // Return kTRUE if new pedestal values were calculated, otherwise kFALSE.

    // sum-up number of read events in all ADCs (skip zero channels)
    Double_t sum = 0;
    Int_t nR = 0;
    for (Int_t i = 0; i < fNModule; i++)
    {
        for (Int_t j = 0; j < fNADC; j++)
        {
            if (fDataRead[i][j]) 
            {
                sum += fDataRead[i][j];
                nR++;
            }
        }
    }

    // warning when all channels are zero
    if (!nR)
    {
        Warning("UpdateCalib", "Zero events in all channels!");
        return kFALSE;
    }
    
    // calculate mean event number
    Double_t meanEn = sum / (Double_t)nR;

    // exit if acquired statistics is not sufficient
    if (meanEn < fgEvMinStat) return kFALSE;

    // calculate new pedestal values
    CalculatePed();

    // clear data
    ClearData();

    return kTRUE;
}

//______________________________________________________________________________
void TTCalibQAC::PrintPedPos()
{
    // Print the current pedestal position to stdout.

    Int_t fg;

    // loop over modules
    for (Int_t i = 0; i < fNModule; i++)
    {
        printf("M%02d ", i);
        
        // loop over channels
        for (Int_t j = 0; j < fNCh; j++)
        {
            printf(" | ");
            
            // loop over pedestal values
            for (Int_t k = 0; k < fNPed; k++)
            {
                if (fPedPos[i][j][k] == 100) fg = 32;
                else fg = 31;
                printf("%c[%d;%dm%4d%c[%dm", 0x1b, 0, fg, fPedPos[i][j][k], 0x1b, 0);
            }
        }

        printf("\n");
    }
    
    printf("\n");
}

//______________________________________________________________________________
void TTCalibQAC::Print(Option_t* option) const
{
    // Print out the content of this class.

    printf("TTCalibQAC content:\n");
    printf("Crate ID             : %d\n", fCrateID);
    printf("IsVeto               : ");
    fIsVeto ? printf("yes\n") : printf("no\n");
    printf("# of modules         : %d\n", fNModule);
    printf("# of ADCs            : %d\n", fNADC);
    printf("# of channels        : %d\n", fNCh);
    printf("# of pedest. per ch. : %d\n", fNPed);
    printf("\n");
    printf("Channel sums[reads]:\n");
    for (Int_t i = 0; i < fNModule; i++)
    {
        printf("M%02d ", i);
        for (Int_t j = 0; j < fNADC; j++)
        {
            printf("%.f[%d] ", fDataSum[i][j], fDataRead[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Pedestal positions:\n");
    for (Int_t i = 0; i < fNModule; i++)
    {
        printf("M%02d ", i);
        for (Int_t j = 0; j < fNCh; j++)
        {
            printf(" | ");
            for (Int_t k = 0; k < fNPed; k++)
            {
                printf("%4d ", fPedPos[i][j][k]);
            }
        }
        printf("\n");
    }
    printf("\n");
    printf("Pedestal values:\n");
    for (Int_t i = 0; i < fNModule; i++)
    {
        printf("M%02d ", i);
        for (Int_t j = 0; j < fNCh; j++)
        {
            printf(" | ");
            for (Int_t k = 0; k < fNPed; k++)
            {
                printf("%4d ", fPed[i][j][k]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

//______________________________________________________________________________
void TTCalibQAC::Streamer(TBuffer& R__b)
{
    // Stream a TTCalibQAC object.

    UInt_t R__s, R__c;

    if (R__b.IsReading())
    {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
        if (R__v) { }

        // call streamer of parent class
        TObject::Streamer(R__b);

        // copy data members
        R__b >> fCrateID;
        R__b >> fIsVeto;
        R__b >> fNModule;
        R__b >> fNADC;
        R__b >> fNCh;
        R__b >> fNPed;
        
        // create the arrays
        fDataSum = new Double_t*[fNModule];
        fDataRead = new Int_t*[fNModule];
        fPed = new UInt_t**[fNModule];
        fPedPos = new UInt_t**[fNModule];
        for (Int_t i = 0; i < fNModule; i++)
        {
            fDataSum[i] = new Double_t[fNADC];
            fDataRead[i] = new Int_t[fNADC];
            fPed[i] = new UInt_t*[fNCh];
            fPedPos[i] = new UInt_t*[fNCh];
            for (Int_t j = 0; j < fNCh; j++)
            {
                fPed[i][j] = new UInt_t[fNPed];
                fPedPos[i][j] = new UInt_t[fNPed];
            }
        }
        
        // copy the array content
        for (Int_t i = 0; i < fNModule; i++) 
        {
            R__b.ReadFastArray(fDataSum[i], fNADC);
            R__b.ReadFastArray(fDataRead[i], fNADC);
            for (Int_t j = 0; j < fNCh; j++)
            {
                R__b.ReadFastArray(fPed[i][j], fNPed);
                R__b.ReadFastArray(fPedPos[i][j], fNPed);
            }
        }

        // check byte count
        R__b.CheckByteCount(R__s, R__c, TTCalibQAC::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(TTCalibQAC::IsA(), kTRUE);

        // call streamer of parent class
        TObject::Streamer(R__b);

        // copy data members
        R__b << fCrateID;
        R__b << fIsVeto;
        R__b << fNModule;
        R__b << fNADC;
        R__b << fNCh;
        R__b << fNPed;

        // copy the arrays
        for (Int_t i = 0; i < fNModule; i++) 
        {
            R__b.WriteFastArray(fDataSum[i], fNADC);
            R__b.WriteFastArray(fDataRead[i], fNADC);
            for (Int_t j = 0; j < fNCh; j++)
            {
                R__b.WriteFastArray(fPed[i][j], fNPed);
                R__b.WriteFastArray(fPedPos[i][j], fNPed);
            }
        }

        // set byte count
        R__b.SetByteCount(R__c, kTRUE);
    }
}

