#pragma once
#include "safe_queue.hpp"
#include "sock.hpp"
#include "thread_pool.hpp"

#include <atomic>
#include <mutex>
#include <functional>
#include <thread>


namespace serv {

class UdpServerMultiThreaded
{
public:
    using Task = std::function<void()>;
    ~UdpServerMultiThreaded();
    UdpServerMultiThreaded(const std::string &ip, int port, int numThreads);
    
    void close();
private:
    ThreadPool m_threadPool;
    UDPSocket m_sock;
    std::mutex m_mutex;
    std::atomic<bool> m_running;
    std::jthread m_recvThread;

    void recvLoop();
    
    
    UdpServerMultiThreaded() = delete;
    UdpServerMultiThreaded(const  UdpServerMultiThreaded&) = delete;
    UdpServerMultiThreaded &operator=(const UdpServerMultiThreaded &) = delete; 


  
};

}