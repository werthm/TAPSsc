// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTLeCroy1445                                                         //
//                                                                      //
// RS232 communication class for LeCroy 1445 high voltage controller.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTLeCroy1445.h"

ClassImp(TTLeCroy1445)


//______________________________________________________________________________
TTLeCroy1445::TTLeCroy1445(const Char_t* device)
    : TTRS232(device)
{
    // Constructor.
    
}

//______________________________________________________________________________
void TTLeCroy1445::Configure()
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
        
        // clear struct for new port settings
        bzero(&options, sizeof(options));
    
        // set control mode flags
        // B9600   : baud rate
        // CS8     : 8n1 (8 bit, no parity, 1 stopbit)
        // CLOCAL  : local connection, no modem control
        // CREAD   : enable receiving characters
        options.c_cflag |= B9600;
        options.c_cflag |= CS8;
        options.c_cflag |= CLOCAL;
        options.c_cflag |= CREAD;

        // input mode flags
        // IGNBRK  : ignore break
        // IXON    : enable XON flow control on input
        // IXOFF   : enable XOFF flow control on input
        options.c_iflag |= IGNBRK;
        options.c_iflag |= IXON;
        options.c_iflag |= IXOFF;
    
        // output mode flags
        // ONLCR   : map NL to CR-NL on output
        options.c_oflag |= ONLCR;
        
        // 2 x 0.1 sec inter-character timer
        options.c_cc[VTIME] = 2;

        // clean line
        tcflush(fDesc, TCIFLUSH);

        // set the new options for the port
        tcsetattr(fDesc, TCSANOW, &options);
        
        /*
        // set baud rates
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);

        // enable the receiver and set local mode
        options.c_cflag |= (CLOCAL | CREAD);

        // mask the character size to 8 bits, no parity, 1 stop bit
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |=  CS8;
        
        // disable hardware flow control
        options.c_cflag &= ~CRTSCTS;
        
        // disable XON XOFF (for transmit and receive)
        options.c_iflag &= ~(IXON | IXOFF | IXANY);

        // enable data to be processed as raw input
        //options.c_lflag &= ~(ICANON | ECHO | ISIG);
        
        // clean line
        tcflush(fDesc, TCIFLUSH);

        // set the new options for the port
        tcsetattr(fDesc, TCSANOW, &options);
        */
    }
}

