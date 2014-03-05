/*************************************************************************
 * Author: Dominik Werthmueller, 2014
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// htmldoc.C                                                            //
//                                                                      //
// ROOT macro for the creation of the HTML documentation.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void htmldoc()
{
    gROOT->Reset();
    if (!gROOT->GetClass("TTMySQLManager")) gSystem->Load("libTAPSsc.so");
    
    gEnv->SetValue("Unix.*.Root.Html.SourceDir", "src:include");
 
    THtml h;
    h.SetOutputDir("htmldoc");
    h.SetAuthorTag("* Author:");
    h.SetProductName("TAPSsc");

    h.MakeAll();
}

