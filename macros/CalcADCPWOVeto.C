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
    
    // TDC offset
    Int_t tdc = 29193 + 72;

    // loop over blocks
    for (Int_t b = 0; b < 1; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 2; m++)
        {
            // loop over channels
            for (Int_t c = 0; c < 16; c++)
            {
                // only 2 channels in the first module
                if (m == 0 && ((c < 11) || (c > 12))) continue;

                // calculate ADC, ADCS and TDC
                Int_t adc  = idBlock[b] + offModule[m] + 2*c;
                Int_t adcs = idBlock[b] + offModule[m] + 2*c+1;
                printf("%d      %d      %d\n", adc, adcs, tdc);
                tdc++;
            }
        }
    }
    
    gSystem->Exit(0);
}

