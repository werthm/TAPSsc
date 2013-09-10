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


#include "TTUtils.h"


//______________________________________________________________________________
Int_t TTUtils::IndexOf(const Char_t* s1, const Char_t* s2, UInt_t p)
{
    // Returns the position of the first occurrence of the string s2
    // in the string s1 after position p. Returns -1 if s2 was not found.

    const Char_t* pos = strstr(s1+p, s2);
    if (pos) return pos-s1;
    else return -1;
}
 
//______________________________________________________________________________
Int_t TTUtils::LastIndexOf(const Char_t* s, Char_t c)
{
    // Returns the position of the last occurrence of the character c
    // in the string s. Returns -1 if c was not found.
 
    const Char_t* pos = strrchr(s, (Int_t)c);
    if (pos) return pos-s;
    else return -1;
}

