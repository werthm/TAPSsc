// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTUtils                                                              //
//                                                                      //
// TAPSsc utility functions namespace                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTUTILS_H
#define TTUTILS_H

#include "Rtypes.h"


namespace TTUtils
{
    Int_t IndexOf(const Char_t* s1, const Char_t* s2, UInt_t p = 0);
    Int_t LastIndexOf(const Char_t* s, Char_t c);
}

#endif

