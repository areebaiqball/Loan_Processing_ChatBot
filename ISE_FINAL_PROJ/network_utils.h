#pragma once
#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET SocketType;
#define CLOSE_SOCKET closesocket
#define SOCKET_ERROR_CODE WSAGetLastError()
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int SocketType;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define CLOSE_SOCKET close
#define SOCKET_ERROR_CODE errno
#endif

namespace NetworkConfig {
    const int PORT = 8080;
    const std::string SERVER_IP = "127.0.0.1";
    const int BUFFER_SIZE = 4096;
    const int MAX_CONNECTIONS = 5;
}

class NetworkUtils {
public:
    static bool initializeNetwork();
    static void cleanupNetwork();
    static std::string getErrorMessage();
};

#endif