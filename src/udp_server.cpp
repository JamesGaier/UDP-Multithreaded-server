#include "udp_server.hpp"
#include "sock.hpp"


namespace serv 
{

UDPServer::UDPServer(const std::string &ip, int port)
: m_socket(ip, port)
, m_running(true)
{
    m_socket.Bind();
    m_socket.setRecvTimeout(3, 0);
    m_recvThread = std::jthread(&UDPServer::recvLoop, this);
}

UDPServer::~UDPServer()
{
    close();
}

void UDPServer::handleClient()
{
}
    
void UDPServer::recvLoop()
{
    while (m_running)
    {
        UDPSocket::PacketData packet;
        {
            std::lock_guard<std::mutex> lg(m_mutex);
            packet = m_socket.Recv();
        }

        auto data = packet.data;
        if (!data)
        {
            std::cerr << RECEIVE_FAILED_ERROR << std::endl;
            continue;
        }
        
        std::string request(data->begin(), data->end());
        std::cout << "GOT MESSAGE: " <<  request << std::endl;
        std::string responseStr = "Hi!!!";
        std::vector<char> response(responseStr.begin(), responseStr.end());
        {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_socket.Send(response, packet.ip, packet.port);
        }
    }
}

void UDPServer::close()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    m_socket.Close();
    m_running = false;
}

}