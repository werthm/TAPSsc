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


#include "TTRS232.h"

ClassImp(TTRS232)


//______________________________________________________________________________
TTRS232::TTRS232(const Char_t* device)
{
    // Constructor.
    
    // init members
    fDesc = -1;

    // try to open the port (read/write, not controlling term. of process, not blocking)
    fDesc = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fDesc == -1)
    {
        Error("TTRS232", "Could not open device '%s'!", device);
        return;
    }
}

//______________________________________________________________________________
TTRS232::~TTRS232()
{
    // Destructor.

    Close();
}

//______________________________________________________________________________
void TTRS232::Close()
{
    // Close the serial port.

    if (fDesc != -1) close(fDesc);
}

