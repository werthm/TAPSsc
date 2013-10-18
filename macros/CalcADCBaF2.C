// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CalcADC.C                                                            //
//                                                                      //
// Calculate BaF2 ADC numbers.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void CalcADCBaF2()
{
    // TAPS block start indices
    const Int_t idBlock[6]    = { 20000, 21000, 22000, 23000, 24000, 25000 };
    
    // module offsets (+32 per module)
    const Int_t offModule[16] = {   0,  32,  64,  96, 128, 160, 192, 224, 
                                  256, 288, 320, 352, 384, 416, 448, 480 };
    
    // register offsets
    const Int_t offLG[4]  = {  2,  6, 14, 18 };       // LG offsets
    const Int_t offSG[4]  = {  3,  7, 15, 19 };       // SG offsets
    const Int_t offLGS[4] = {  0,  4, 12, 16 };       // LGS offsets
    const Int_t offSGS[4] = {  1,  5, 13, 17 };       // SGS offsets
    const Int_t offTDC[4] = {  8,  9, 10, 11 };       // TDC offsets
    const Int_t offBP[2]  = { 20, 21 };               // Bit pattern offsets

    // loop over blocks
    for (Int_t b = 0; b < 6; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 16; m++)
        {
            // loop over channels
            for (Int_t c = 0; c < 4; c++)
            {
                // calculate LG, SG and TDC
                //Int_t lg  = idBlock[b] + offModule[m] + offLG[c];
                //Int_t sg  = idBlock[b] + offModule[m] + offSG[c];
                //Int_t tdc = idBlock[b] + offModule[m] + offTDC[c];
                //printf("%d      %d      %d\n", lg, sg, tdc);
                
                // calculate LGS, SGS and TDC
                Int_t lg  = idBlock[b] + offModule[m] + offLGS[c];
                Int_t sg  = idBlock[b] + offModule[m] + offSGS[c];
                Int_t tdc = idBlock[b] + offModule[m] + offTDC[c];
                printf("%d      %d      %d\n", lg, sg, tdc);
            }
        }
    }
    
    printf("\n\n");
    
    // loop over blocks
    for (Int_t b = 0; b < 6; b++)
    {
        // loop over modules
        for (Int_t m = 0; m < 16; m++)
        {
            // loop over pattern units
            for (Int_t c = 0; c < 2; c++)
            {
                // calculate pattern units
                Int_t pu = idBlock[b] + offModule[m] + offBP[c];

                printf("%d\n", pu);
            }
        }
    }

    gSystem->Exit(0);
}

