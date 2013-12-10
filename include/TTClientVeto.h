/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientVeto                                                         //
//                                                                      //
// Veto TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTCLIENTVETO_H
#define TTCLIENTVETO_H

#include "TTClient.h"
#include "TTCalibQAC.h"


class TTClientVeto : public TTClient
{

private:
    TTCalibQAC* fCalibQAC;          // QAC pedestal calibration

public:
    TTClientVeto() : TTClient(),
                     fCalibQAC(0) { }
    TTClientVeto(const Char_t* server, Int_t port);
    virtual ~TTClientVeto();
 
    TTCalibQAC* GetCalibQAC() const { return fCalibQAC; }

    Bool_t WriteADConfig();
    Bool_t StartCalibQAC();
    Bool_t StopCalibQAC();

    ClassDef(TTClientVeto, 0) // Veto TAPS client
};

#endif

