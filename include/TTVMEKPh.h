// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMEKPh                                                             //
//                                                                      //
// VME access class via the Mainz Kernph Pentium M based VMEbus SBC     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMEKPH_H
#define TTVMEKPH_H

#include <fcntl.h>

#include "TTVMEModule.h"


class TTVMEKPh
{

private:
    Int_t fDesc;                    // file descriptor for /dev/mem
    Int_t fNModule;                 // number of modules
    TTVMEModule** fModule;          // array of modules (owned)

public:
    TTVMEKPh() : fDesc(-1),
                 fNModule(0),
                 fModule(0) { }
    TTVMEKPh(Long_t adrRange);
    virtual ~TTVMEKPh();
    
    void AddModule(TTVMEModule* m);
 
    TTVMEModule* GetModule(Int_t i) const { return fModule[i]; }

    ClassDef(TTVMEKPh, 0) // VME access via Kernph controller
};

#endif

