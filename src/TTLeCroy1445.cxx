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

//______________________________________________________________________________
void TTLeCroy1445::PrintCmdHelp()
{
    // Print command help to stdout.
    // Command list taken from http://ikpe1101.ikp.kfa-juelich.de/manuals/
    // LeCroy_1440/LeCroy_1445_System_Controller.html
    
    printf("\n");
    printf("LeCroy 1445 command overview (Firmware 1.7)\n");
    printf("\n");
    printf("Main commands:\n");
    printf("M n         Mainframe specifier\n");
    printf("ST          Status Request\n");
    printf("EM          Empty HV Card Slot Request\n");
    printf("N           Non Updated Channel Request\n");
    printf("RL          Report both Current Limits\n");
    printf("VER         Print Version number of Firmware\n");
    printf("R           Read\n");
    printf("W v         Write\n");
    printf("I           Write with Auto Increment of Channel Pointer\n");
    printf("ON          Turn High Voltage Output On\n");
    printf("OF          Turn High Voltage Output Off\n");
    printf("LI +/- n    Current Limit Specifier\n");
    printf("SW          Swap Demand and Backup Program Buffers\n");
    printf("CO          Copy Demand into BAckup Program Buffers\n");
    printf("U           Update (Demand = Demand + Backup - Actual)\n");
    printf("CL          Clear Faults\n");
    printf("\n");
    printf("Pointer Modifiers:\n");
    printf("C n         Channel Specifier, Select Demand Programming Buffer\n");
    printf("B           Backup Programming Buffer. Must precede 'C' if used in the same command.\n");
    printf("P           Specifies Programming buffer, either Demand or Backup\n");
    printf("V           Specifies Actual HV Output Value\n");
    printf("\n");
    printf("Iteration Modifiers:\n");
    printf("A           Modifies command to function on all channels or all mainframes.\n");
    printf("DO n        Specifies a number of successive channels for command to operate on\n");
    printf("\n");
    printf("Format Modifiers:\n");
    printf("F           Selects 8 column output format\n");
    printf("E           Selects output of Demand, Backup and Actual\n");
    printf("\n");
    printf("Control Characters:\n");
    printf("<^C>        Abort Currently executing command\n");
    printf("<^X>        Clear command line\n");
    printf("<^H>        Rubout or Delete\n");
    printf("<^S>        Suspend printout\n");
    printf("<^Q>        Resume printout\n");
    printf("<^Z>        Reboot\n");
    printf(";           Begin or end comment characters\n");
    printf(",           Multiply preceding number by 16 and add next number,\n");
    printf("            i.e. 'C2,3' is equivalent to 'C35'.\n");
    printf("\n");
}

