/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClient                                                             //
//                                                                      //
// Parent TAPS client class.                                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClient.h"

ClassImp(TTClient)


//______________________________________________________________________________
TTClient::TTClient(const Char_t* host, Int_t port)
    : TTNetClient(host, port), TObject()
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
            TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCType);
            
            // wait for the response
            if (fSocket->Select(TSocket::kRead, TTConfig::kStdNetTimeout) == 1)
            {
                // get response
                Char_t res[256];
                fSocket->Recv(res, 256);
                
                // check response
                Int_t nc = TTUtils::GetNetworkCmd(res);
                if (nc == TTConfig::kNCTypeRet)
                {
                    Int_t type;
                    sscanf(res, "%*d %d", &type);
                    TServerType_t t = (TServerType_t) type;
                    if (t == kNoServer) return t;
                    else if (t == kBaF2Server) return t;
                    else if (t == kVetoServer) return t;
                    else if (t == kPWOServer) return t;
                    else if (t == kHVServer) return t;
                    else return kNoServer;
                }
                else return kNoServer;
            }
            else return kNoServer;
        }
    }
}

