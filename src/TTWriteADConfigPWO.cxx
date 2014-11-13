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

    return kTRUE;
}

