// SVN Info: $Id$

/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfigVeto                                                  //
//                                                                      //
// Class to write the AcquDAQ cofiguration files for Veto modules from  //
// a template file and values from the MySQL database.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTWriteADConfigVeto.h"

ClassImp(TTWriteADConfigVeto)


//______________________________________________________________________________
Bool_t TTWriteADConfigVeto::ParseLine()
{
    // Parses the line 'fLineIn' from the template file. 

    // parse key line: 'Hardware-ID'
    if (TTUtils::IndexOf(fLineIn, "Hardware-ID:") != -1)
    {
        Int_t elem;
        Double_t tmp;
        
        // get map info from data base
        if (TTMySQLManager::GetManager()->GetElements("Map.Veto.HWID", fCrate, fModule, &elem) != 1)
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Map.Veto.HWID' values.");
            return kFALSE;
        }
  
        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.Veto.HWID", 1, &elem, &tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.Veto.HWID' values.");
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
    const Int_t keyN = 2;
    const Char_t* keySt[keyN] = { "Thr-VLED:", "Ped-VG:" };
    const Char_t* keyID[keyN] = { "Par.Veto.Thr.LED", "Par.Veto.QAC" };
    
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
                sprintf(fLineOut, keySt[i]);            
                for (Int_t j = 0; j < fNElements; j++) sprintf(fLineOut, "%s %.f", fLineOut, tmp[j]);
                return kTRUE;
            }
        }
    }
    
    // no key to be overwritten found -> copy input to output line
    sprintf(fLineOut,"%s", fLineIn);

    return kTRUE;
}

