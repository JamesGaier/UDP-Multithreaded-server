#include "udp_server.hpp"

#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <csignal>
#include <condition_variable>
#include <atomic>
#include <mutex>


std::atomic<bool> running = true;
std::condition_variable progCv;
std::mutex mutex;

void signalHandler(int sigNum)
{
    std::cout << "SIGINT Ctrl + C was pressed." << std::endl;
    
    progCv.notify_all();
    running = false;
    exit(sigNum);
}

int main()
{
    using namespace std::chrono_literals;
    signal(SIGINT, signalHandler);
    serv::UDPServer server("127.0.0.1", 8080);
    
    std::unique_lock<std::mutex> ul(mutex);
    progCv.wait(ul, [&] { return !running.load(); });
    
    server.close();
}