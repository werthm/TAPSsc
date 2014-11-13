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


#include <fcntl.h>
#include <sys/mman.h>

#include "TError.h"

#include "TTVMEKPh.h"
#include "TTVMEModule.h"

ClassImp(TTVMEKPh)

//______________________________________________________________________________
TTVMEKPh::TTVMEKPh(Long_t adrRange)
{
    // Constructor.
    
    // init members
    fDesc = -1;
    fNModule = 0;
    fModule = 0;

    // try to open /dev/mem
    fDesc = open("/dev/mem", O_RDWR);
    if (fDesc == -1)
    {
        Error("TTVMEKPh", "Could not open /dev/mem!");
        return;
    }

    // open temporary memory map
    volatile void* v = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fDesc, 0xaa000000);
    if (v == (void*)-1)
    {
        Error("TTVMEKPh", "Could not create memory map!");
        close(fDesc);
        fDesc = -1;
        return;
    }
    
    // set highest 3 bits for 32 bit addressing
    volatile Long_t* vl = (volatile Long_t*) v;
    *vl = 0xe0000000 & adrRange;
    if (munmap((void*)v, 0x1000) == -1)
    {
        Error("TTVMEKPh", "Could not unmap memory map!");
        close(fDesc);
        fDesc = -1;
        return;
    }
}

//______________________________________________________________________________
TTVMEKPh::~TTVMEKPh()
{
    // Destructor.
    
    if (fModule)
    {
        for (Int_t i = 0; i < fNModule; i++) delete fModule[i];
        delete [] fModule;
    }
    if (fDesc != -1) close(fDesc);
}

//______________________________________________________________________________
void TTVMEKPh::AddModule(TTVMEModule* m)
{
    // Add the VME module 'm' to the list of modules and perform the virtual
    // memory mapping.
    
    // leave if /dev/mem is not open
    if (fDesc == -1) return;

    // open map in 32-bit access
    Long_t a = m->GetAddress() & 0x1fffffff;
    a |= 0x80000000;
    
    // map memory
    m->MapMemory(a, fDesc);
    
    // first module
    if (!fModule)
    {
        fNModule = 1;
        fModule = new TTVMEModule*[fNModule];
        fModule[0] = m;
    }
    else
    {
        // backup old list
        TTVMEModule* old[fNModule];
        for (Int_t i = 0; i < fNModule; i++) old[i] = fModule[i];

        // destroy old list
        delete [] fModule;
    
        // create new list
        fModule = new TTVMEModule*[fNModule+1];
        for (Int_t i = 0; i < fNModule; i++) fModule[i] = old[i];

        // update new list
        fModule[fNModule] = m;
        fNModule++;
    }
}

