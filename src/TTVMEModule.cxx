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


#include "TTVMEModule.h"

ClassImp(TTVMEModule)


//______________________________________________________________________________
TTVMEModule::TTVMEModule(Long_t adr, Int_t len, Int_t nCh, Int_t nADC)
{
    // Constructor.
    
    // init members
    fAdr = adr;
    fMap = 0;
    fMapLen = len;
    fNCh = nCh;
    fNADC = nADC;
    fNData = 0;
    fADC = new UShort_t[fNADC];
    fData = new UShort_t[fNADC];
}

//______________________________________________________________________________
TTVMEModule::~TTVMEModule()
{
    // Destructor.

    if (fMap) UnMapMemory();
    if (fADC) delete [] fADC;
    if (fData) delete [] fData;
}

//______________________________________________________________________________
void TTVMEModule::MapMemory(Long_t adr, Int_t desc)
{
    // Map the virtual memory using the address 'adr' and the file descriptor
    // 'desc'.

    // unmap if already mapped
    if (fMap) UnMapMemory();

    // try to map
    fMap = mmap(NULL, fMapLen, PROT_READ | PROT_WRITE, MAP_SHARED, desc, adr);
    
    // check mapping
    if (fMap == (void*)-1)
    {
        fMap = 0;
        Error("MapMemory", "Could not create memory map of length 0x%x at 0x%lx!",
              fMapLen, adr);
    }
}

//______________________________________________________________________________
void TTVMEModule::UnMapMemory()
{
    // Unmap the virtual memory.
    
    // check for map
    if (!fMap) return;
    
    // try to unmap
    if (munmap((void*)fMap, fMapLen) != -1) 
        fMap = 0;
    else
        Error("UnMapMemory", "Could not unmap memory map!");
}

