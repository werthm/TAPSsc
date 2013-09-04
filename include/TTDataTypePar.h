// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTDataTypePar                                                        //
//                                                                      //
// Parameter data type class.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTDATATYPEPAR_H
#define TTDATATYPEPAR_H

#include "TTDataType.h"


class TTDataTypePar : public TTDataType
{

private:
    Double_t fMin;                  // minimum value of parameter
    Double_t fMax;                  // maximum value of parameter

public:
    TTDataTypePar() : TTDataType() { }
    TTDataTypePar(const Char_t* name, const Char_t* title, 
                  Int_t nSize);
    virtual ~TTDataTypePar() { }
 
    Double_t GetMin() const { return fMin; }
    Double_t GetMax() const { return fMax; }

    void SetMin(Double_t min) { fMin = min; }
    void SetMax(Double_t max) { fMax = max; }
    
    virtual void Print();
    
    ClassDef(TTDataTypePar, 1) // Parameter data type class
};

#endif

