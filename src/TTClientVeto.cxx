// SVN Info: $Id: TTClientVeto.cxx 1752 2013-11-06 16:49:35Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTClientVeto                                                         //
//                                                                      //
// Veto TAPS client class.                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TTClientVeto.h"

ClassImp(TTClientVeto)


//______________________________________________________________________________
TTClientVeto::TTClientVeto(const Char_t* host, Int_t port)
    : TTClient(host, port)
{
    // Constructor.
    
    // init members
    fCalibQAC = 0;
}

//______________________________________________________________________________
TTClientVeto::~TTClientVeto()
{
    // Destructor.
    
    if (fCalibQAC) delete fCalibQAC;
}

//______________________________________________________________________________
Bool_t TTClientVeto::WriteADConfig()
{
    // Write the AcquDAQ configuration files.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kVetoServer)
    {
        Error("WriteADConfig", "Cannot write Veto AcquDAQ configuration files on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send write AcquDAQ config file command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCWriteARCfgVeto);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, TTConfig::kLongNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCWriteARCfgVetoSuccess) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

//______________________________________________________________________________
Bool_t TTClientVeto::StartCalibQAC()
{
    // Start the QAC calibration.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kVetoServer)
    {
        Error("StartCalibQAC", "Cannot start QAC calibration on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send start QAC calibration command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCStartCalibQAC);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, TTConfig::kLongNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCStartCalibQACSuccess) return kTRUE;
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

//______________________________________________________________________________
Bool_t TTClientVeto::StopCalibQAC()
{
    // Stop the QAC calibration.
    // Return kTRUE on success, otherwise kFALSE.
    
    Char_t tmp[256];

    // check if server has the correct type and is connected
    if (GetType() != kVetoServer)
    {
        Error("StopCalibQAC", "Cannot stop QAC calibration on server '%s'!",
              GetHost().GetHostName());
        return kFALSE;
    }

    // send stop QAC calibration command
    TTUtils::SendNetworkCmd(fSocket, TTConfig::kNCStopCalibQAC);
    
    // wait for the response
    if (fSocket->Select(TSocket::kRead, TTConfig::kStdNetTimeout) == 1)
    {
        // get response
        fSocket->Recv(tmp, 256);
        
        // check response
        Int_t nc = TTUtils::GetNetworkCmd(tmp);
        if (nc == TTConfig::kNCStopCalibQACSuccess) 
        {
            // receive the QAC calibration
            if (fSocket->Select(TSocket::kRead, TTConfig::kStdNetTimeout) == 1)
            {
                // receive object
                TMessage* mes;
                fSocket->Recv(mes);

                // read object
                TTCalibQAC* calib = (TTCalibQAC*)mes->ReadObjectAny(TTCalibQAC::Class());
                
                // save new calibration
                if (fCalibQAC) delete fCalibQAC;
                fCalibQAC = calib;

                // clean-up
                delete mes;

                return kTRUE;
            }
        }
        else return kFALSE;
    }

    // no answer here
    return kFALSE;
}

