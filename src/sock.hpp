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

#include <vector>
#include <ostream>

namespace serv 
{

class UDPSocket
{
public:
    constexpr static int P_SIZE = 1024;
    constexpr static auto SOCKET_CREATION_FAILED = "socket creation failed.";
    constexpr static auto BIND_FAILED_ERROR = "bind failed.";
    constexpr static char NULL_TERM = '\0';
    constexpr static auto TIMEOUT_ERROR = "receive timeout socket option was not able to be set!";
    constexpr static auto MALFORMED_PACKET = "packet data malformed";
    
    using BufferOpt = std::optional<std::vector<char>>;
    struct PacketData
    {
        BufferOpt data;
        std::string ip;
        int port;
        
        friend std::ostream &operator<<(std::ostream &os, PacketData &packetData)
        {
            os << "src url: udp://" << packetData.ip << ":" << std::to_string(packetData.port) << "\n";
            os << "data: \n ";
            auto data = packetData.data;
            if (!data)
            {
                os << MALFORMED_PACKET;
                return os;
            }
            std::string str(data->begin(), data->end());
            os << str << "\n";
            return os;
        }
    };

    
    
    UDPSocket(const std::string &srcIp, int srcPort);
    
    // c lib has same name upper case disambiguates
    void Bind();
    
    PacketData Recv();
    
    void Send(const std::vector<char> &bytes, const std::string &ip, int port);
    
    void Close();
    
    void setRecvTimeout(time_t timeoutSec, time_t timeoutMicro);

private:
    int m_socketfd{};
    sockaddr_in m_sockAddr;
    
    void makeSocket();
    // sockaddr_in makeSockConfig(const std::string &ip, int port);
    void printBuffer();
};


}