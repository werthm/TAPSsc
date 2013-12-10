// SVN Info: $Id: TTDataTypePar.cxx 1708 2013-09-11 18:07:46Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTDataTypePar                                                        //
//                                                                      //
// Parameter data type class.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTDataTypePar.h"

ClassImp(TTDataTypePar)


//______________________________________________________________________________
TTDataTypePar::TTDataTypePar(const Char_t* name, const Char_t* title, 
                             Int_t nSize)
    : TTDataType(name, title, kParType, nSize)
{
    // Constructor.
    
    fMap = 0;
}

//______________________________________________________________________________
void TTDataTypePar::Print()
{
    // Print the content of this class.
    
    TTDataType::Print();

    printf("Minimum value  : %f\n", fMin);
    printf("Maximum value  : %f\n", fMax);
    if (fMap) printf("Map            : %s\n", fMap->GetName());
    else printf("Map            : none\n");
}

