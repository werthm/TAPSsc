// SVN Info: $Id: htmldoc.C 1661 2013-08-18 21:32:15Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
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

    THtml h;
    h.SetInputDir(".:src:include");
    h.SetOutputDir("htmldoc");
    h.SetAuthorTag("* Author:");
    h.SetProductName("TAPSsc");

    h.MakeAll();
}

