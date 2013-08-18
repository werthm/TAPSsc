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
#include "TTNetClient.h"


class TTServerManager
{
    
private:
    Int_t fNServer;                             // number of servers
    TTNetClient** fServer;                      // servers
    Bool_t fSilence;                            // silence mode toggle
    static TTServerManager* fgServerManager;    // pointer to static instance of this class
    
    Bool_t RegisterServers();

public:
    TTServerManager();
    virtual ~TTServerManager();
    
    void SetSilenceMode(Bool_t s) { fSilence = s; }
    
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

