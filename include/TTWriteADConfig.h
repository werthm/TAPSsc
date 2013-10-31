// SVN Info: $Id$

/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfig                                                      //
//                                                                      //
// Base class to write AcquDAQ cofiguration files from a template file  //
// and values from the MySQL database.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTWRITEADCONFIG_H
#define TTWRITEADCONFIG_H

#include <fstream>

#include "TEnv.h"
#include "TError.h"

#include "TTMySQLManager.h"
#include "TTUtils.h"


class TTWriteADConfig
{

private:
    const Char_t* fTemplatePath;        // pointer to template path (in gEnv)
    const Char_t* fADConfigPath;        // pointer to AcquDAQ config file path (in gEnv)

    std::ifstream* fTemplateFile;       // template file stream
    std::ofstream* fADConfigFile;       // AcquDAQ config file stream

    Bool_t OpenTemplate();
    Bool_t OpenADConfig(Int_t module);
    Bool_t RemoveADConfig(Int_t module);

protected:
    Int_t fCrate;                       // vme crate number
    Char_t* fType;                      // crate type (i.e. BaF2, Veto, PWO)
    Char_t* fMap;                       // map data type
    Int_t fModule;                      // number of currently processed module

    Int_t fNElements;                   // number of elements (i.e. channels) on module
    Int_t* fElements;                   // list of element numbers on module

    Char_t* fLineIn;                    // current line of template file to parse
    Char_t* fLineOut;                   // current parsed line to write to config file

    virtual Bool_t ParseLine() = 0;

public:
    TTWriteADConfig(const Int_t crate, const Char_t* const type,
                    const Char_t* map);
    virtual ~TTWriteADConfig();

    Bool_t Write(const Int_t module);

    ClassDef(TTWriteADConfig, 0) // Write AcquDAQ coniguration files base
};

#endif

