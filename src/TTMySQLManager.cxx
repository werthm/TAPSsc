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

    // read map types
    if (!ReadDataTypes(kMapType))
    {
        if (!fSilence) Error("TTMySQLManager", "Could not read the TAPSsc map definitions!");
        return;
    }
    
    // read parameter types
    if (!ReadDataTypes(kParType))
    {
        if (!fSilence) Error("TTMySQLManager", "Could not read the TAPSsc parameter definitions!");
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
    // Return kTRUE on success, kFALSE if an error occurred.
    
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
                TString map;

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
                        case 6:
                            map = str;
                            break;
                    }

                    count++;
                }

                // clean-up
                delete token;
                
                // check number of tokens
                if (count == 7 && type == kParType)
                {
                    // create a new data type object
                    TTDataTypePar* data = new TTDataTypePar(name.Data(), title.Data(), size);
                    data->SetTableName(table);
                    data->SetMin(min);
                    data->SetMax(max);
                    
                    // try to get map
                    TTDataTypeMap* m = (TTDataTypeMap*) fData->FindObject(map.Data());
                    if (!m)
                    {
                        if (!fSilence) Error("ReadDataTypes", "Could not find map '%s' for parameter '%s'!", 
                                             map.Data(), name.Data());
                        return kFALSE;
                    }
                    
                    // set map
                    data->SetMap(m);

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
Int_t TTMySQLManager::GetElements(const Char_t* data, Int_t crate, Int_t module, 
                                  Int_t* outElem)
{
    // Search all elements of the crate 'crate' and module 'module' using the map
    // data type 'data' and write / them to 'outElem'. 
    // Return the number of elements or -1 if any error occurred.
    
    // get the data type
    TTDataType* d = (TTDataType*) fData->FindObject(data);
    if (!d)
    {
        if (!fSilence) Error("GetElements", "Data type '%s' was not found!", data);
        return -1;
    }

    // check data type
    if (d->GetType() != kMapType)
    {
        if (!fSilence) Error("GetElements", "Data type '%s' is not a map data type!", data);
        return -1;
    }   
 
    // create the query
    Char_t query[256];
    sprintf(query, "select elem from %s where crate = %d AND module = %d ORDER BY channel ASC", 
            d->GetTableName(), crate, module);

    // read from database
    TSQLResult* res = SendQuery(query);

    // check result
    if (!res)
    {
        if (!fSilence) Error("GetElements", "Could not read table of map data type '%s'!", 
                             d->GetName());
        return -1;
    }
    else 
    {
        // read and save data
        Int_t nElem = res->GetRowCount();
        for (Int_t i = 0; i < nElem; i++) 
        {
            TSQLRow* row = res->Next();
            outElem[i] = atoi(row->GetField(0));
            delete row;
        }
        delete res;
        return nElem;
    }
}

//______________________________________________________________________________
Bool_t TTMySQLManager::ReadParameters(const Char_t* data, Int_t length, 
                                      Int_t* elem, Double_t* outPar)
{
    // Read 'length' entries of the parameter data type 'data' for the elements 
    // 'elem' from the database to the array 'outPar'.
    // Return kFALSE if an error occurred, otherwise kTRUE.

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
 
    // get the parameters
    Int_t read = 0;
    Char_t query[1024];
    for (Int_t i = 0; i < length; i++)
    {
        // build query
        sprintf(query, "SELECT value FROM %s WHERE elem = %d ORDER BY id DESC LIMIT 1", 
                d->GetTableName(), elem[i]);
 
        // get data from database
        TSQLResult* res = SendQuery(query);
        if (res)
        {
            if (res->GetRowCount() == 1)
            {
                TSQLRow* row = res->Next();
                outPar[i] = atof(row->GetField(0));
                
                delete row;
                delete res;
                read++;
            }
            else 
            {
                delete res;
            }
        } 
    }

    // check outcome
    if (read == length)
    {
        if (!fSilence) Info("ReadParameters", "Read %d parameters of '%s' from the database", 
                            length, data);
         return kTRUE;
    }
    else
    {
        if (!fSilence) Error("ReadParameters", "Could not read %d parameters of '%s'!", 
                             length, data);
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t TTMySQLManager::ReadMaps(const Char_t* data, Int_t length, Int_t* elem,
                                Int_t* outCrate, Int_t* outModule, Int_t* outChannel)
{
    // Read 'length' entries of the map data type 'data' for the elements 'elem' 
    // from the database to the arrays 'outCrate', 'outModule' and 'outChannel' 
    // (if non-zero, respectively).
    // Return kFALSE if an error occurred, otherwise kTRUE.
    
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
 
    // get the values
    Int_t read = 0;
    Char_t query[1024];
    for (Int_t i = 0; i < length; i++)
    {
        // build query
        sprintf(query, "SELECT crate,module,channel FROM %s WHERE elem = %d ORDER BY id DESC LIMIT 1", 
                        d->GetTableName(), elem[i]);
 
        // get data from database
        TSQLResult* res = SendQuery(query);
        if (res)
        {
            if (res->GetRowCount() == 1)
            {
                TSQLRow* row = res->Next();
                outCrate[i] = atoi(row->GetField(0));
                outModule[i] = atoi(row->GetField(1));
                outChannel[i] = atoi(row->GetField(2));
                
                delete row;
                delete res;
                read++;
            }
            else 
            {
                delete res;
            }
        } 
    }

    // check outcome
    if (read == length)
    {
        if (!fSilence) Info("ReadMaps", "Read %d maps of '%s' from the database", 
                            length, data);
         return kTRUE;
    }
    else
    {
        if (!fSilence) Error("ReadMaps", "Could not read %d maps of '%s'!", 
                             length, data);
        return kFALSE;
    }
}

//______________________________________________________________________________
Bool_t TTMySQLManager::WriteParameters(const Char_t* data, Int_t length, 
                                       Int_t* elem, Double_t* par)
{
    // Write 'length' entries of the parameter data type 'data' for the elements 
    // in 'elem' using the value array 'par' to the database.
    // Return kFALSE if an error occurred, otherwise kTRUE.
  
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

    // check parameter limits
    TTDataTypePar* dd = (TTDataTypePar*) d;
    for (Int_t i = 0; i < length; i++)
    {
        // check minimum
        if (par[i] < dd->GetMin())
        {
            Error("WriteParameters", "Value %.f of element %d below minimum of %.f - aborting!", 
                  par[i], i, dd->GetMin());
            return kFALSE;
        }
        // check maximum
        if (par[i] > dd->GetMax())
        {
            Error("WriteParameters", "Value %.f of element %d above maximum of %.f - aborting!", 
                  par[i], i, dd->GetMax());
            return kFALSE;
        }
    }
    
    // insert the parameters
    Int_t written = 0;
    Char_t query[1024];
    for (Int_t i = 0; i < length; i++)
    {
        // build query
        sprintf(query, "INSERT INTO %s (elem, value) VALUES (%d, %lf)", d->GetTableName(), elem[i], par[i]);
 
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
Bool_t TTMySQLManager::WriteMaps(const Char_t* data, Int_t length, Int_t* elem,
                                 Int_t* crate, Int_t* module, Int_t* channel)
{
    // Write 'length' entries of the map data type 'data' for the elements in
    // 'elem' using the value arrays 'crate', 'module' and 'channel' to the database.
    // Return kFALSE if an error occurred, otherwise kTRUE.
    
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
        sprintf(query, "INSERT INTO %s (elem, crate, module, channel) VALUES (%d, %d, %d, %d)", 
                d->GetTableName(), elem[i], crate[i], module[i], channel[i]);
 
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
    // Return kTRUE on success, otherwise kFALSE
    
    // check server connection
    if (!IsConnected())
    {
        if (!fSilence) Error("InitDatabase", "No connection to the database!");
        return kFALSE;
    }
    
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
    sprintf(query, "CREATE TABLE %s ( %s ) ENGINE = INNODB", d->GetTableName(), tableFormat);

    // submit the query
    res = SendQuery(query);
    delete res;
}

