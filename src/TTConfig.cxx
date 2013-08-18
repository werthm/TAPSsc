// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTConfig                                                             //
//                                                                      //
// TAPSsc configuration namespace                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTConfig.h"


namespace TTConfig
{   
    // format of the parameter tables
    const Char_t* kParTableFormat =
                    "id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                    "elem INT,"
                    "value DOUBLE,"
                    "time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
                    "               ON UPDATE CURRENT_TIMESTAMP";
    
    // format of the map tables
    const Char_t* kMapTableFormat =
                    "id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                    "elem INT,"
                    "crate INT,"
                    "board INT,"
                    "channel INT,"
                    "time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
                    "               ON UPDATE CURRENT_TIMESTAMP";
    
    // version numbers
    const Char_t kTAPSscVersion[] = "0.0.1";

    // networking
    const Int_t kTAPSServerPort = 45000;
}

