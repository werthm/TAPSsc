// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTSerialComm                                                         //
//                                                                      //
// Serial port communication class.                                     //
// Source: http://slackware.mirror.ac.za/slackware-4.0/docs/mini/       //
//         Serial-Port-Programming                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERIALCOMM_H
#define TTSERIALCOMM_H

#include <fcntl.h>
#include <termios.h>

#include "TError.h"


class TTSerialComm
{

private:
    Int_t fDesc;                        // file descriptor

public:
    TTSerialComm() : fDesc(-1) { }
    TTSerialComm(const Char_t* device);
    virtual ~TTSerialComm();
    
    void Configure();
    void Close();

    ClassDef(TTSerialComm, 0) // Serial port communication
};

#endif

