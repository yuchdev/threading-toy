#ifndef _TIMED_QUEUE_H_
#define _TIMED_QUEUE_H_

#include <list>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
 
 
/// Thread-safe fixed-size shared queue with low 'empty queue' latency.
/// @note The queue 'owns' the elements that it contains.
template <typename T> 
class TimedQueue
{
public:
 
    /// Constructor.
    /// @param size Queue size.
    TimedQueue(size_t size):_counter(), _max_counter(size){}

    TimedQueue(const TimedQueue&) = delete;
    TimedQueue& operator=(const TimedQueue&) = delete;
    TimedQueue(TimedQueue&&) = delete;
    TimedQueue& operator=(TimedQueue&&) = delete;

 
    /// Returns the number of elements contained in the Queue.        
    int count() const{
        return _counter.load();
    }
 
    /// Puts the item into the queue. 
    /// @note if the queue is full then this method blocks until there is the room for the item again.
    void enqueue(T* item){
        std::unique_lock<std::mutex> l(_mutex);
        _cond.wait( l, [this]{return (_counter.load() < _max_counter);} );
        _queue.push_back( std::unique_ptr<T>(item) );
        ++_counter;
        _cond.notify_one();
    }
 
 
    /// Puts the item into the queue. 
    /// @param millisecondsTimeout Numbers of milliseconds to wait.
    /// @return 'true' if the operation was completed successfully, 'false' if the operation timed out.
    /// @note if the queue is full then this method blocks until there is the room for the item again or the operation timed out.
    bool try_enqueue(T* item, unsigned millisecondsTimeout){
        std::unique_lock<std::mutex> l(_mutex);
        if( _cond.wait_for( l, 
            std::chrono::milliseconds(millisecondsTimeout),
            [this]{return (_counter.load() < _max_counter);} ) )
        {
            _queue.push_back( std::unique_ptr<T>(item) );
            ++_counter;
            _cond.notify_one();
            return true;
        }
        else
        {
            return false;
        }
 
    }
 
    /// Removes and returns the item at the beginning of the Queue.
    /// @note if the queue is empty then this method blocks until there is an item again.
    T* dequeue(){
        std::unique_lock<std::mutex> l(_mutex);
        _cond.wait( l, [this]{return _counter.load() > 0;} );
        T* res = _queue.front().release();
        _queue.pop_front();
        --_counter;
        _cond.notify_one();
        return res;
    }
 
    /// Removes and returns the item at the beginning of the Queue. 
    /// @param millisecondsTimeout Numbers of milliseconds to wait.
    /// @returns The item at the betting of the Queue or NULL if the operation timed out.	
    /// @note if the queue is empty then this method blocks until there is an item again or the operation timed out.
	T* try_enqueue(unsigned millisecondsTimeout){
        std::unique_lock<std::mutex> l(_mutex);
        if( _cond.wait_for( l, 
            std::chrono::milliseconds(millisecondsTimeout),
            [this]{return _counter.load() > 0;} ) )
        {
            T* res = _queue.front().release();
            _queue.pop_front();
            --_counter;
            _cond.notify_one();
            return res;
        }
        else
        {
            return nullptr;
        }
    }
 
private:
	// Synchronization
    std::condition_variable _cond;
    mutable std::mutex _mutex;

	// Underlying container tor queue
    std::list< std::unique_ptr<T> > _queue;

	// Atomic items counter
    std::atomic<size_t> _counter;

	// Maximum size of the queue
    const size_t _max_counter;
};
 

#endif //_TIMED_QUEUE_H_ 
