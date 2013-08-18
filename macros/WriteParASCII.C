// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// WriteParASCII.C                                                      //
//                                                                      //
// Write parameters from an ASCII file to the database.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void WriteParASCII()
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);

    // configuration
    const Char_t inputFile[] = "30MeV_LED1_04.2013.dat";
    const Char_t data[]      = "Par.BaF2.Thr.LED1";
    
    // par array
    Double_t par[1000];
    Int_t nPar = 0;

    // read from file (one value per line)
    Char_t* line[256];
    FILE* f = fopen(inputFile, "r");
    while (fgets(line, 256, f)) sscanf(line, "%lf", &par[nPar++]);
    fclose(f);

    // show paramters
    for (Int_t i = 0; i < nPar; i++) printf("Par %03d : %lf\n", i, par[i]);
    printf("Read %d parameters\n", nPar);
    
    // ask user
    Char_t answer[128];
    printf("Write to data '%s'? (yes/no) : ", data);
    scanf("%s", answer);
    if (strcmp(answer, "yes")) 
    {
        printf("Aborted.\n");
        gSystem->Exit(0);
    }

    // write to database
    TTMySQLManager::GetManager()->WriteParameters(data, nPar, par);
  
    gSystem->Exit(0);
}

