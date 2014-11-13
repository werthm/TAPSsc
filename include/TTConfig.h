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


#ifndef TTCONFIG_H
#define TTCONFIG_H

#include "Rtypes.h"

namespace TTConfig
{   
    // database format definitions
    extern const Char_t* kParTableFormat;
    extern const Char_t* kMapTableFormat;
 
    // version numbers etc.
    extern const Char_t kTAPSscVersion[];

    // networking
    extern const Int_t kTAPSServerPort;
    extern const Int_t kStdNetTimeout;
    extern const Int_t kHVNetTimeout;
    extern const Int_t kShortNetTimeout;
    extern const Int_t kLongNetTimeout;

    // network commands
    enum ETTNetCmd
    {
        kNCStatus,
        kNCStop,
        kNCType,
        kNCTypeRet,
        kNCReady,
        kNCReadHV,
        kNCReadHVFailed,
        kNCReadHVSuccess,
        kNCWriteHV,
        kNCWriteHVFailed,
        kNCWriteHVSuccess,
        kNCStatusHV,
        kNCStatusHVFailed,
        kNCStatusHVRet,
        kNCSetStatusHV,
        kNCSetStatusHVFailed,
        kNCSetStatusHVSuccess,
        kNCWriteARCfgBaF2,
        kNCWriteARCfgBaF2Failed,
        kNCWriteARCfgBaF2Success,
        kNCWriteARCfgVeto,
        kNCWriteARCfgVetoFailed,
        kNCWriteARCfgVetoSuccess,
        kNCWriteARCfgPWO,
        kNCWriteARCfgPWOFailed,
        kNCWriteARCfgPWOSuccess,
        kNCStartCalibQAC,
        kNCStartCalibQACSuccess,
        kNCStartCalibQACFailed,
        kNCStopCalibQAC,
        kNCStopCalibQACSuccess,
        kNCStopCalibQACFailed
    };
}

#endif

