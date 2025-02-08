#include "sock.hpp"
#include "thread_pool.hpp"

#include <thread>
#include <vector>
#include <chrono>
#include <condition_variable>
#include <random>

template <typename IntType>
IntType random(int lower, int upper)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dist(lower, upper);
    return dist(gen);
}

void send(serv::UDPSocket &sock, int port)
{
    std::string str = "Hello! " + std::to_string(port);
    std::vector<char> bytes(str.begin(), str.end());
    sock.Send(bytes, "127.0.0.1", 8080);
}

void sendMulti(int i)
{
    constexpr static int LOWER = 3'000;
    constexpr static int UPPER = 60'000;
    constexpr static auto LOCAL_HOST = "127.0.0.1";
    auto randPort = random<int>(LOWER, UPPER);
    serv::UDPSocket sock(LOCAL_HOST, randPort);
    
    send(sock, i);

    auto result = sock.Recv();
    
    auto data = result.data;
    if (!data)
    {
        sock.Close();
        std::cerr << "FAILD TO SEND " << randPort << std::endl;
        return;
    }

    sock.Close();
}


void send17()
{
    std::vector<std::jthread> senders;
    constexpr static auto NUM_THREADS = 17;
    
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        senders.emplace_back([=] {
            sendMulti(i);
    
        });
    }
}

int main()
{
    constexpr static int LOWER = 3'000;
    constexpr static int UPPER = 60'000;
    auto randPort = random<int>(LOWER, UPPER);
    serv::UDPSocket server("127.0.0.1", randPort);
    
    constexpr static int RECV_TIMEOUT_SEC = 60;
    server.setRecvTimeout(RECV_TIMEOUT_SEC,0);
    server.Bind();


    send(server, randPort);

    auto result = server.Recv();
    
    auto data = result.data;
    if (!data)
    {
        server.Close();
        throw std::runtime_error("ERROR: buffer empty");
    }
    
    std::cout << result << std::endl;
    
    server.Close();
    
    send17();
}