// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTRS232                                                              //
//                                                                      //
// Serial port RS232 communication class.                               //
// Source: http://slackware.mirror.ac.za/slackware-4.0/docs/mini/       //
//         Serial-Port-Programming                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTRS232_H
#define TTRS232_H

#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#include "TError.h"


class TTRS232
{

protected:
    Int_t fDesc;                        // file descriptor
    Char_t* fBuffer;                    // character buffer

public:
    TTRS232() : fDesc(-1), fBuffer(0) { }
    TTRS232(const Char_t* device);
    virtual ~TTRS232();
    
    virtual void Configure() = 0;
    Char_t* SendCmd(const Char_t* c, Bool_t addCR = kTRUE);
    void Close();

    ClassDef(TTRS232, 0) // Serial port RS232 communication
};

#endif

