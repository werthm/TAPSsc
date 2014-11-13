/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTDataTypeMap                                                        //
//                                                                      //
// Map data type class.                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTDATATYPEMAP_H
#define TTDATATYPEMAP_H

#include "TTDataType.h"

class TTDataTypeMap : public TTDataType
{

public:
    TTDataTypeMap() : TTDataType() { }
    TTDataTypeMap(const Char_t* name, const Char_t* title, 
                  Int_t nSize);
    virtual ~TTDataTypeMap() { }
 
    ClassDef(TTDataTypeMap, 1) // Map data type class
};

#endif

