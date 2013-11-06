// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServerManager                                                      //
//                                                                      //
// This class handles all the communication with the TAPS servers.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVERMANAGER_H
#define TTSERVERMANAGER_H

#include "TEnv.h"
#include "TError.h"

#include "TTConfig.h"
#include "TTClientBaF2.h"
#include "TTClientVeto.h"
#include "TTClientPWO.h"
#include "TTClientHV.h"


class TTServerManager
{
    
private:
    Int_t fNServer;                             // number of servers
    TTClient** fServer;                         // servers
    TList* fServerBaF2;                         // list of BaF2 servers
    TList* fServerVeto;                         // list of Veto servers
    TList* fServerPWO;                          // list of PWO servers
    TTClientHV* fServerHV;                      // single HV server
    Bool_t fSilence;                            // silence mode toggle
    static TTServerManager* fgServerManager;    // pointer to static instance of this class
    
    Bool_t RegisterServers();

public:
    TTServerManager();
    virtual ~TTServerManager();
 
    void SetSilenceMode(Bool_t s) { fSilence = s; }
    
    Bool_t ReadHV(TTDataTypePar* d, Int_t elem, Int_t* outHV);
    Bool_t WriteHV(TTDataTypePar* d, Int_t elem);
    Bool_t IsConnectedToHV();
    Bool_t GetStatusHV(Int_t mainframe, Bool_t* outSt);
    Bool_t SetStatusHV(Int_t mainframe, Bool_t status);
    
    Bool_t WriteADConfigBaF2();
    Bool_t WriteADConfigVeto();

    Bool_t StartCalibQACBaF2();
    Bool_t StartCalibQACVeto();
    Bool_t StopCalibQACBaF2();
    Bool_t StopCalibQACVeto();

    void PrintStatus();

    static TTServerManager* GetManager()
    {
        // return a pointer to the static instance of this class
        if (!fgServerManager) fgServerManager = new TTServerManager();
        return fgServerManager;
    }
    
    ClassDef(TTServerManager, 0) // Communication with TAPS servers
};

#endif

