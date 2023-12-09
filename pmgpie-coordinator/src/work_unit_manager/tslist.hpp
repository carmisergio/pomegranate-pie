#include <list>

// Thread-safe queue
template <typename T>
class TSList
{
private:
    // Underlying queue
    std::list<T> m_list;

    // mutex for thread synchronization
    std::mutex m_mutex;

    // Condition variable for signaling
    std::condition_variable m_cond;

public:
    // Pushes an element to the list
    void push(T item)
    {

        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Add item
        m_list.push_back(item);

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
        m_list.push_front(item);

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
                    { return !m_list.empty(); });

        // retrieve item
        T item = m_list.front();
        m_list.pop_front();

        // return item
        return item;
    }

    // Checks if the queue is empty
    bool empty()
    {

        // acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Call wrapped funciton
        return m_list.empty();
    }
};