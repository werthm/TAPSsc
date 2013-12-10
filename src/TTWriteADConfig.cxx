// SVN Info: $Id: TTWriteADConfig.cxx 1754 2013-11-12 16:57:24Z werthm $

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


#include "TTWriteADConfig.h"

ClassImp(TTWriteADConfig)


//______________________________________________________________________________
TTWriteADConfig::TTWriteADConfig(const Int_t crate, const Char_t* const type, 
                                 const Char_t* map, const Int_t nElem)
{
    // Constructor.

    fCrate = crate;
    fType = new Char_t[256];
    strcpy(fType, type);
    fMap = new Char_t[256];
    strcpy(fMap, map);
    fModule = -1;

    fNElements = nElem;
    fElements = new Int_t[fNElements];

    fLineIn  = new Char_t[1024];
    fLineOut = new Char_t[1024];

    fTemplatePath = 0;
    fADConfigPath = 0;

    fTemplateFile = 0;
    fADConfigFile = 0;
}

//______________________________________________________________________________
TTWriteADConfig::~TTWriteADConfig()
{
    // Destructor.

    if (fType) delete [] fType;
    if (fMap) delete [] fMap;

    if (fElements) delete fElements;

    if (fLineIn)  delete [] fLineIn;
    if (fLineOut) delete [] fLineOut;

    if (fTemplateFile)
    {
        if (fTemplateFile->is_open())
            fTemplateFile->close();
        delete fTemplateFile;
    }

    if (fADConfigFile)
    {
        if (fADConfigFile->is_open())
            fADConfigFile->close();
        delete fADConfigFile;
    }
}

//______________________________________________________________________________
Bool_t TTWriteADConfig::Write(const Int_t module)
{
    // Writes the AcquDAQ configuration file for the module 'module' of the
    // crate 'fCrate'. Returns kTRUE if successful, kFALSE otherwise.
    
    // set current module number
    fModule = module;

    // open template
    if (!OpenTemplate()) return kFALSE;

    // open output file
    if (!OpenADConfig(module)) return kFALSE;

    // init channel array
    Int_t ch[fNElements];
    for (Int_t i = 0; i < fNElements; i++) ch[i] = i;

    // get elements in the module
    if (!TTMySQLManager::GetManager()->ReadElements(fMap, fNElements, ch, fCrate, module, fElements))
    {
        Error("Write", "Could not read elements from the database!");
        return kFALSE;
    }
  
    // parse lines of template files
    while (fTemplateFile->good())
    {
        // read next line
        fTemplateFile->getline(fLineIn, 1024);

        // call child parse line function
        if (!ParseLine())
        {
            Error("Write", "Child function 'ParseLine()' reported an error.");
            fTemplateFile->close();
            fADConfigFile->close();
            RemoveADConfig(module);
            return kFALSE;
        }

        // write output
        sprintf(fLineOut, "%s\n", fLineOut);
        *fADConfigFile << fLineOut;
    }

    // check whether file is at end
    if (!fTemplateFile->eof())
    {
        Error("Write", "Error while reading template file '%s'.", fTemplatePath);
        fTemplateFile->close();
        fADConfigFile->close();
        RemoveADConfig(module);
        return kFALSE;
    }

    // close files
    fTemplateFile->close();
    fADConfigFile->close();

    Info("Write", "AcquDAQ configuration file (crate: %02d, module: %02d) '%s%02d.dat' successfully written.", 
         fCrate, module, fADConfigPath, module);
    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTWriteADConfig::OpenTemplate()
{
    // (Re-)opens the AcquDAQ template file for the server type 'fType'. Returns
    // kTRUE if successful, kFALSE otherwise.
    
    Char_t tmp[256];

    // reset template file
    if (!fTemplateFile)
    {
        // prepare environment key
        sprintf(tmp, "ARDAQ.Cfg.Mod.Templ.%s", fType);

        // get template path from config file
        if (gEnv->Defined(tmp))
        {
            fTemplatePath = gEnv->GetValue(tmp, "null");
        }
        else
        {
            Error("OpenTemplate", "Could not find template file path in cofiguration (key: '%s').", tmp);
            return kFALSE;
        }
    }
    else
    {
        // close file fist
        if (fTemplateFile->is_open()) fTemplateFile->close();

        // delete file (necessary ?)
        delete fTemplateFile;
        fTemplateFile = 0;
    }
    
    // construct absolute template path
    if (gSystem->Getenv("TAPSSC")) sprintf(tmp, "%s/%s", gSystem->Getenv("TAPSSC"), fTemplatePath);
    else strcpy(tmp, fTemplatePath);

    // open template file
    fTemplateFile = new ifstream();
    fTemplateFile->open(tmp, ifstream::in);

    // error if file could not be opened, otherwise save file name
    if (!fTemplateFile->is_open())
    {
        Error("OpenTemplate", "Template file '%s' could not be opened!", tmp);
        return kFALSE;
    }

    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTWriteADConfig::OpenADConfig(Int_t module)
{
    // Opens the AcquDAQ config file for the module 'module'. Returns kTRUE
    // if successful, kFALSE otherwise.

    // reset AcquDAQ config file
    if (!fADConfigFile)
    {
        // prepare environment key
        Char_t tmp[256];
        sprintf(tmp, "ARDAQ.Cfg.Mod.File.Prefix");

        // get template path from config file
        if (gEnv->Defined(tmp))
        {
            fADConfigPath = gEnv->GetValue(tmp, "null");
        }
        else
        {
            Error("OpenADConfig", "Could not find AcquDAQ configuration file path in cofiguration (key: '%s').", tmp);
            return kFALSE;
        }
    }
    else
    {
        // close file first
        if (fADConfigFile->is_open()) fADConfigFile->close();

        // delete file
        delete fADConfigFile;
        fADConfigFile = 0;
    }

    // set up path
    Char_t path[256];
    sprintf(path, "%s%02d.dat", fADConfigPath, module);

    // open template file
    fADConfigFile = new ofstream();
    fADConfigFile->open(path, ifstream::trunc);
    if (!fADConfigFile->is_open())
    {
        Error("OpenADConfig", "AcquDAQ configuration file '%s' could not be opened!", path);
        return kFALSE;
    }

    return kTRUE;
}

//______________________________________________________________________________
Bool_t TTWriteADConfig::RemoveADConfig(Int_t module)
{
    // Removes the AcquDAQ coniguration file for module 'module'.

    // set up path
    Char_t path[256];
    sprintf(path, "%s%02d", fADConfigPath, module);

    // remove the file
    gSystem->Unlink(path);

    // user info
    Info("RemoveADConfig", "Deleted AcquDAQ configuration file '%s' for module %02d.", path, module);

    return kTRUE;
}

