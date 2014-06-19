/*************************************************************************
 * Author: Dominik Werthmueller, 2014
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// WriteHV.C                                                            //
//                                                                      //
// Write the HV values to the hardware.                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void WriteHV(const Char_t* dType = "Par.BaF2.HV")
{
    // load TAPSsc
    gSystem->Load("libTAPSsc.so");
    
    // load configuration file
    Char_t cfg[256];
    sprintf(cfg, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    gEnv->ReadFile(cfg, kEnvLocal);
    
    // get selected data type
    TTDataTypePar* dataType = TTMySQLManager::GetManager()->GetDataTypes()->FindObject(dType);
    if (!dataType)
    {
        printf("ERROR: Data type '%s' was not found!\n", dType);
        gSystem->Exit(1);
    }

    // user info
    printf("Writing HV for %d elements of %s\n", dataType->GetSize(), dataType->GetName());

    // loop over elements
    for (Int_t i = 0; i < dataType->GetSize(); i++) 
    {
        // write HV to hardware
        if (!TTServerManager::GetManager()->WriteHV(dataType, i))
        {
            printf("ERROR: There was an error during HV writing of element %d!\n", i);
        }
        else
        {
            printf("INFO: HV value of element %d was successfully written.\n", i);
        }
    }
 
    gSystem->Exit(0);
}

