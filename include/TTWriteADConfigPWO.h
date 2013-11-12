// SVN Info: $Id$

/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfigPWO                                                   //
//                                                                      //
// Class to write the AcquDAQ cofiguration files for PWO modules from   //
// a template file and values from the MySQL database.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTWRITEADCONFIGPWO_H
#define TTWRITEADCONFIGPWO_H

#include "TTWriteADConfig.h"


class TTWriteADConfigPWO : public TTWriteADConfig
{

protected:
    virtual Bool_t ParseLine();

public:
    TTWriteADConfigPWO(const Int_t crate)
       : TTWriteADConfig(crate, "PWO", "Map.PWO", 16) { };
    virtual ~TTWriteADConfigPWO() {};

    ClassDef(TTWriteADConfigPWO, 0) // Write AcquDAQ PWO configuration files
};

#endif

