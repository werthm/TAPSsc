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
#include "TSocket.h"


namespace TTUtils
{
    Int_t IndexOf(const Char_t* s1, const Char_t* s2, UInt_t p = 0);
    Int_t LastIndexOf(const Char_t* s, Char_t c);

    Int_t GetNetworkCmd(const Char_t* cmd);
    void SendNetworkCmd(TSocket* s, Int_t cmd);
    void SendNetworkCmd(TSocket* s, Int_t cmd, Int_t par);
    void SendNetworkCmd(TSocket* s, Int_t cmd, Int_t par0, Int_t par1);
    void SendNetworkCmd(TSocket* s, Int_t cmd, const Char_t* msg, Int_t par);
}

#endif

