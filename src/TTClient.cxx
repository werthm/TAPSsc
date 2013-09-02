// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClient                                                             //
//                                                                      //
// TAPS client class.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClient.h"

ClassImp(TTClient)


//______________________________________________________________________________
TTClient::TTClient(const Char_t* host, Int_t port)
    : TTNetClient(host, port)
{
    // Constructor.
    
}

//______________________________________________________________________________
TTClient::~TTClient()
{
    // Destructor.

}

//______________________________________________________________________________
TServerType_t TTClient::GetType()
{
    // Return the type of the connected server.
    
    // check the socket
    if (!fSocket) return kNoServer;
    else
    {
        // check the connection
        if (!fSocket->IsValid()) return kNoServer;
        else
        {
            // get the server type
            fSocket->Send("TYPE");
            
            // wait for the response
            if (fSocket->Select(TSocket::kRead, 2000) == 1)
            {
                // get response
                Char_t res[256];
                fSocket->Recv(res, 256);
                
                // check response
                TServerType_t t = (TServerType_t) atoi(res);
                if (t == kNoServer) return t;
                else if (t == kBaF2Server) return t;
                else if (t == kVetoServer) return t;
                else if (t == kPWOServer) return t;
                else if (t == kHVServer) return t;
                else return kNoServer;
            }
            else return kNoServer;
        }
    }
}

