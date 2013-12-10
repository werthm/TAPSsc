// SVN Info: $Id: TTWriteADConfigPWO.cxx 1709 2013-09-12 14:12:57Z werthm $

/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfigPWO                                                   //
//                                                                      //
// Class to write the AcquDAQ cofiguration files for PWO modules from   //
// a template file and values from the MySQL database.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTWriteADConfigPWO.h"

ClassImp(TTWriteADConfigPWO)


//______________________________________________________________________________
Bool_t TTWriteADConfigPWO::ParseLine()
{
    // Parses the line 'fLineIn' from the template file. 

    // parse key line: 'xxx'
    if (TTUtils::IndexOf(fLineIn, "xxx") != -1)
    {
        Double_t tmp[fNElements];

        // get info from data base
        if (!TTMySQLManager::GetManager()->ReadParameters("xxx", fNElements, fElements, tmp))
        {
            Error("ParseLine", "MySQLManager reported an error when trying to get 'xxx' values.");
            return kFALSE;
        }
        else
        {
            // prepare output line
            sprintf(fLineOut, "xxx:");            
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

