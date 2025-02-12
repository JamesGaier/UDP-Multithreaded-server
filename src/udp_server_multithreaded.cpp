#include "udp_server_multithreaded.hpp"

#include <chrono>

namespace serv {

UdpServerMultiThreaded::UdpServerMultiThreaded(const std::string &ip, int port, int numThreads)
: m_threadPool(numThreads)
, m_sock(ip, port)
, m_running(true)
{
    m_sock.Bind();
    m_recvThread = std::jthread(&UdpServerMultiThreaded::recvLoop, this);
}


UdpServerMultiThreaded::~UdpServerMultiThreaded()
{
    close();
}

void UdpServerMultiThreaded::recvLoop()
{
    while (m_running)
    {
        UDPSocket::PacketData packetData;
        {
            std::lock_guard<std::mutex> lg(m_mutex);
            packetData = m_sock.Recv();
        }
        
        auto data = packetData.data;
        if (!data)
            continue;
        

        using namespace std::chrono_literals;
        std::cout << "Adding task to queue" << std::endl;
        m_threadPool.addTask([=] {
            std::string toPrint = "Server ran job for 60s and got: ";
            std::string dataToStr(data->begin(), data->end());
            toPrint += dataToStr;
            std::this_thread::sleep_for(60s);
            std::cout << toPrint << std::endl;
        });
    }
}

void UdpServerMultiThreaded::close()
{
    m_running = false;
    m_sock.Close();
}
    
}