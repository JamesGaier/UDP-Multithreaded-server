#pragma once

// Server side implementation of UDP client-server model 
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#include <array>

namespace serv 
{


class UDPSocket
{
public:
    constexpr static uint16_t P_SIZE = 1024;
    constexpr static auto SOCKET_CREATION_FAILED = "socket creation failed.";
    constexpr static auto BIND_FAILED_ERROR = "bind failed.";
    constexpr static char NULL_TERM = '\0';
    constexpr static auto TIMEOUT_ERROR = "receive timeout socket option was not able to be set!";
    
    UDPSocket(const std::string &srcIp, int srcPort);
    
    // c lib has same name upper case disambiguates
    void Bind();
    
    void Recv(const std::string &ip, int port);
    
    void Send(const std::vector<char> &bytes, const std::string &ip, int port);
    
    void Close();
    
    void setRecvTimeout(time_t timeoutSec, time_t timeoutMicro);

private:
    std::array<char, P_SIZE> m_buffer;
    int m_socketfd{};
    sockaddr_in m_sockAddr;
    
    void makeSocket();
    // sockaddr_in makeSockConfig(const std::string &ip, int port);
    void printBuffer();
};


}