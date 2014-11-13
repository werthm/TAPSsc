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


#include "TSocket.h"

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

//______________________________________________________________________________
Int_t TTUtils::GetNetworkCmd(const Char_t* cmd)
{
    // Extract and return the principal network command contained in the string
    // 'cmd'.

    Int_t c;
    sscanf(cmd, "%d%*s", &c);
    return c;
}

//______________________________________________________________________________
void TTUtils::SendNetworkCmd(TSocket* s, Int_t cmd)
{
    // Send the network command 'cmd' as string via the socket 's'.
    
    Char_t tmp[32];
    sprintf(tmp, "%d", cmd);
    s->Send(tmp);
}

//______________________________________________________________________________
void TTUtils::SendNetworkCmd(TSocket* s, Int_t cmd, Int_t par)
{
    // Send the network command 'cmd' combined with the parameter 'par' as 
    // string via the socket 's'.
    
    Char_t tmp[32];
    sprintf(tmp, "%d %d", cmd, par);
    s->Send(tmp);
}

//______________________________________________________________________________
void TTUtils::SendNetworkCmd(TSocket* s, Int_t cmd, Int_t par0, Int_t par1)
{
    // Send the network command 'cmd' combined with the parameters 'par0' and 
    // 'par1' as string via the socket 's'.
    
    Char_t tmp[32];
    sprintf(tmp, "%d %d %d", cmd, par0, par1);
    s->Send(tmp);
}

//______________________________________________________________________________
void TTUtils::SendNetworkCmd(TSocket* s, Int_t cmd, const Char_t* msg, Int_t par)
{
    // Send the network command 'cmd' combined with the string 'msg' and the 
    // parameter 'par' as string via the socket 's'.
    
    Char_t tmp[32];
    sprintf(tmp, "%d %s %d", cmd, msg, par);
    s->Send(tmp);
}

