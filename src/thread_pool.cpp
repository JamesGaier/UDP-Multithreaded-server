#include "thread_pool.hpp"
#include <sstream>

namespace serv
{

ThreadPool::ThreadPool(int numThreads)
: m_running(true)
, m_numThreads(numThreads)
{
}

void ThreadPool::addTask(Task task)
{
    m_queue.push(task);
}
    
void ThreadPool::processTask()
{
    while (m_running)
    {
        // all thread safe operations so no locking yea!
        auto taskOpt = m_queue.pop();
        if (!taskOpt)
            continue;
        
        auto task = *taskOpt;
        task();
    }
}

void ThreadPool::start()
{
    for (int i = 0; i < m_numThreads; ++i)
    {
        m_threads.emplace_back(&ThreadPool::processTask, this);
    }
}

void ThreadPool::stop()
{
    using namespace std::chrono_literals;
    m_running = false;
    while (!m_threads.empty())
    {
        m_queue.stop();
        m_threads.back().join();
        m_threads.pop_back();
    }
}

}