// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTNetServer                                                          //
//                                                                      //
// Network server class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVER_H
#define TTSERVER_H

#include "TTNetServer.h"


class TTServer : public TTNetServer
{
    virtual void ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServer() : TTNetServer() { }
    TTServer(Int_t port);
    virtual ~TTServer();

    ClassDef(TTServer, 0) // TAPS server
};

#endif

