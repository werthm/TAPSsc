// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMySQLManager                                                       //
//                                                                      //
// This class handles all the communication with the MySQL server.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTMySQLManager.h"

ClassImp(TTMySQLManager)


// init static class members
TTMySQLManager* TTMySQLManager::fgMySQLManager = 0;


//______________________________________________________________________________
TTMySQLManager::TTMySQLManager()
{
    // Constructor.

    // init members
    fDB = 0;
    fSilence = kFALSE;
    fData = new THashList();
    fData->SetOwner(kTRUE);

    // read parameter types
    if (!ReadDataTypes(kParType))
    {
        if (!fSilence) Error("TTMySQLManager", "Could not read the TAPSsc parameter definitions!");
        return;
    }

    // read map types
    if (!ReadDataTypes(kMapType))
    {
        if (!fSilence) Error("TTMySQLManager", "Could not read the TAPSsc map definitions!");
        return;
    }

    // get database hostname
    const Char_t* strDBHost = gEnv->GetValue("DB.Host", "null");
    if (!strcmp(strDBHost, "null")) 
    {
        if (!fSilence) Error("TTMySQLManager", "Database host not included in configuration file!");
        return;
    }

    // get database name
    const Char_t* strDBName = gEnv->GetValue("DB.Name", "null");
    if (!strcmp(strDBName, "null")) 
    {
        if (!fSilence) Error("TTMySQLManager", "Database name not included in configuration file!");
        return;
    }

    // get database user
    const Char_t* strDBUser = gEnv->GetValue("DB.User", "null");
    if (!strcmp(strDBUser, "null")) 
    {
        if (!fSilence) Error("TTMySQLManager", "Database user not included in configuration file!");
        return;
    }
  
    // get database password
    const Char_t* strDBPass = gEnv->GetValue("DB.Pass", "null");
    if (!strcmp(strDBPass, "null")) 
    {
        if (!fSilence) Error("TTMySQLManager", "Database password not included in configuration file!");
        return;
    }
     
    // open connection to MySQL server on localhost
    Char_t szMySQL[200];
    sprintf(szMySQL, "mysql://%s/%s", strDBHost, strDBName);
    fDB = TSQLServer::Connect(szMySQL, strDBUser, strDBPass);
    if (!fDB)
    {
        if (!fSilence) Error("TTMySQLManager", "Cannot connect to the database '%s' on '%s@%s'!",
               strDBName, strDBUser, strDBHost);
        return;
    }
    else if (fDB->IsZombie())
    {
        if (!fSilence) Error("TTMySQLManager", "Cannot connect to the database '%s' on '%s@%s'!",
               strDBName, strDBUser, strDBHost);
        return;
    }
    else
    {
        if (!fSilence) Info("TTMySQLManager", "Connected to the database '%s' on '%s@%s' using TAPSsc %s",
                            strDBName, strDBUser, strDBHost, TTConfig::kTAPSscVersion);
    }
}

//______________________________________________________________________________
TTMySQLManager::~TTMySQLManager()
{
    // Destructor.

    // close DB
    if (fDB) delete fDB;
    if (fData) delete fData;
}

//______________________________________________________________________________
Bool_t TTMySQLManager::ReadDataTypes(TDataType_t type)
{
    // Read the TAPSsc data types definitions from the configuration file.
    // Return kTRUE on success, kFALSE if an error occured.
    
    Int_t added = 0;
    
    // try to get the TAPSsc source path from the shell variable TAPSSC
    // otherwise use the current directory
    TString path;
    if (gSystem->Getenv("TAPSSC")) path = gSystem->Getenv("TAPSSC");
    else path = gSystem->pwd();
    
    // choose definition file
    const Char_t* cfg;
    if (type == kParType) cfg = gEnv->GetValue("DB.DataTypes.Par", "null");
    else if (type == kMapType) cfg = gEnv->GetValue("DB.DataTypes.Map", "null");
    else return kFALSE;

    // check definition file
    if (!strcmp(cfg, "null")) 
    {
        if (!fSilence) Error("ReadDataTypes", "Could not open the data type definition file!");
        return kFALSE;
    }

    // open the file
    path.Append("/");
    path.Append(cfg);
    std::ifstream infile;
    infile.open(path.Data());
        
    // check if file is open
    if (!infile.is_open())
    {
        if (!fSilence) Error("ReadDataTypes", "Could not open data definition file '%s'", path.Data());
        return kFALSE;
    }
    else
    {
        // read the file
        while (infile.good())
        {
            TString line;
            line.ReadLine(infile);
            
            // trim line
            line.Remove(TString::kBoth, ' ');
            
            // skip comments and empty lines
            if (line.BeginsWith("#") || line == "") continue;
            else
            {
                // tokenize the string
                TObjArray* token = line.Tokenize(",");
                
                // some variables for data extraction
                Int_t count = 0;
                TString name;
                TString title;
                TString table;
                Int_t size = 0;
                Double_t min = 0;
                Double_t max = 0;

                // iterate over tokens
                TIter next(token);
                TObjString* s;
                while ((s = (TObjString*)next()))
                {
                    // get the string and trim it
                    TString str(s->GetString());
                    str.Remove(TString::kBoth, ' ');
                    
                    // set data
                    switch (count)
                    {
                        case 0:
                            name = str;
                            break;
                        case 1:
                            title = str;
                            break;
                        case 2:
                            table = str;
                            break;
                        case 3:
                            size = atoi(str.Data());
                            break;
                        case 4:
                            min = atof(str.Data());
                            break;
                        case 5:
                            max = atof(str.Data());
                            break;
                    }

                    count++;
                }

                // clean-up
                delete token;
                
                // check number of tokens
                if (count == 6 && type == kParType)
                {
                    // create a new data type object
                    TTDataTypePar* data = new TTDataTypePar(name, title, size);
                    data->SetTableName(table);
                    data->SetMin(min);
                    data->SetMax(max);

                    // add it to list
                    fData->Add(data);

                    // count data type
                    added++;
                }
                else if (count == 4 && type == kMapType)
                {
                    // create a new data type object
                    TTDataTypeMap* data = new TTDataTypeMap(name, title, size);
                    data->SetTableName(table);
                
                    // add it to list
                    fData->Add(data);

                    // count data type
                    added++;
                }
                else
                {
                    if (!fSilence) Error("ReadDataTypes", "Error in data type definition '%s'!", line.Data());
                    return kFALSE;
                }
            }
        }
    }
    
    // close the file
    infile.close();

    // user information
    Char_t tmp[16];
    if (type == kParType) strcpy(tmp, "parameter");
    else if (type == kMapType) strcpy(tmp, "map");
    if (!fSilence) Info("ReadDataTypes", "Registered %d data definitions of type '%s'", added, tmp);
    
    return kTRUE;
}

//______________________________________________________________________________
TSQLResult* TTMySQLManager::SendQuery(const Char_t* query)
{
    // Send a query to the database and return the result.

    // check server connection
    if (!IsConnected())
    {
        if (!fSilence) Error("SendQuery", "No connection to the database!");
        return 0;
    }

    // execute query
    return fDB->Query(query);
}

//______________________________________________________________________________
Bool_t TTMySQLManager::IsConnected()
{
    // Check if the connection to the database is open.

    if (!fDB)
    {
        if (!fSilence) Error("IsConnected", "Cannot access database!");
        return kFALSE;
    }
    else
        return kTRUE;
}

//______________________________________________________________________________
Bool_t TTMySQLManager::ReadParameters(const Char_t* data, Int_t length, Double_t* par)
{
    // Read 'length' entries of the parameter data type 'data' from the database to
    // the array 'par'.
    // Return kFALSE if an error occured, otherwise kTRUE.
 
    // get the data type
    TTDataType* d = (TTDataType*) fData->FindObject(data);
    if (!d)
    {
        if (!fSilence) Error("ReadParameters", "Data type '%s' was not found!", data);
        return kFALSE;
    }

    // check data type
    if (d->GetType() != kParType)
    {
        if (!fSilence) Error("ReadParameters", "Data type '%s' is not a parameter data type!", data);
        return kFALSE;
    }   
 
    // create the query
    Char_t query[1024];
    sprintf(query,
            "SELECT value FROM %s WHERE id IN (SELECT MAX(id) FROM %s GROUP BY elem )",
            d->GetTableName(), d->GetTableName());

    // read from database
    TSQLResult* res = SendQuery(query);

    // check result
    if (!res)
    {
        if (!fSilence) Error("ReadParameters", "Could not read values of parameter data type '%s'!", 
                             d->GetName());
        return kFALSE;
    }
    else if (!res->GetRowCount())
    {
        if (!fSilence) Error("ReadParameters", "No values found for parameter data type '%s'!", 
                             d->GetName());
        delete res;
        return kFALSE;
    }

    // count rows
    if (res->GetRowCount() != length)
    {
        if (!fSilence) Error("ReadParameters", "Numbers of requested and read values differ! (%d != %d)", 
                             length, res->GetRowCount());
        delete res;
        return kFALSE;
    }

    // read get data
    for (Int_t i = 0; i < length; i++) 
    {
        TSQLRow* row = res->Next();
        par[i] = atof(row->GetField(0));
        delete row;
    }

    // clean-up
    delete res;
    
    // user information
    if (!fSilence) Info("ReadParameters", "Read %d parameters of '%s' from the database", 
                        length, data);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTMySQLManager::ReadMaps(const Char_t* data, Int_t length, Int_t* crate,
                                Int_t* board, Int_t* channel)
{
    // Read 'length' entries of the map data type 'data' from the database to
    // the arrays 'crate', 'board' and 'channel' (if non-zero, respectively).
    // Return kFALSE if an error occured, otherwise kTRUE.
 
    // get the data type
    TTDataType* d = (TTDataType*) fData->FindObject(data);
    if (!d)
    {
        if (!fSilence) Error("ReadMaps", "Data type '%s' was not found!", data);
        return kFALSE;
    }

    // check data type
    if (d->GetType() != kMapType)
    {
        if (!fSilence) Error("ReadMaps", "Data type '%s' is not a map data type!", data);
        return kFALSE;
    }   
 
    // create the query
    Char_t query[1024];
    sprintf(query,
            "SELECT crate,board,channel FROM %s WHERE id IN (SELECT MAX(id) FROM %s GROUP BY elem )",
            d->GetTableName(), d->GetTableName());

    // read from database
    TSQLResult* res = SendQuery(query);

    // check result
    if (!res)
    {
        if (!fSilence) Error("ReadMaps", "Could not read values of map data type '%s'!", 
                             d->GetName());
        return kFALSE;
    }
    else if (!res->GetRowCount())
    {
        if (!fSilence) Error("ReadMaps", "No values found for map data type '%s'!", 
                             d->GetName());
        delete res;
        return kFALSE;
    }

    // count rows
    if (res->GetRowCount() != length)
    {
        if (!fSilence) Error("ReadMaps", "Numbers of requested and read values differ! (%d != %d)", 
                             length, res->GetRowCount());
        delete res;
        return kFALSE;
    }

    // read get data
    for (Int_t i = 0; i < length; i++) 
    {
        TSQLRow* row = res->Next();
        if (crate) crate[i] = atof(row->GetField(0));
        if (board) board[i] = atof(row->GetField(1));
        if (channel) channel[i] = atof(row->GetField(2));
        delete row;
    }

    // clean-up
    delete res;
    
    // user information
    if (!fSilence) Info("ReadMaps", "Read %d maps of '%s' from the database", 
                        length, data);
    
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTMySQLManager::WriteParameters(const Char_t* data, Int_t length, Double_t* par)
{
    // Write 'length' entries of the parameter data type 'data' using the value 
    // array 'par' to the database.
    // Return kFALSE if an error occured, otherwise kTRUE.
  
    // get the data type
    TTDataType* d = (TTDataType*) fData->FindObject(data);
    if (!d)
    {
        if (!fSilence) Error("WriteParameters", "Data type '%s' was not found!", data);
        return kFALSE;
    }

    // check data type
    if (d->GetType() != kParType)
    {
        if (!fSilence) Error("WriteParameters", "Data type '%s' is not a parameter data type!", data);
        return kFALSE;
    }
    
    // insert the parameters
    Int_t written = 0;
    Char_t query[1024];
    for (Int_t i = 0; i < length; i++)
    {
        // build query
        sprintf(query, "INSERT INTO %s (elem, value) VALUES (%d, %lf)", d->GetTableName(), i, par[i]);
 
        // write data to database
        TSQLResult* res = SendQuery(query);
        if (res) 
        {
            delete res;
            written++;
        }
    }

    // check outcome
    if (written == length)
    {
        if (!fSilence) Info("WriteParameters", "Wrote %d parameters of '%s' to the database", 
                                               length, data);
        return kTRUE;
    }
    else
    {
        if (!fSilence) Error("WriteParameters", "Could not write %d parameters of '%s'!", 
                                                length, data);
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t TTMySQLManager::WriteMaps(const Char_t* data, Int_t length, Int_t* crate, 
                                 Int_t* board, Int_t* channel)
{
    // Write 'length' entries of the map data type 'data' using the value 
    // arrays 'crate', 'board' and 'channel' to the database.
    // Return kFALSE if an error occured, otherwise kTRUE.
    
    // get the data type
    TTDataType* d = (TTDataType*) fData->FindObject(data);
    if (!d)
    {
        if (!fSilence) Error("WriteMaps", "Data type '%s' was not found!", data);
        return kFALSE;
    }

    // check data type
    if (d->GetType() != kMapType)
    {
        if (!fSilence) Error("WriteMaps", "Data type '%s' is not a map data type!", data);
        return kFALSE;
    }
    
    // insert the parameters
    Int_t written = 0;
    Char_t query[1024];
    for (Int_t i = 0; i < length; i++)
    {
        // build query
        sprintf(query, "INSERT INTO %s (elem, crate, board, channel) VALUES (%d, %d, %d, %d)", 
                d->GetTableName(), i, crate[i], board[i], channel[i]);
 
        // write data to database
        TSQLResult* res = SendQuery(query);
        if (res) 
        {
            delete res;
            written++;
        }
    }

    // check outcome
    if (written == length)
    {
        if (!fSilence) Info("WriteMaps", "Wrote %d maps of '%s' to the database", 
                                         length, data);
        return kTRUE;
    }
    else
    {
        if (!fSilence) Error("WriteMaps", "Could not write %d maps of '%s'!", 
                                          length, data);
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t TTMySQLManager::InitDatabase()
{
    // Init a new TAPSsc database on a MySQL server.
    // Return kFALSE on user abort, otherwise kTRUE.
    
    // ask for user confirmation
    Char_t answer[256];
    printf("\nWARNING: You are about to initialize a new TAPSsc database.\n"
           "         All existing tables in the database '%s' on '%s'\n"
           "         will be deleted!\n\n", fDB->GetDB(), fDB->GetHost());
    printf("Are you sure to continue? (yes/no) : ");
    scanf("%s", answer);
    if (strcmp(answer, "yes")) 
    {
        printf("Aborted.\n");
        return kFALSE;
    }
    
    // create the data tbles
    TTDataType* d;
    TIter next(fData);
    while ((d = (TTDataType*)next())) CreateTable(d);

    return kTRUE;
}

//______________________________________________________________________________
void TTMySQLManager::CreateTable(TTDataType* d)
{
    // Create a table for the data type 'data'.
    
    // user information
    if (!fSilence) Info("CreateTable", "Adding data table '%s'", d->GetTableName());
        
    // delete the old table if it exists
    TSQLResult* res = SendQuery(TString::Format("DROP TABLE IF EXISTS %s", d->GetTableName()));
    delete res;

    // prepare CREATE TABLE query
    Char_t query[1024];
    const Char_t* tableFormat = 0;
    if (d->GetType() == kParType) tableFormat = TTConfig::kParTableFormat;
    else if (d->GetType() == kMapType) tableFormat = TTConfig::kMapTableFormat;
    sprintf(query, "CREATE TABLE %s ( %s )", d->GetTableName(), tableFormat);

    // submit the query
    res = SendQuery(query);
    delete res;
}

