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
Bool_t TTLeCroy1445::Configure()
{
    // Configure the serial port.
    // Return kTRUE on success, otherwise kFALSE.
    
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
    Int_t ret = tcsetattr(fDesc, TCSANOW, &options);
    
    // return status
    return ret == 0 ? kTRUE : kFALSE;

    /*
    // alternative configuration (requires #include <cstdlib>)
    Char_t systemcall[8192];
    sprintf(systemcall,"stty 9600 time 2 min 0 -parity cs8 ixon ixoff -hupcl cread "
                       "clocal -cstopb -crtscts onlcr ignbrk -brkint -ignpar "
                       "-parmrk -inpck -istrip -inlcr -igncr -iuclc -ixany -imaxbel "
                       "-opost -olcuc -ocrnl -onocr -onlret -ofill -ofdel nl0 cr0 "
                       "tab0 bs0 vt0 ff0 -isig -icanon -iexten -echo -echoe -echok "
                       "-echonl -noflsh -xcase -tostop -echoprt -echoctl -echoke -icrnl </dev/ttyUSB0\n");
    system(systemcall);
    */
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

//______________________________________________________________________________
Int_t TTLeCroy1445::GetHVStatus(Int_t mf)
{
    // Return the status of the high voltage of mainframe 'mf'.
    // Return values are kOn, kOff and kUndef.

    // check if port was configured
    if (IsConfigured())
    {
        // create command
        Char_t cmd[256];
        sprintf(cmd, "M%d ST", mf);
        Char_t* ret = SendCmd(cmd);
        
        // check response
        if (TTUtils::IndexOf(ret, "HV on") != -1) return kOn;
        else if (TTUtils::IndexOf(ret, "HV off") != -1) return kOff;
        else return kUndef;
    }
    else return kOff;
}

//______________________________________________________________________________
Bool_t TTLeCroy1445::ChangeHVStatus(Int_t mf, Bool_t status)
{
    // Change the status of the high voltage of mainframe 'mf' to 'status'.
    // Return kTRUE on success, otherwise kFALSE.
    
    // check if port was configured
    if (IsConfigured())
    { 
        // check current status
        Bool_t cur = IsHVOn(mf);
        
        // check if nothing has to be done
        if (cur == status)
        {
            if (cur) Error("ChangeHVStatus", "HV of mainframe %d is already on!", mf);
            else Error("ChangeHVStatus", "HV of mainframe %d is already off!", mf);
            return kFALSE;
        }
        
        // create command
        Char_t cmd[256];
        if (status) sprintf(cmd, "M%d ON", mf);
        else sprintf(cmd, "M%d OFF", mf);
        SendCmd(cmd);
         
        // check status
        cur = IsHVOn(mf);
        if (cur != status)
        {
            Error("ChangeHVStatus", "Could not change HV status!");
            return kFALSE;
        }
        else return kTRUE;
    }
    else return kFALSE;
}

//______________________________________________________________________________
Bool_t TTLeCroy1445::IsHVOn(Int_t mf)
{
    // Check if the high voltage of mainframe 'mf' is on.
    // Return kTRUE if it is on, otherwise kFALSE.

    // try to read HV status
    while (1)
    {
        Int_t ret = GetHVStatus(mf);
        if (ret == kOn) return kTRUE;
        else if (ret == kOff) return kFALSE;
        usleep(100000);
    }
}

//______________________________________________________________________________
Bool_t TTLeCroy1445::ReadHV(Int_t mf, Int_t c, Int_t* outDem, 
                            Int_t* outBack, Int_t* outAc)
{
    // Read the high voltage values of the channel 'c' (0-255) of mainframe 'm'
    // and save them to 'outDem' (demand), 'outBack' (backup) and 'outAc' (actual).
    // Return kTRUE on success, otherwise kFALSE.

    // check if port was configured
    if (IsConfigured())
    { 
        // create command
        Char_t cmd[256];
        sprintf(cmd, "M%d R E C%d", mf, c);
        Char_t* ret = SendCmd(cmd);
        
        // try to search the beginning of the interesting line
        Int_t begin = TTUtils::IndexOf(ret, "\nC ");
        if (begin == -1) return kFALSE;
        begin++;

        // try to search the ending of the interesting line
        Int_t end = TTUtils::IndexOf(ret, "\r", begin);
        if (end == -1) return kFALSE;
        
        // extract the interesting line
        Char_t line[256];
        strncpy(line, ret+begin, end-begin);
        line[end-begin] = '\0';
        
        // try to parse the line
        Int_t ch, de, ba, ac;
        Int_t nscan = sscanf(line, "C%d -%d -%d -%d", &ch, &de, &ba, &ac);
        if (nscan != 4) return kFALSE;
        
        // check channel number
        if (ch != c) return kFALSE;
        
        // save values
        *outDem = -de;
        if (outBack) *outBack = -ba;
        if (outAc) *outAc = -ac;

        return kTRUE;
    }
    else return kFALSE;
}


//______________________________________________________________________________
Bool_t TTLeCroy1445::TurnHVOn(Int_t mf)
{
    // Turn the high voltage of mainframe 'mf' on.
    // Return kTRUE on success, otherwise kFALSE.
    
    return ChangeHVStatus(mf, kTRUE);
}

//______________________________________________________________________________
Bool_t TTLeCroy1445::TurnHVOff(Int_t mf)
{
    // Turn the high voltage of mainframe 'mf' off.
    // Return kTRUE on success, otherwise kFALSE.
    
    return ChangeHVStatus(mf, kFALSE);
}

