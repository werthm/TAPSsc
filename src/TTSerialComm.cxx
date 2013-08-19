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


#include "TTSerialComm.h"

ClassImp(TTSerialComm)


//______________________________________________________________________________
TTSerialComm::TTSerialComm(const Char_t* device)
{
    // Constructor.
    
    // init members
    fDesc = -1;

    // try to open the port (read/write, not controlling term. of process, not blocking)
    fDesc = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fDesc == -1)
    {
        Error("TTSerialComm", "Could not open device '%s'!", device);
        return;
    }
}

//______________________________________________________________________________
TTSerialComm::~TTSerialComm()
{
    // Destructor.

    Close();
}

//______________________________________________________________________________
void TTSerialComm::Configure()
{
    // Configure the serial port.
    
    // check if port is open
    if (fDesc != -1)
    {
        // configure port reading
        fcntl(fDesc, F_SETFL, FNDELAY);
        
        // get the current options for the port
        struct termios options;
        tcgetattr(fDesc, &options);
        
        // set baud rates
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);

        // enable the receiver and set local mode
        options.c_cflag |= (CLOCAL | CREAD);

        // mask the character size to 8 bits, no parity
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |=  CS8;
        
        // disable hardware flow control
        options.c_cflag &= ~CRTSCTS;
        
        // disable XON XOFF (for transmit and receive)
        options.c_iflag &= ~(IXON | IXOFF | IXANY);

        // enable data to be processed as raw input
        options.c_lflag &= ~(ICANON | ECHO | ISIG);

        // set the new options for the port
        tcsetattr(fDesc, TCSANOW, &options);
    }
    else
    {
        Error("Configure", "No serial port was opened!");
    }
}

//______________________________________________________________________________
void TTSerialComm::Close()
{
    // Close the serial port.

    if (fDesc != -1) close(fDesc);
}

