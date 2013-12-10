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
    
    // get serial device
    const Char_t* device = gEnv->GetValue("HV.RS232.Device", "null");
    if (!strcmp(device, "null")) 
    {
        Error("HVTalk", "Could not read serial device from configuration file!");
        gSystem->Exit(-1);
    }

    // start communication with HV 
    TTLeCroy1445* lecroy = new TTLeCroy1445(device);
    if (!lecroy->Init())
    {
        Error("HVTalk", "No connection to LeCroy 1445!");
        gSystem->Exit(-1);
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
        printf("HVTALK> ");
        std::cin.getline(input, 256);
        
        // convert characters to uppercase
        for (Int_t i = 0; i < strlen(input); i++)
            input[i] = toupper(input[i]);
        
        // check quit
        if (!strcmp(input, "QUIT")) break;
        if (!strcmp(input, "EXIT")) break;
        
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

