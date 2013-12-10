/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CalcADCVeto.C                                                        //
//                                                                      //
// Calculate Veto ADC numbers.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void CalcADCVeto()
{
    // TAPS block start indices
    const Int_t idBlock[3]    = { 26000, 27000, 28000 };
    
    // module offsets (+32 per module)
    const Int_t offModule[16] = {   0,  32,  64,  96, 128, 160, 192, 224, 
                                  256, 288, 320, 352, 384, 416, 448, 480 };
    
    // register offsets
    const Int_t offADC[8] = { 8, 9, 10, 11, 12, 13, 14, 15 };     // ADC offsets
    const Int_t offTDC[8] = { 0, 4,  1,  5,  2,  6,  3,  7 };     // TDC offsets
    const Int_t offBP[2]  = { 16, 17 };                           // Bit pattern offsets

    // loop over blocks
    for (Int_t b = 0; b < 3; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 16; m++)
        {
            // loop over channels
            for (Int_t c = 0; c < 8; c++)
            {
                // calculate ADC and TDC
                Int_t adc = idBlock[b] + offModule[m] + offADC[c];
                Int_t tdc = idBlock[b] + offModule[m] + offTDC[c];
                printf("%d      %d\n", adc, tdc);
            }
        }
    }
    
    printf("\n\n");
    
    // loop over blocks
    for (Int_t b = 0; b < 3; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 16; m++)
        {
            // loop over pattern units (BP[1] is not used)
            for (Int_t c = 0; c < 1; c++)   
            {
                // calculate pattern units
                Int_t pu = idBlock[b] + offModule[m] + offBP[c];

                printf("%d\n", pu);
            }
        }
    }


    gSystem->Exit(0);
}

