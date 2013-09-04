// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Init.C                                                               //
//                                                                      //
// Initialize the TAPSsc database.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void WritePar(const Char_t* data, const Char_t* file)
{
    // Write the parameter data type 'data' to the database using the values in the
    // ASCII file 'file' (TAPSMaintain export format).
    
    Double_t par[1000];
    Int_t nPar = 0;

    // read from file
    Char_t* line[256];
    FILE* f = fopen(file, "r");
    while (fgets(line, 256, f)) 
    {
        TString l((const Char_t*)line);
        l.Remove(TString::kBoth, ' ');
        l.Remove(TString::kBoth, '\n');
        if (l.BeginsWith("#") || l == "") continue;
        else sscanf(l.Data(), "%*d%lf", &par[nPar++]);
    }
    fclose(f);

    // show parameters
    //for (Int_t i = 0; i < nPar; i++) printf("Par %03d : %lf\n", i, par[i]);
    //printf("Read %d parameters\n", nPar);
    
    // write to database
    TTMySQLManager::GetManager()->WriteParameters(data, nPar, par);
}

//______________________________________________________________________________
void WriteMap(const Char_t* data, const Char_t* file)
{
    // Write the map data type 'data' to the database using the values in the
    // ASCII file 'file' (custom SQL export format).
    
    // par array
    Int_t mainframe[1000];
    Int_t board[1000];
    Int_t channel[1000];
    Int_t nMap = 0;

    // read from file
    Char_t* line[256];
    FILE* f = fopen(file, "r");
    while (fgets(line, 256, f)) 
    {
        TString l((const Char_t*)line);
        l.Remove(TString::kBoth, ' ');
        l.Remove(TString::kBoth, '\n');
        if (l.BeginsWith("#") || l == "") continue;
        else 
        {
            sscanf(l.Data(), "%*d%d%d", &mainframe[nMap], &channel[nMap]);
            board[nMap] = 0;
            nMap++;
        }
    }
    fclose(f);

    // show data
    //for (Int_t i = 0; i < nMap; i++) printf("Par %03d : %d  %d\n", i, mainframe[i], channel[i]);
    //printf("Read %d parameters\n", nMap);
    
    // write to database
    TTMySQLManager::GetManager()->WriteMaps(data, nMap, mainframe, board, channel);
}

//______________________________________________________________________________
void Init()
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);

    // init TAPSsc database
    if (TTMySQLManager::GetManager()->InitDatabase())
    {
        // write default parameter data
        WritePar("Par.BaF2.HV", "data/init/par_baf2_hv");
        WritePar("Par.BaF2.Thr.CFD", "data/init/par_baf2_cfd");
        WritePar("Par.BaF2.Thr.LED1", "data/init/par_baf2_led1");
        WritePar("Par.BaF2.Thr.LED2", "data/init/par_baf2_led2");
        WritePar("Par.BaF2.QAC.LG", "data/init/par_baf2_qac_ped_lg");
        WritePar("Par.BaF2.QAC.LGS", "data/init/par_baf2_qac_ped_lgs");
        WritePar("Par.BaF2.QAC.SG", "data/init/par_baf2_qac_ped_sg");
        WritePar("Par.BaF2.QAC.SGS", "data/init/par_baf2_qac_ped_sgs");
        WritePar("Par.PWO.HV", "data/init/par_pwo_hv");
        WritePar("Par.Veto.Thr.LED", "data/init/par_veto_led");
        WritePar("Par.Veto.QAC", "data/init/par_veto_qac_ped");

        // write default map data
        WriteMap("Map.BaF2.HV", "data/init/map_baf2_hv");
        WriteMap("Map.PWO.HV", "data/init/map_pwo_hv");
    }

    gSystem->Exit(0);
}

