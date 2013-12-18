/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// ChannelManager.C                                                     //
//                                                                      //
// Manage the TAPS channels.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


Char_t gTmp[256] = "";

// BaF2 configuration
const Int_t gNBaF2       = 384;
const Int_t gNCratesBaF2 = 6;
const Int_t gNModBaF2    = 16;
const Int_t gNChBaF2     = 4;
const Int_t gCratesBaF2[gNCratesBaF2] = { 1, 2, 3, 4, 5, 6 };
const Char_t gMapBaF2[] = "Map.BaF2";

// Veto configuration
const Int_t gNVeto       = 384;
const Int_t gNCratesVeto = 3;
const Int_t gNModVeto    = 16;
const Int_t gNChVeto     = 8;
const Int_t gCratesVeto[gNCratesVeto] = { 7, 8, 9 };
const Char_t gMapVeto[] = "Map.Veto";


//______________________________________________________________________________
void PrintHelp()
{
    // Print help about commands.

    printf("\n");
    printf("Available commands:\n");
    printf("quit/exit              quit the program\n");
    printf("help                   print this help\n");
    printf("print                  print the current configuration\n");
    printf("swap                   swap two channels\n");
    printf("check                  perform a consistency check of the mapping\n");
    printf("\n");
}

//______________________________________________________________________________
Char_t FormatBaF2Elem(Int_t c, Int_t m, Int_t ch, Int_t elem)
{
    // Format the BaF2 element 'elem' using the crate 'c', the module 'm' and 
    // the channel 'ch'.
    
    // foreground color
    Int_t fg = 32;
    
    // check if the element number is standard
    Int_t e_norm = 64*c + gNChBaF2*m + ch;
    if (e_norm != elem) fg = 31;
    
    // format element
    sprintf(gTmp, "%c[%d;%dm%3d%c[%dm", 0x1b, 0, fg, elem, 0x1b, 0);
    
    return gTmp;
}

//______________________________________________________________________________
Char_t FormatVetoElem(Int_t c, Int_t m, Int_t ch, Int_t elem)
{
    // Format the Veto element 'elem' using the crate 'c', the module 'm' and 
    // the channel 'ch'.
    
    // foreground color
    Int_t fg = 32;
    
    // check if the element number is standard
    Int_t e_norm = 128*c + gNChVeto*m + ch;
    if (e_norm != elem) fg = 31;
    
    // format element
    sprintf(gTmp, "%c[%d;%dm%3d%c[%dm", 0x1b, 0, fg, elem, 0x1b, 0);
    
    return gTmp;
}

//______________________________________________________________________________
void Print()
{
    // Print the current channel configuration.
    
    Int_t elem[gNModVeto][gNChVeto];
    Int_t ch[gNChVeto] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    
    printf("\n");
    printf("Current configuration\n\n");
 
    //
    // print BaF2 config
    //
    
    // loop over crates
    for (Int_t i = 0; i < gNCratesBaF2; i++)
    {
        printf("BaF2 Crate %d\n", i);
        
        // read all channels of all modules
        for (Int_t j = 0; j < gNModBaF2; j++)
        {
            // get elements in module
            if (!TTMySQLManager::GetManager()->ReadElements(gMapBaF2, gNChBaF2, ch, gCratesBaF2[i], j, elem[j]))
            {
                printf("Cannot read from database - aborting!\n");
                printf("\n");
                return;
            }
        }

        // loop over channels
        for (Int_t j = 0; j < gNChBaF2; j++)
        {
            // loop over modules
            for (Int_t k = 0; k < gNModBaF2; k++)
            {
                printf("| %s ", FormatBaF2Elem(i, k, j, elem[k][j]));
            }
            printf("|\n");
        }
        
        printf("\n");
    }

    //
    // print Veto config
    //
    
    // loop over crates
    for (Int_t i = 0; i < gNCratesVeto; i++)
    {
        printf("Veto Crate %d\n", i);
        
        // read all channels of all modules
        for (Int_t j = 0; j < gNModVeto; j++)
        {
            // get elements in module
            if (!TTMySQLManager::GetManager()->ReadElements(gMapVeto, gNChVeto, ch, gCratesVeto[i], j, elem[j]))
            {
                printf("Cannot read from database - aborting!\n");
                printf("\n");
                return;
            }
        }

        // loop over channels
        for (Int_t j = 0; j < gNChVeto; j++)
        {
            // loop over modules
            for (Int_t k = 0; k < gNModVeto; k++)
            {
                printf("| %s ", FormatVetoElem(i, k, j, elem[k][j]));
            }
            printf("|\n");
        }
        
        printf("\n");
    }

    printf("\n");
}

//______________________________________________________________________________
void Swap()
{
    // Swap two channels.
    
    Bool_t isVeto;
    Int_t elem[2];
    Int_t crate[2];
    Int_t mod[2];
    Int_t ch[2];

    printf("\n");
    printf("Hardware channel swapping\n");
    
    // ask for BaF2/Veto
    Char_t input[256];
    printf("What channels would you like to swap (BaF2/Veto) : ");
    std::cin.getline(input, 256);
    if (!strcmp(input, "BaF2")) 
    {
        printf("Swapping BaF2 channels\n");
        isVeto = kFALSE;
    }
    else if (!strcmp(input, "Veto")) 
    {
        printf("Swapping Veto channels\n");
        isVeto = kTRUE;
    }
    else
    {
        printf("Invalid answer - aborting!\n");
        printf("\n");
        return;
    }
    
    // set config
    Int_t maxElem;
    const Char_t* data;
    if (isVeto)
    {
        maxElem = gNVeto-1;
        data = gMapVeto;
    }
    else
    {
        maxElem = gNBaF2-1;
        data = gMapBaF2;
    }
 
    // ask for the elements to swap
    printf("Swapping hardware channels of element 1 <-> element 2\n");
    printf("Enter element 1 : ");
    std::cin.getline(input, 256);
    elem[0] = atoi(input);
    printf("Enter element 2 : ");
    std::cin.getline(input, 256);
    elem[1] = atoi(input);

    // validate input
    if (elem[0] == elem[1])
    {
        printf("Element numbers cannot be equal!\n");
        printf("\n");
        return;
    }
    if (elem[0] < 0 || elem[0] > maxElem)
    {
        printf("Invalid element number %d - aborting!", elem[0]);
        printf("\n");
        return;
    }
    if (elem[1] < 0 || elem[1] > maxElem)
    {
        printf("Invalid element number %d - aborting!", elem[1]);
        printf("\n");
        return;
    }
   
    // read current configuration
    if (!TTMySQLManager::GetManager()->ReadMaps(data, 2, elem, crate, mod, ch))
    {
        printf("Cannot read from database - aborting!\n");
        printf("\n");
        return;
    }

    // print current configuration
    printf("Element %3d : Crate %2d / Module %2d / Channel %2d\n", elem[0], crate[0], mod[0], ch[0]);
    printf("Element %3d : Crate %2d / Module %2d / Channel %2d\n", elem[1], crate[1], mod[1], ch[1]);
    
    // ask for confirmation
    Char_t input[256];
    if (isVeto) printf("Really swap Veto elements %d and %d? (yes/no) : ", elem[0], elem[1]);
    else printf("Really swap BaF2 elements %d and %d? (yes/no) : ", elem[0], elem[1]);
    std::cin.getline(input, 256);
    if (!strcmp(input, "yes")) 
    {
        // swap elements
        Int_t tmp = elem[0];
        elem[0] = elem[1];
        elem[1] = tmp;

        // write new configuration
        if (!TTMySQLManager::GetManager()->WriteMaps(data, 2, elem, crate, mod, ch))
        {
            printf("Cannot write to database - aborting!\n");
            printf("\n");
            return;
        }
        printf("Done.\n");
    }
    else
    {
        printf("Aborting!\n");
        printf("\n");
        return;
    }
     
    printf("\n");
}

//______________________________________________________________________________
void Check()
{
    // Perform a consistency check of the mapping.
    
    Int_t errors = 0;
    Int_t elem[gNChVeto];
    Int_t elemBaF2[gNBaF2];
    Int_t elemVeto[gNVeto];
    Int_t ch[gNChVeto] = { 0, 1, 2, 3, 4, 5, 6, 7 };
    for (Int_t i = 0; i < gNBaF2; i++) elemBaF2[i] = 0;
    for (Int_t i = 0; i < gNVeto; i++) elemVeto[i] = 0;

    printf("\n");
    printf("Channel consistency check\n");
    
    //
    // check BaF2 config
    //
    
    printf("Checking BaF2\n");

    // loop over crates
    for (Int_t i = 0; i < gNCratesBaF2; i++)
    {
        // loop over modules
        for (Int_t j = 0; j < gNModBaF2; j++)
        {
            // get elements in module
            if (!TTMySQLManager::GetManager()->ReadElements(gMapBaF2, gNChBaF2, ch, gCratesBaF2[i], j, elem))
            {
                printf("Cannot read from database - aborting!\n");
                printf("\n");
                return;
            }

            // check and mark elements
            for (Int_t k = 0; k < gNChBaF2; k++) 
            {
                if (elem[k] < 0 || elem[k] >= gNBaF2) 
                {
                    printf("Invalid element %d found in crate %d / module %d / channel %d!\n", elem[k], gCratesBaF2[i], j, k);
                    errors++;
                }
                else elemBaF2[elem[k]]++;
            }
        }
    }

    // loop over elements
    for (Int_t i = 0; i < gNBaF2; i++)
    {
        if (elemBaF2[i] == 0)
        {
            printf("Element %d not present in mapping!\n", i);
            errors++;
        }
        else if (elemBaF2[i] > 1)
        {
            printf("Multiple mappings of element %d!\n", i);
            errors++;
        }
    }
    
    //
    // check Veto config
    //
    
    printf("Checking Veto\n");

    // loop over crates
    for (Int_t i = 0; i < gNCratesVeto; i++)
    {
        // loop over modules
        for (Int_t j = 0; j < gNModVeto; j++)
        {
            // get elements in module
            if (!TTMySQLManager::GetManager()->ReadElements(gMapVeto, gNChVeto, ch, gCratesVeto[i], j, elem))
            {
                printf("Cannot read from database - aborting!\n");
                printf("\n");
                return;
            }

            // check and mark elements
            for (Int_t k = 0; k < gNChVeto; k++) 
            {
                if (elem[k] < 0 || elem[k] >= gNVeto) 
                {
                    printf("Invalid element %d found in crate %d / module %d / channel %d!\n", elem[k], gCratesVeto[i], j, k);
                    errors++;
                }
                else elemVeto[elem[k]]++;
            }
        }
    }

    // loop over elements
    for (Int_t i = 0; i < gNVeto; i++)
    {
        if (elemVeto[i] == 0)
        {
            printf("Element %d not present in mapping!\n", i);
            errors++;
        }
        else if (elemVeto[i] > 1)
        {
            printf("Multiple mappings of element %d!\n", i);
            errors++;
        }
    }

    // summary
    if (errors) 
        printf("%c[%d;%dmFound %d errors in the mapping!%c[%dm\n", 0x1b, 0, 31, errors, 0x1b, 0);
    else
        printf("%c[%d;%dmMappings ok%c[%dm\n", 0x1b, 0, 32, 0x1b, 0);

    printf("\n");
}

//______________________________________________________________________________
void ChannelManager()
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);
    
    // set silence mode
    TTMySQLManager::GetManager()->SetSilenceMode(kTRUE);
    
    // user info
    printf("\nWelcome to the TAPS channel manager!\n\n");
    printf("%c[%d;%dmATTENTION: All element indices start at zero here!%c[%dm\n", 0x1b, 0, 31, 0x1b, 0);
    printf("\n");
    printf("Enter 'help' for a list of commands\n");
    printf("Enter 'quit' or 'exit' to leave\n\n");
    
    // user input loop
    Char_t input[256];
    while (1)
    {
        // read command
        printf("CMD> ");
        std::cin.getline(input, 256);
        
        // convert characters to uppercase
        for (Int_t i = 0; i < strlen(input); i++)
            input[i] = toupper(input[i]);
        
        // check quit
        if (!strcmp(input, "QUIT")) break;
        if (!strcmp(input, "EXIT")) break;
        
        // check help
        if (!strcmp(input, "HELP")) PrintHelp();
        else if (!strcmp(input, "PRINT")) Print();
        else if (!strcmp(input, "SWAP")) Swap();
        else if (!strcmp(input, "CHECK")) Check();
    }

    gSystem->Exit(0);
}

