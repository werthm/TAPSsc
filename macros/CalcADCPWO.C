// SVN Info: $Id: CalcADCPWO.C 1750 2013-11-04 18:12:20Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CalcADCPWO.C                                                         //
//                                                                      //
// Calculate PWO ADC numbers.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void CalcADCPWO()
{
    // TAPS block start indices
    const Int_t idBlock[1]    = { 29000 };
    
    // module offsets (+32 per module)
    const Int_t offModule[5] = { 0, 32, 64, 96, 128 };
    
    // TDC offset (PWO TDCs start in input B of Caen V1190A)
    Int_t tdc = 29192 + 32;

    // loop over blocks
    for (Int_t b = 0; b < 1; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 5; m++)
        {
            // loop over channels
            for (Int_t c = 0; c < 16; c++)
            {
                // only 8 channels in the last module
                if (m == 4 && c == 8) break;

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

