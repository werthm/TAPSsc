// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTNetClient                                                          //
//                                                                      //
// Network client class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTNetClient.h"

ClassImp(TTNetClient)


//______________________________________________________________________________
TTNetClient::TTNetClient(const Char_t* host, Int_t port)
{
    // Constructor.
    
    // init members
    fSocket = 0;

    // try to create client socket
    fSocket = new TSocket(host, port);
    if (!fSocket->IsValid())
    {
        Error("TTNetClient", "Could not open connection to '%s' on port %d", host, port);
        return;
    }
}

//______________________________________________________________________________
TTNetClient::~TTNetClient()
{
    // Destructor.

    if (fSocket)
    {
        fSocket->Close();
        delete fSocket;
    }
}

//______________________________________________________________________________
Int_t TTNetClient::GetStatus()
{
    // Return the connection status to the server.
    
    // check the socket
    if (!fSocket) return kNoConn;
    else
    {
        // check the connection
        if (!fSocket->IsValid()) return kNoConn;
        else
        {
            // check if the server is responding
            TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCStatus);
            
            // wait for the response
            if (fSocket->Select(TSocket::kRead, TTConfig::kStdNetTimeout) == 1)
            {
                // get response
                Char_t res[256];
                fSocket->Recv(res, 256);
                
                // check response
                Int_t nc = TTUtils::GetNetworkCmd(res);
                if (nc == TTConfig::kNCReady) return kReady;
                return kBadResp;
            }
            else return kNoResp;
        }
    }
}

