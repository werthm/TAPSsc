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

#include "TSQLServer.h"
#include "TError.h"

#include "TTDataType.h"

class TSQLResult;
class THashList;

class TTMySQLManager
{

private:
    TSQLServer* fDB;                            // MySQL database conneciton
    Bool_t fSilence;                            // silence mode toggle
    THashList* fData;                           // data types
    static TTMySQLManager* fgMySQLManager;      // pointer to static instance of this class
    
    Bool_t ReadDataTypes(TDataType_t type);
    TSQLResult* SendQuery(const Char_t* query);
    
    TTMySQLManager();

public:
    virtual ~TTMySQLManager();

    void SetSilenceMode(Bool_t s) { fSilence = s; }
    Bool_t IsConnected();
    
    const Char_t* GetDBName() const { return fDB ? fDB->GetDB() : 0; }
    const Char_t* GetDBHost() const { return fDB ? fDB->GetHost() : 0; }
    THashList* GetDataTypes() const { return fData; }
    
    Bool_t ReadParameters(const Char_t* data, Int_t length, 
                          Int_t* elem, Double_t* outPar);
    Bool_t ReadMaps(const Char_t* data, Int_t length, Int_t* elem,
                    Int_t* outCrate = 0, Int_t* outModule = 0, Int_t* outChannel = 0);
    Bool_t WriteParameters(const Char_t* data, Int_t length, 
                           Int_t* elem, Double_t* par);
    Bool_t WriteMaps(const Char_t* data, Int_t length, Int_t* elem, 
                     Int_t* crate, Int_t* module, Int_t* channel);
    Bool_t ReadElements(const Char_t* data, Int_t length, Int_t* channels,
                        Int_t crate, Int_t module, Int_t* outElem);
    
    void CreateTable(TTDataType* d);
    Bool_t InitDatabase();
    
    static TTMySQLManager* GetManager()
    {
        // return a pointer to the static instance of this class
        if (!fgMySQLManager) fgMySQLManager = new TTMySQLManager();
        if (!fgMySQLManager->IsConnected()) Error("GetManager", "No connection to the database!");
        return fgMySQLManager;
    }
    
    ClassDef(TTMySQLManager, 0) // Communication with MySQL Server
};

#endif

