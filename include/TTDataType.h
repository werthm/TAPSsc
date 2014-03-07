/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTDataType                                                           //
//                                                                      //
// Data type class.                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTDATATYPE_H
#define TTDATATYPE_H

#include "TNamed.h"


enum ETDataType {
    kNoType,
    kParType,
    kMapType
};
typedef ETDataType TDataType_t;


class TTDataType : public TNamed
{

private:
    TDataType_t fType;          // type of data
    Int_t fSize;                // number of parameters
    TString fTableName;         // name of the database table

public:
    TTDataType() : TNamed(), 
                   fType(kNoType), fSize(0), fTableName("") { }
    TTDataType(const Char_t* name, const Char_t* title, 
               TDataType_t type, Int_t nSize);
    virtual ~TTDataType() { }
 
    TDataType_t GetType() const { return fType; }
    Int_t GetSize() const { return fSize; }
    const Char_t* GetTableName() const { return fTableName.Data(); }

    void SetTableName(TString tName) { fTableName = tName; }
    
    virtual void Print(Option_t* option = "") const;
    virtual ULong_t Hash() const { return fName.Hash(); }

    ClassDef(TTDataType, 1) // Data type class
};

#endif

