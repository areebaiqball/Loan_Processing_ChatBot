#pragma once
#include "network_utils.h"
#include <iostream>

bool NetworkUtils::initializeNetwork() {
#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }
#endif
    return true;
}

void NetworkUtils::cleanupNetwork() {
#ifdef _WIN32
    WSACleanup();
#endif
}

std::string NetworkUtils::getErrorMessage() {
#ifdef _WIN32
    int errorCode = WSAGetLastError();
    return "Socket error code: " + std::to_string(errorCode);
#else
    return std::string(strerror(errno));
#endif
}