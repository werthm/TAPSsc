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


#ifndef TTMYSQLMANAGER_H
#define TTMYSQLMANAGER_H

#include <fstream>

#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TError.h"
#include "TEnv.h"
#include "TObjString.h"
#include "TObjArray.h"
#include "THashList.h"
#include "TSystem.h"

#include "TTConfig.h"
#include "TTDataType.h"


class TTMySQLManager
{

private:
    TSQLServer* fDB;                            // MySQL database conneciton
    Bool_t fSilence;                            // silence mode toggle
    THashList* fData;                           // data types
    static TTMySQLManager* fgMySQLManager;      // pointer to static instance of this class
    
    Bool_t ReadDataTypes(TDataType_t type);
   
    TSQLResult* SendQuery(const Char_t* query);

public:
    TTMySQLManager();
    virtual ~TTMySQLManager();

    void SetSilenceMode(Bool_t s) { fSilence = s; }
    Bool_t IsConnected();
    
    const Char_t* GetDBName() const { return fDB ? fDB->GetDB() : 0; }
    const Char_t* GetDBHost() const { return fDB ? fDB->GetHost() : 0; }
    THashList* GetDataTypes() const { return fData; }
    
    void CreateTable(TTDataType* d);
 
    Bool_t ReadParameters(const Char_t* data, Int_t length, Double_t* par);
    Bool_t ReadMaps(const Char_t* data, Int_t length, Int_t* crate = 0,
                    Int_t* board = 0, Int_t* channel = 0);
    Bool_t WriteParameters(const Char_t* data, Int_t length, Double_t* par);
    Bool_t WriteMaps(const Char_t* data, Int_t length, Int_t* crate,
                     Int_t* board, Int_t* channel);
    
    void InitDatabase();
    
    static TTMySQLManager* GetManager()
    {
        // return a pointer to the static instance of this class
        if (!fgMySQLManager) fgMySQLManager = new TTMySQLManager();
        if (!fgMySQLManager->IsConnected())
        {
            Error("GetManager", "No connection to the database!");
            return 0;
        }
        else return fgMySQLManager;
    }
    
    ClassDef(TTMySQLManager, 0) // Communication with MySQL Server
};

#endif

