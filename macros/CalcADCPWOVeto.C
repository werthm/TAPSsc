// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CalcADCPWOVeto.C                                                     //
//                                                                      //
// Calculate PWO Veto ADC numbers.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void CalcADCPWOVeto()
{
    // TAPS block start indices
    const Int_t idBlock[1]    = { 29128 };
    
    // module offsets (+32 per module)
    const Int_t offModule[2] = { 0, 32 };
    
    // TDC offset (PWO-Veto TDCs start in input D of Caen V1190A)
    Int_t tdc = 29192 + 3*32;

    // TDC hardware channels
    Int_t tdcHW[18] = {  8,  9, 10,   11, 12, 13, 
                        16, 17, 18,   19, 20, 21,
                        24, 25, 26,   27, 28, 29 };

    // loop over blocks
    Int_t elem = 0;
    for (Int_t b = 0; b < 1; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 2; m++)
        {
            // loop over channels
            for (Int_t c = 0; c < 16; c++)
            {
                // only last 2 channels in the first module
                if (m == 0 && c < 14) continue;

                // calculate ADC, ADCS and TDC
                Int_t adc  = idBlock[b] + offModule[m] + 2*c;
                Int_t adcs = idBlock[b] + offModule[m] + 2*c+1;
                printf("%d      %d      %d\n", adc, adcs, tdc + tdcHW[elem++]);
            }
        }
    }
    
    gSystem->Exit(0);
}

