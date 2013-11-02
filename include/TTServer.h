// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTServer                                                             //
//                                                                      //
// Parent TAPS server class.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTSERVER_H
#define TTSERVER_H

#include "TTNetServer.h"


// server type enum
enum ETServerType {
    kNoServer,
    kBaF2Server,
    kVetoServer,
    kPWOServer,
    kHVServer
};
typedef ETServerType TServerType_t;


class TTServer : public TTNetServer
{

private:
    Int_t fID;                      // server ID
    TServerType_t fType;            // TAPS server type
    Long_t fVMERange;               // VME range

protected:
    virtual Bool_t ProcessCommand(const Char_t* cmd, TSocket* s);

public:
    TTServer() : TTNetServer(),
                 fID(-1), fType(kNoServer), fVMERange(0) { }
    TTServer(TServerType_t type, Int_t port, Int_t id);
    virtual ~TTServer();
    
    Int_t GetID() const { return fID; }
    TServerType_t GetType() const { return fType; }
    Long_t GetVMERange() const { return fVMERange; }

    void SetVMERange(Long_t r) { fVMERange = r; }

    ClassDef(TTServer, 0) // Parent TAPS server
};

#endif

