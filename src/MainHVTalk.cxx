// SVN Info: $Id: MainHVTalk.cxx 1709 2013-09-12 14:12:57Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MainHVTalk                                                           //
//                                                                      //
// Contains the main method for the HVTalk application.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include <iostream>

#include "TSystem.h"
#include "TEnv.h"

#include "TTLeCroy1445.h"


// global variables
static TTLeCroy1445* gLeCroy = 0;
static Char_t gCfgFile[256] = "";


// enum for command line arguments
enum EHVTalkArgs 
{
    kPrintHelp,
    kOk
};


//______________________________________________________________________________
void PrintHelp()
{
    // Print program usage.

    printf("\n");
    printf("HVTalk\n");
    printf("Usage: HVTalk [-c config.rootrc] [-h]\n");
    printf("\n");
}

//______________________________________________________________________________
Int_t DecodeArgs(Int_t argc, Char_t* argv[])
{
    // Decode command line arguments.
    
    Char_t params[1024];
    Char_t tmp[256];
     
    // concatenate all arguments into one string
    params[0] = '\0';
    for (Int_t i = 1; i < argc; i++)
    {
        strcat(params, argv[i]);
        strcat(params, " ");
    }
    
    // loop over all "-" argument tokens
    Int_t start = 0;
    Int_t end = 0;
    while ((start = TTUtils::IndexOf(params, "-", start)) >= 0)
    {
        // end is either the beginning of the next "-" token or the
        // end of the total argument string
        if ((end = TTUtils::IndexOf(params, "-", start+1)) == -1)
        end = strlen(params);
    
        // copy the token into a separate string, terminate it
        strncpy(tmp, params+start, end-start-1);
        tmp[end-start-1] = '\0';
        
        // parse server configuration
        if (TTUtils::IndexOf(tmp, "-h") == 0) return kPrintHelp;
        else if (TTUtils::IndexOf(tmp, "-c") == 0) 
        {
            strcpy(gCfgFile, tmp+3);
        }

        // set the new start to the current end value
        start = end;
    }

    return kOk;
}

//______________________________________________________________________________
Int_t main(Int_t argc, Char_t* argv[])
{
    // Main method.
    
    Char_t tmp[256];
    
    // decode command line arguments
    switch (DecodeArgs(argc, argv))
    {
        case kPrintHelp:
            PrintHelp();
            return 0;
        case kOk:
            break;
    }
    
    // try to load configuration file either via cmd-line argument or env. variable
    if (strcmp(gCfgFile, "")) strcpy(tmp, gCfgFile);
    else sprintf(tmp, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    if (gEnv->ReadFile(tmp, kEnvLocal))
    {
        Error("main", "Could not find configuration file!");
        Error("main", "Check the command line arguments or the TAPSSC environment variable!");
        return -1;
    }
    
    // get serial device
    const Char_t* device = gEnv->GetValue("HV.RS232.Device", "null");
    if (!strcmp(device, "null")) 
    {
        Error("main", "Could not read serial device from the configuration file!");
        return -1;
    }

    // start communication with HV 
    gLeCroy = new TTLeCroy1445(device);
    if (!gLeCroy->Init())
    {
        Error("main", "No connection to LeCroy 1445!");
        delete gLeCroy;
        return -1;
    }

    // user info
    printf("\nWelcome to hvtalk!\n");
    printf("Enter 'help' for a list of commands\n");
    printf("Enter 'quit' or 'exit' to leave\n\n");
    
    // user input loop
    Char_t input[256];
    while (1)
    {
        // read command
        printf("HVTalk> ");
        std::cin.getline(input, 256);
        
        // convert characters to uppercase
        for (UInt_t i = 0; i < strlen(input); i++)
            input[i] = toupper(input[i]);
        
        // check quit
        if (!strcmp(input, "QUIT")) break;
        if (!strcmp(input, "EXIT")) break;
        
        // check help
        if (!strcmp(input, "HELP")) 
        {
            gLeCroy->PrintCmdHelp();
            continue;
        }

        // send command
        Char_t* res = gLeCroy->SendCmd(input);
        
        // print response
        if (res) 
        {
            fputs(res, stdout);
            printf("\n");
        }
        else printf("no response\n");
    }
 
    // clean-up
    if (gLeCroy) delete gLeCroy;

    return 0;
}

