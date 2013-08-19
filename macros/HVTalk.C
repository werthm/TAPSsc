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
    TTLeCroy1445* hvtalk = new TTLeCroy1445("/dev/ttyS0");
    hvtalk->Configure();

    // user info
    printf("\nWelcome to hvtalk!\n");
    printf("Enter 'quit' to leave\n\n");
    
    // user input loop
    Char_t* input[256];
    while (strcmp(input, "quit"))
    {
        // read command
        printf("> ");
        scanf("%s", input);
    }
    
    // clean-up
    delete hvtalk;

    gSystem->Exit(0);
}

