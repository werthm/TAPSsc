// SVN Info: $Id: TTWriteADConfigBaF2.h 1754 2013-11-12 16:57:24Z werthm $

/*************************************************************************
 * Author: Thomas Strub, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTWriteADConfigBaF2                                                  //
//                                                                      //
// Class to write the AcquDAQ cofiguration files for BaF2 modules from  //
// a template file and values from the MySQL database.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTWRITEADCONFIGBAF2_H
#define TTWRITEADCONFIGBAF2_H

#include "TTWriteADConfig.h"


class TTWriteADConfigBaF2 : public TTWriteADConfig
{

protected:
    virtual Bool_t ParseLine();

public:
    TTWriteADConfigBaF2(const Int_t crate)
       : TTWriteADConfig(crate, "BaF2", "Map.BaF2", 4) { };
    virtual ~TTWriteADConfigBaF2() {};

    ClassDef(TTWriteADConfigBaF2, 0) // Write AcquDAQ BaF2 configuration files
};

#endif

