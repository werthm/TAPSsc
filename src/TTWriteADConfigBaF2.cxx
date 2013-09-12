// SVN Info: $Id$

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


#include "TTWriteADConfigBaF2.h"

ClassImp(TTWriteADConfigBaF2)


//______________________________________________________________________________
Bool_t TTWriteADConfigBaF2::ParseLine()
{
    // Parses the line 'fLineIn' from the template file. 

    // parse key line: 'Hardware-ID'
    if (TTUtils::IndexOf(fLineIn, "Hardware-ID:") != -1)
    {
        // get info from data base
        //Double_t tmp = TTMySQLManager::GetManager()->GetHardwareID(fCrate, fModule);

        // prepare output line
        sprintf(fLineOut, "Hardware-ID: %s", "1010");
    }

    // parse key line: 'Thr-CFD'
    else if (TTUtils::IndexOf(fLineIn, "Thr-CFD:") != -1)
    {
        Double_t tmp[fNElements];

        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.BaF2.Thr.CFD", fNElements, fElements, tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.BaF2.Thr.CFD' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "Thr-CFD:");            
            for (Int_t i = 0; i < fNElements; i++) sprintf(fLineOut, "%s %.f", fLineOut, tmp[i]);
        }
    }

    // parse key line: 'Thr-LED1'
    else if (TTUtils::IndexOf(fLineIn, "Thr-LED1:") != -1)
    {
        Double_t tmp[fNElements];

        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.BaF2.Thr.LED1", fNElements, fElements, tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.BaF2.Thr.LED1' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "Thr-LED1:");            
            for (Int_t i = 0; i < fNElements; i++) sprintf(fLineOut, "%s %.f", fLineOut, tmp[i]);
        }
    }

    // parse key line: 'Thr-LED2'
    else if (TTUtils::IndexOf(fLineIn, "Thr-LED2:") != -1)
    {
        Double_t tmp[fNElements];

        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.BaF2.Thr.LED2", fNElements, fElements, tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.BaF2.Thr.LED2' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "Thr-LED2:");            
            for (Int_t i = 0; i < fNElements; i++) sprintf(fLineOut, "%s %.f", fLineOut, tmp[i]);
        }
    }

    // parse default line
    else
    {
        // copy input to output line
        sprintf(fLineOut,"%s", fLineIn);
    }

    return kTRUE;
}

