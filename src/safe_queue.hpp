#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>
#include <optional>


namespace serv
{

template <typename T>
class BlockingQueue
{
public:
    constexpr static auto QUEUE_EMPTY = "cannot pop item, queue is already empty!";
    
    ~BlockingQueue()
    {
        m_done = true;
        m_cv.notify_one();
    }

    void push(const T &item)
    {
        {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_queue.push(item);
        }
        m_cv.notify_one();
    }

    std::optional<T> pop()
    {
        std::unique_lock<std::mutex> ul(m_mutex);
        m_cv.wait(ul, [=, this] { return !m_queue.empty() || m_done.load(); });
        
        if (m_queue.empty())
            return std::nullopt;
            
        T task = std::move(m_queue.front());
        m_queue.pop();
        return task;
    }
    
    bool empty()
    {
        std::lock_guard<std::mutex> lg(m_mutex);
        return m_queue.empty();
    }
    
    void stop()
    {
        {
            std::lock_guard<std::mutex> lg(m_mutex);
            m_done = true; 
        }
        m_cv.notify_all();
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_done{false};
};

}