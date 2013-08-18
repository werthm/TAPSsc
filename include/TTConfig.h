// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTConfig                                                             //
//                                                                      //
// TAPSsc configuration namespace                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCONFIG_H
#define TTCONFIG_H

#include "Rtypes.h"


namespace TTConfig
{   
    // database format definitions
    extern const Char_t* kParTableFormat;
    extern const Char_t* kMapTableFormat;
 
    // version numbers etc.
    extern const Char_t kTAPSscVersion[];

    // networking
    extern const Int_t kTAPSServerPort;
}

#endif

