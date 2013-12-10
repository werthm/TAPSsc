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

private:
    Char_t* fDevice;                    // device specifier
    Bool_t fIsConfigured;               // configured flag

protected:
    Int_t fDesc;                        // file descriptor
    Char_t* fBuffer;                    // character buffer
    
    virtual Bool_t Configure() = 0;

public:
    TTRS232() : fDevice(0), fIsConfigured(kFALSE), 
                fDesc(-1), fBuffer(0) { }
    TTRS232(const Char_t* device);
    virtual ~TTRS232();
    
    Bool_t IsConfigured() const { return fIsConfigured; }

    Bool_t Init();
    Char_t* SendCmd(const Char_t* c, Bool_t addCR = kTRUE, Int_t* outNread = 0);
    void Close();

    ClassDef(TTRS232, 0) // Serial port RS232 communication
};

#endif

