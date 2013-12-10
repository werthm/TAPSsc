// SVN Info: $Id: TTClientPWO.cxx 1715 2013-09-13 20:38:21Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientPWO                                                          //
//                                                                      //
// PWO TAPS client class.                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClientPWO.h"

ClassImp(TTClientPWO)


//______________________________________________________________________________
TTClientPWO::TTClientPWO(const Char_t* host, Int_t port)
    : TTClient(host, port)
{
    // Constructor.
    
}

