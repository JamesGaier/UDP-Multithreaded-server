#include "sock.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <optional>

namespace serv
{
    

void UDPSocket::makeSocket()
{
    m_socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socketfd < 0)
    {
        throw std::runtime_error(SOCKET_CREATION_FAILED);
    } 
}

UDPSocket::UDPSocket(const std::string &srcIp, int srcPort)
{
    memset(&m_sockAddr, 0, sizeof(m_sockAddr)); 
    m_sockAddr.sin_family = AF_INET;
    m_sockAddr.sin_addr.s_addr = inet_addr(srcIp.data());
    m_sockAddr.sin_port = htons(srcPort);
    makeSocket();
}


void UDPSocket::Bind()
{
    const auto sockAddr = (const sockaddr *)&m_sockAddr;
    const auto servAddrSize = sizeof(m_sockAddr);
    int bindFailed = bind(m_socketfd, sockAddr, servAddrSize);
    if (bindFailed < 0)
    {
        throw std::runtime_error(BIND_FAILED_ERROR);
    }
}

UDPSocket::PacketData UDPSocket::Recv()
{
    PacketData packetData;
    std::vector<char> buffer(P_SIZE);
    sockaddr_in sockAddrCpy(m_sockAddr);
    socklen_t length = sizeof(sockAddrCpy);
    auto sockAddr = (sockaddr *)&sockAddrCpy;
    int recvSuccess = recvfrom(
        m_socketfd, 
        buffer.data(), 
        P_SIZE,
        MSG_WAITALL, 
        sockAddr,
        &length
    );

    std::vector<char> srcIpBuf(P_SIZE);
    inet_ntop(AF_INET, &sockAddrCpy.sin_addr, srcIpBuf.data(), P_SIZE);
    std::string srcIpStr(srcIpBuf.begin(), srcIpBuf.end());
    packetData.ip = srcIpStr;
    packetData.port = ntohs(sockAddrCpy.sin_port);
    
    if (recvSuccess == -1)
    {
        packetData.data = {};
        std::cerr << "socket rcvfrom timed out" << std::endl;
        return packetData;
    }
    
    packetData.data = std::move(buffer);
    
    return packetData; 
}


void UDPSocket::Send(const std::vector<char> &bytes, const std::string &ip, int port)
{
    
    sockaddr_in dstAddr;
    memset(&dstAddr, 0, sizeof(dstAddr)); 
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_addr.s_addr = inet_addr(ip.data());
    dstAddr.sin_port = htons(port);

    auto sockAddr = (const struct sockaddr *)&dstAddr;
    socklen_t length = sizeof(dstAddr);
    sendto(
        m_socketfd,
        bytes.data(),
        bytes.size(),
        MSG_CONFIRM,
        sockAddr,
        length
    );
    std::cout << "Sending message to address" << std::endl;
}



void UDPSocket::setRecvTimeout(time_t timeoutSec, time_t timeoutMicro)
{
    struct timeval timeout;
    timeout.tv_sec = timeoutSec;
    timeout.tv_usec = timeoutMicro;
    int socketOptSuccess = setsockopt(m_socketfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    if (socketOptSuccess == -1)
    {
        throw std::runtime_error(TIMEOUT_ERROR);
    }
}

void UDPSocket::Close()
{
    close(m_socketfd);
}

}