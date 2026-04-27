// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "MavLinkConnection.hpp"
#include "Utils.hpp"

#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <glob.h>

using namespace mavlinkcom;
using namespace mavlinkcom_impl;

std::vector<SerialPortInfo> MavLinkConnection::findSerialPorts(int vid, int pid)
{
    unused(vid); // avoid warning: unused parameter
    unused(pid); // avoid warning: unused parameter
    
    std::vector<SerialPortInfo> ports;
    
    // On macOS, serial ports typically show up as /dev/cu.*
    // We use glob to find them.
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    // Common patterns for USB serial on Mac
    int return_value = glob("/dev/cu.*", GLOB_TILDE, NULL, &glob_result);
    
    if(return_value == 0) {
        for(size_t i = 0; i < glob_result.gl_pathc; ++i) {
            std::string port_path = glob_result.gl_pathv[i];
            
            SerialPortInfo portInfo;
            portInfo.pid = 0;
            portInfo.vid = 0;
            portInfo.portName = std::wstring(port_path.begin(), port_path.end());
            portInfo.displayName = std::wstring(port_path.begin(), port_path.end());
            
            ports.push_back(portInfo);
        }
    }
    
    globfree(&glob_result);

    return ports;
}
