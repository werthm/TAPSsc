// SVN Info: $Id$

/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfigVeto                                                  //
//                                                                      //
// Class to write the AcquDAQ cofiguration files for Veto modules from  //
// a template file and values from the MySQL database.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTWRITEADCONFIGVETO_H
#define TTWRITEADCONFIGVETO_H

#include "TTWriteADConfig.h"


class TTWriteADConfigVeto : public TTWriteADConfig
{

protected:
    virtual Bool_t ParseLine();

public:
    TTWriteADConfigVeto(const Int_t crate)
       : TTWriteADConfig(crate, "Veto") { };
    virtual ~TTWriteADConfigVeto() {};

    ClassDef(TTWriteADConfigVeto, 0) // Write AcquDAQ Veto configuration files
};

#endif

