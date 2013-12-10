/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTDataTypeMap                                                        //
//                                                                      //
// Map data type class.                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTDataTypeMap.h"

ClassImp(TTDataTypeMap)


//______________________________________________________________________________
TTDataTypeMap::TTDataTypeMap(const Char_t* name, const Char_t* title, 
                             Int_t nSize)
    : TTDataType(name, title, kMapType, nSize)
{
    // Constructor.
    
}

