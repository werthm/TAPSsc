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

    // parse key line: 'Thr-LED'
    if (TTUtils::IndexOf(fLineIn, "Thr-LED:") != -1)
    {
        Double_t tmp[fNElements];

        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.Veto.Thr.LED", fNElements, fElements, tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.Veto.Thr.LED' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "Thr-LED:");            
            for (Int_t i = 0; i < fNElements; i++) sprintf(fLineOut, "%s %.f", fLineOut, tmp[i]);
        }
    }

    // parse key line: 'Thr-LED1'
    else if (TTUtils::IndexOf(fLineIn, "QAC:") != -1)
    {
        Double_t tmp[fNElements];

        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("Par.Veto.QAC", fNElements, fElements, tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'Par.Veto.QAC' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "QAC:");            
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

