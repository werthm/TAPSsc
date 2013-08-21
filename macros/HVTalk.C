// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HVTalk.C                                                             //
//                                                                      //
// ROOT macro imitating hvtalk for controlling LeCroy 1445.             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void HVTalk()
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);
    
    // start communication with HV 
    TTLeCroy1445* lecroy = new TTLeCroy1445("/dev/ttyUSB0");
    lecroy->Configure();

    // user info
    printf("\nWelcome to hvtalk!\n");
    printf("Enter 'help' for a list of commands\n");
    printf("Enter 'quit' to leave\n\n");
    
    // user input loop
    Char_t input[256];
    while (1)
    {
        // read command
        printf("HVTALK> ");
        std::cin.getline(input, 256);
        
        // convert characters to uppercase
        for (Int_t i = 0; i < strlen(input); i++)
            input[i] = toupper(input[i]);
        
        // check quit
        if (!strcmp(input, "QUIT")) break;
        
        // check help
        if (!strcmp(input, "HELP")) 
        {
            lecroy->PrintCmdHelp();
            continue;
        }

        // send command
        Char_t* res = lecroy->SendCmd(input);
        
        // print response
        if (res) 
        {
            fputs(res, stdout);
            printf("\n");
        }
        else printf("no response\n");
    }
    
    // clean-up
    delete lecroy;

    gSystem->Exit(0);
}

