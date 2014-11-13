/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfigBaF2                                                  //
//                                                                      //
// Class to write the AcquDAQ cofiguration files for BaF2 modules from  //
// a template file and values from the MySQL database.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TError.h"

#include "TTWriteADConfigBaF2.h"
#include "TTUtils.h"
#include "TTMySQLManager.h"

ClassImp(TTWriteADConfigBaF2)

//______________________________________________________________________________
Bool_t TTWriteADConfigBaF2::ParseLine()
{
    // Parses the line 'fLineIn' from the template file. 

    // parse key line: 'Hardware-ID'
    if (TTUtils::IndexOf(fLineIn, "Hardware-ID:") != -1)
    {
        Int_t ch = 0;
        Int_t elem;
        Double_t tmp;
        
        // get map info from data base
        if (!TTMySQLManager::GetManager()->ReadElements("Map.BaF2.HWID", 1, &ch, fCrate, fModule, &elem))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Map.BaF2.HWID' values.");
            return kFALSE;
        }
  
        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.BaF2.HWID", 1, &elem, &tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.BaF2.HWID' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "Hardware-ID: %04d", (Int_t)tmp);
            return kTRUE;
        }
    }
    
    // keys for channel-wise configuration
    Char_t fval[256];
    const Int_t keyN = 7;
    const Char_t* keySt[keyN] = { "Thr-CFD-Raw:", "Thr-LED1:", "Thr-LED2:",
                                  "Ped-LG:", "Ped-LGS:", "Ped-SG:", "Ped-SGS:" };
    const Char_t* keyID[keyN] = { "Par.BaF2.Thr.CFD", "Par.BaF2.Thr.LED1", "Par.BaF2.Thr.LED2",
                                  "Par.BaF2.QAC.LG", "Par.BaF2.QAC.LGS", "Par.BaF2.QAC.SG", "Par.BaF2.QAC.SGS" };
    
    // loop over keys
    for (Int_t i = 0; i < keyN; i++)
    {
        // check for key
        if (TTUtils::IndexOf(fLineIn, keySt[i]) != -1)
        {
            Double_t tmp[fNElements];

            // get info from data base
            if (!TTMySQLManager::GetManager()->ReadParameters(keyID[i], fNElements, fElements, tmp))
            {
                Error("ParseLine", "MySQLManager reported an error when trying to get '%s' values.", keyID[i]);
                return kFALSE;
            }
            else
            {
                // prepare output line
                sprintf(fLineOut, "%s ", keySt[i]);
                for (Int_t j = 0; j < fNElements; j++) 
                {
                    if (j == fNElements-1) sprintf(fval, "%.f", tmp[j]);
                    else sprintf(fval, "%.f ", tmp[j]);
                    strcat(fLineOut, fval);
                }
                return kTRUE;
            }
        }
    }
    
    // no key to be overwritten found -> copy input to output line
    sprintf(fLineOut,"%s", fLineIn);

    return kTRUE;
}

