/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientBaF2                                                         //
//                                                                      //
// BaF2 TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENTBAF2_H
#define TTCLIENTBAF2_H

#include "TTClient.h"

class TTCalibQAC;

class TTClientBaF2 : public TTClient
{

private:
    TTCalibQAC* fCalibQAC;          // QAC pedestal calibration

public:
    TTClientBaF2() : TTClient(),
                     fCalibQAC(0) { }
    TTClientBaF2(const Char_t* server, Int_t port);
    virtual ~TTClientBaF2();
 
    TTCalibQAC* GetCalibQAC() const { return fCalibQAC; }

    Bool_t WriteADConfig();
    Bool_t StartCalibQAC();
    Bool_t StopCalibQAC();

    ClassDef(TTClientBaF2, 0) // BaF2 TAPS client
};

#endif

