// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HVTalk.C                                                             //
//                                                                      //
// ROOT macro imitating hvtalk.                                         //
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
    TTLeCroy1445* hvtalk = new TTLeCroy1445("/dev/ttyUSB0");
    hvtalk->Configure();

    // user info
    printf("\nWelcome to hvtalk!\n");
    printf("Enter 'quit' to leave\n\n");
    
    // user input loop
    Char_t input[256];
    while (1)
    {
        // read command
        printf("> ");
        std::cin.getline(input, 256);

        // check quit
        if (!strcmp(input, "quit")) break;
        
        // send command
        Char_t* res = hvtalk->SendCmd(input);
        
        // print response
        if (res) 
        {
            fputs(res, stdout);
            printf("\n");
        }
        else printf("no response\n");
    }
    
    // clean-up
    delete hvtalk;

    gSystem->Exit(0);
}

