#include "safe_queue.hpp"
#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable>
#include <iostream>


namespace serv 
{

class ThreadPool
{
public:    
    using Task = std::function<void()>;
    ThreadPool(int numThreads);
    
    void addTask(Task task);
    
    void processTask();

    void start();

    void stop();

private:
    std::vector<std::thread> m_threads;
    ThreadSafeQueue<Task> m_queue;
    std::atomic<bool> m_running;
    std::condition_variable m_cv;
    std::mutex m_logMutex;
    int m_numThreads;
};

}