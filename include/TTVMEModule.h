// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTVMEModule                                                          //
//                                                                      //
// Parent VME module class.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTVMEMODULE_H
#define TTVMEMODULE_H

#include <sys/mman.h>

#include "Rtypes.h"
#include "TError.h"


class TTVMEModule
{

private:
    Long_t fAdr;                // VME base address
    volatile void* fMap;        // memory map pointer
    Int_t fMapLen;              // memory map length

public:
    TTVMEModule() : fAdr(0),
                    fMap(0), 
                    fMapLen(0) { }
    TTVMEModule(Long_t adr, Int_t len);
    virtual ~TTVMEModule();
    
    Long_t GetAddress() const { return fAdr; }
    volatile void* GetMap() const { return fMap; }
    Int_t GetMapLength() const { return fMapLen; }
    
    void MapMemory(Long_t adr, Int_t desc);
    void UnMapMemory();
    
    UShort_t Read(Long_t reg);
    void Write(Long_t reg, UShort_t val);
    
    ClassDef(TTVMEModule, 0) // Parent VME module class
};

//______________________________________________________________________________
inline UShort_t TTVMEModule::Read(Long_t reg)
{
    if (fMap)
        return *((volatile UShort_t*)((Long_t)fMap + reg));
    else 
        return 0;
}

//______________________________________________________________________________
inline void TTVMEModule::Write(Long_t reg, UShort_t val)
{
    if (fMap)
    {
        *((volatile UShort_t*)((Long_t)fMap + reg)) = val;
    }
}

#endif

