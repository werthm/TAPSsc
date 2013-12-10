// SVN Info: $Id: TTVMEModule.h 1749 2013-11-04 01:26:31Z werthm $

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

protected:
    Int_t fNCh;                 // number of channels
    Int_t fNADC;                // number of ADCs
    Int_t fNData;               // number of read data
    UShort_t* fADC;             // ADC array
    UShort_t* fData;            // data array

public:
    TTVMEModule() : fAdr(0),
                    fMap(0), 
                    fMapLen(0), 
                    fNCh(0), fNADC(0),
                    fNData(0), fADC(0), fData(0) { }
    TTVMEModule(Long_t adr, Int_t len, Int_t nCh, Int_t nADC);
    virtual ~TTVMEModule();
    
    Long_t GetAddress() const { return fAdr; }
    volatile void* GetMap() const { return fMap; }
    Int_t GetMapLength() const { return fMapLen; }
    Int_t GetNData() const { return fNData; }
    UShort_t* GetADC() const { return fADC; }
    UShort_t* GetData() const { return fData; }

    void MapMemory(Long_t adr, Int_t desc);
    void UnMapMemory();
       
    UShort_t Read(Long_t reg = 0);
    UInt_t Read32(Long_t reg = 0);
    void Write(Long_t reg, UShort_t val);
    
    void ClearData();

    ClassDef(TTVMEModule, 0) // Parent VME module class
};

//______________________________________________________________________________
inline UShort_t TTVMEModule::Read(Long_t reg)
{
    // VME 16-bit read function.

    if (fMap)
        return *((volatile UShort_t*)((Long_t)fMap + reg));
    else 
        return 0;
}

//______________________________________________________________________________
inline UInt_t TTVMEModule::Read32(Long_t reg)
{
    // VME 32-bit read function.

    if (fMap)
        return *((volatile UInt_t*)((Long_t)fMap + reg));
    else 
        return 0;
}

//______________________________________________________________________________
inline void TTVMEModule::Write(Long_t reg, UShort_t val)
{
    // VME 16-bit write function.

    if (fMap)
    {
        *((volatile UShort_t*)((Long_t)fMap + reg)) = val;
    }
}

#endif

