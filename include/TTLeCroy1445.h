// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTLeCroy1445                                                         //
//                                                                      //
// RS232 communication class for LeCroy 1445 high voltage controller.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTLECROY1445_H
#define TTLECROY1445_H

#include "TTRS232.h"


class TTLeCroy1445 : public TTRS232
{

public:
    TTLeCroy1445() : TTRS232() { }
    TTLeCroy1445(const Char_t* device);
    virtual ~TTLeCroy1445() { }
    
    virtual void Configure();
    void PrintCmdHelp();

    ClassDef(TTLeCroy1445, 0) // LeCroy 1445 communication
};

#endif

