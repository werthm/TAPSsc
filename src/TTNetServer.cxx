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
    //fServer->SetOption(kNoBlock, 1);
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
    
    // create monitor to watch network sockets
    TMonitor* mon = new TMonitor();
    mon->Add(fServer);
    
    // create list for network sockets
    TList* sockets = new TList();
    sockets->SetOwner(kTRUE);

    // listen to port
    while (1)
    {
        // check running status
        if (!fIsRunning) break;
        
        // remove inactive sockets from monitor, close and destroy them
        for (Int_t i = 0; i < sockets->GetSize(); i++) 
        {
            TSocket* t = (TSocket*)sockets->At(i);
            if (!t->IsValid())
            {
                mon->Remove(t);
                t->Close();
                sockets->Remove(t);
                delete t;
            }
        }

        // wait for connections via socket
        TSocket* s = mon->Select(100);
        
        // check socket connection
        if (s == (TSocket*)-1) continue;
        
        // accept new connections
        if (s->IsA() == TServerSocket::Class())
        {
            // get socket
            TSocket* sn = ((TServerSocket*) s)->Accept();
            
            // add socket to monitor
            mon->Add(sn);
            
            // add socket to list
            sockets->Add(sn);
            
            continue;
        }

        // accept message
        TMessage* mess;
        s->Recv(mess);
        
        // skip empty message
        if (!mess) continue;

        // interpret message
        if (mess->What() == kMESS_STRING)
        {
            Char_t cmd[256];
            mess->ReadString(cmd, 256);
            ProcessCommand(cmd, s);
        }
        else if (mess->What() == kMESS_OBJECT)
        {
        }

        // clean-up
        delete mess;
    }

    // remove sockets from monitor and close them
    for (Int_t i = 0; i < sockets->GetSize(); i++) 
    {
        TSocket* t = (TSocket*)sockets->At(i);
        mon->Remove(t);
        t->Close();
    }

    // clean-up of monitor
    mon->Remove(fServer);
    delete mon;

    // clean-up of sockets and socket list
    delete sockets;
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

