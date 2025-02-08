#include "sock.hpp"
#include "thread_pool.hpp"

#include <thread>
#include <vector>
#include <chrono>

int main()
{
    using namespace std::chrono_literals;
    // serv::UDPSocket server("127.0.0.1", 8080);
    // server.setRecvTimeout(3, 0);
    // server.Bind();
    // server.Recv("127.0.0.1", 8080);
    // server.Close();
    serv::ThreadPool tp(10);
    tp.start();
    for (int i = 0; i < 100; ++i)
    {
        tp.addTask([=] {
            std::cout << "Task " << i << " starting..." << std::endl;
            std::this_thread::sleep_for(100ms);
            std::cout << "Task done!" << std::endl;
        });
    }
    
    std::this_thread::sleep_for(2s);
    tp.stop();
}