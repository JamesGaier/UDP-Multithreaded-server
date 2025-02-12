#pragma once
#include "thread_pool.hpp"
#include "sock.hpp"

#include <atomic>
#include <mutex>
#include <thread>


namespace serv 
{

class UDPServer
{
public:
    constexpr static auto RECEIVE_FAILED_ERROR = "ERROR: failed to receive packet data from client...";
    
    UDPServer(const std::string &ip, int port);

    UDPServer() = delete;
    UDPServer(const UDPServer &) = delete;
    UDPServer &operator=(const UDPServer &) = delete;

    ~UDPServer();
    
    void handleClient();
    
    void recvLoop();

    void close();
private:
    UDPSocket m_socket;   
    std::atomic<bool> m_running;
    std::mutex m_mutex;
    std::jthread m_recvThread;
    std::atomic<int> m_curNumThreads;
};


}