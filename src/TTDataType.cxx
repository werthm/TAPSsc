// SVN Info: $Id: TTDataType.cxx 1661 2013-08-18 21:32:15Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTDataType                                                           //
//                                                                      //
// Data type class.                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTDataType.h"

ClassImp(TTDataType)


//______________________________________________________________________________
TTDataType::TTDataType(const Char_t* name, const Char_t* title, 
                       TDataType_t type, Int_t nSize)
    : TNamed(name, title)
{
    // Constructor.
    
    fType = type;
    fSize = nSize;
}

//______________________________________________________________________________
void TTDataType::Print()
{
    // Print the content of this class.
    
    Char_t type[16];
    if (fType == kParType) strcpy(type, "Parameter");
    else if (fType == kMapType) strcpy(type, "Map");
    else strcpy(type, "unknown");

    printf("Data Type Information\n");
    printf("Name           : %s\n", GetName());
    printf("Title          : %s\n", GetTitle());
    printf("Type           : %s\n", type);
    printf("Table name     : %s\n", fTableName.Data());
    printf("Number of par. : %d\n", fSize);
}

