#pragma once

#include <queue>

// Thread-safe queue
template <typename T>
class TSQueue
{
private:
    // Underlying queue
    std::deque<T> m_queue;

    // mutex for thread synchronization
    std::mutex m_mutex;

    // Condition variable for signaling
    std::condition_variable m_cond;

public:
    // Pushes an element to the queue
    void push(T item)
    {

        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Add item
        m_queue.push_back(item);

        // Notify one thread that
        // is waiting
        m_cond.notify_one();
    }

    // Pushes an element to the front of the queue
    void push_front(T item)
    {

        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Add item
        m_queue.push_front(item);

        // Notify one thread that
        // is waiting
        m_cond.notify_one();
    }

    // Pops an element off the queue
    T pop()
    {

        // acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // wait until queue is not empty
        m_cond.wait(lock,
                    [this]()
                    { return !m_queue.empty(); });

        // retrieve item
        T item = m_queue.front();
        m_queue.pop_front();

        // return item
        return item;
    }

    // Checks if the queue is empty
    bool empty()
    {

        // acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Call wrapped funciton
        return m_queue.empty();
    }
};