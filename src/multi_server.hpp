#include "sock.hpp"


namespace serv 
{

class MultithreadedServer
{
public:
    // MultithreadedServer(int numThreads);
private:
    UDPSocket m_socket;   
    ThreadPool m_threadPool;
};


}