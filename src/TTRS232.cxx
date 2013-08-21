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
    fBuffer = new Char_t[32768];

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
    if (fBuffer) delete [] fBuffer;
}

//______________________________________________________________________________
void TTRS232::Close()
{
    // Close the serial port.

    // check if port is open
    if (fDesc != -1) close(fDesc);
}

//______________________________________________________________________________
Char_t* TTRS232::SendCmd(const Char_t* c, Bool_t addCR)
{
    // Send the command 'c' to the serial port and return the answer.
    // If 'addCR' is kTRUE, a carriage return is added to the command.
    
    // check if port is open
    if (fDesc != -1)
    {
        // terminate command with carriage return if necessary
        // and send the command
        Int_t res;
        if (addCR)
        {
            Int_t len = strlen(c);
            Char_t tmp[len+1];
            strncpy(tmp, c, len);
            tmp[len] = '\r';
            res = write(fDesc, tmp, len+1);
        }
        else
        {
            res = write(fDesc, c, strlen(c));
        }

        // wait some time
        usleep(100000);

        // check write result
        if (res == -1) 
        {
            Error("SendCmd", "Write error - %s!", strerror(errno));
            return 0;
        }
        
        // prepare variables for POSIX select()
        fd_set rfds;
        struct timeval tv;
        int retval;
        Char_t tmp2[32768];
        Int_t nRead = 0;

        // clear buffer
        memset(fBuffer, 0, 32768);
        
        // loop until no more input arrives within timeout
        do
        {
            // watch file descriptor to see when it has input
            FD_ZERO(&rfds);
            FD_SET(fDesc, &rfds);
            
            // wait for input
            tv.tv_sec = 0;
            tv.tv_usec = 500000;
            retval = select(fDesc+1, &rfds, NULL, NULL, &tv);
            
            // read if input has arrived
            if (retval)
            {
                // read into buffer
                res = read(fDesc, tmp2, 32768);
            
                // check read result
                if (res != -1)
                {
                    // terminate buffer
                    tmp2[res] = '\0';
                    strcat(fBuffer, tmp2);
                    nRead += res;
                }
                else
                {
                    Error("SendCmd", "Read error - %s!", strerror(errno));
                    return 0;
                }
            }
        } while (retval);
        
        return fBuffer;
    }
    else return 0;
}

