// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTNetServer                                                          //
//                                                                      //
// Network server class.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTNetServer.h"

ClassImp(TTNetServer)


//______________________________________________________________________________
TTNetServer::TTNetServer(Int_t port)
{
    // Constructor.
    
    // init members
    fServer = 0;
    fIsRunning = kFALSE;

    // try to create server socket
    fServer = new TServerSocket(port, kTRUE);
    if (!fServer->IsValid())
    {
        Error("TTNetServer", "Could not open server socket on port %d", port);
        fServer = 0;
        return;
    }
    
    // set non-blocking socket
    fServer->SetOption(kNoBlock, 1);
}

//______________________________________________________________________________
TTNetServer::~TTNetServer()
{
    // Destructor.

    if (fServer)
    {
        fServer->Close();
        delete fServer;
    }
}

//______________________________________________________________________________
void TTNetServer::StartListening()
{
    // Start listening on the server socket.
    
    // check server status
    if (!fServer)
    {
        Error("StartListening", "Cannot listen to socket - server is not running!");
        return;
    }

    // set running flag
    fIsRunning = kTRUE;

    // listen to port
    while (1)
    {
        // check running status
        if (!fIsRunning) return;

        // accept connections via socket
        TSocket* socket = fServer->Accept();
        
        // check socket connection
        if (socket == (TSocket*)-1)
        {   
            // wait and continue to listen
            gSystem->Sleep(100);
            continue;
        }

        // accept message
        TMessage* mess;
        socket->Recv(mess);
        
        // skip empty message
        if (!mess)
        {
            delete socket;
            continue;
        }

        // interpret message
        if (mess->What() == kMESS_STRING)
        {
            Char_t cmd[256];
            mess->ReadString(cmd, 256);
            ProcessCommand(cmd, socket);
        }
        else if (mess->What() == kMESS_OBJECT)
        {
        }

        // clean-up
        delete mess;
        delete socket;
    }
}

//______________________________________________________________________________
void TTNetServer::StopListening()
{
    // Stop listening on the server socket.
    
    // check server status
    if (!fServer)
    {
        Error("StopListening", "Server is not running!");
        return;
    }
    
    // set running flag
    fIsRunning = kFALSE;
}

//______________________________________________________________________________
void TTNetServer::ProcessCommand(const Char_t* cmd, TSocket* s)
{
    // Process the command 'cmd' coming from the socket 's'.
    
    // STOP command: stop listening on socket
    if (!strcmp(cmd, "STOP"))
    {
        StopListening();
    }
    // STATUS command: return the server status
    else if (!strcmp(cmd, "STATUS"))
    {
        s->Send("READY");
    }
    else
    {
        Warning("ProcessCommand", "Unknown command '%s'", cmd);
    }
}

