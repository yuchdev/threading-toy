#include <iostream>
#include <thread>
#include "timed_queue.h"
 
template <typename T>
SharedQueue<T>& getSharedQueue(size_t queueSize){
    static SharedQueue<T> q(queueSize);
    return q;
}
 
/////////////////////////////////////////////////////
// TEST ITEMS
 
struct TestItem
{
    TestItem(int i):_x(i), _y(rand()), _z(){};
    int _x;
    long _y;
    double _z;
};
 
 
struct HeavyItem
{
    HeavyItem(int i):_i(i),_array(){}
        int _i;
        long _array[255];
};
 
 
////////////////////////////////////////////////////
// LOAD TESTS 1
template <typename T>
void add_queue_values(SharedQueue<T>& q){
    srand (static_cast<unsigned>(time(NULL)));
    int counter = 0;
    while (true)
    {
        std::cout << ' ' << counter << '\n';
        T* i = new T( rand() );
        q.enqueue(i);
        ++counter;
    }
}
 
template <typename T>
void read_queue_values(SharedQueue<T>& q){
    int counter = 0;
    while (true)
    {
        std::cout << counter << '\n';
        T* i = q.dequeue();
        delete i;
        --counter;
    }
}
 
////////////////////////////////////////////////////
// LOAD TESTS 2
template <typename T>
void add_queue_values_timed(SharedQueue<T>& q){
    srand (static_cast<unsigned>(time(NULL)));
    int counter = 0;
    while (true)
    {
        std::cout << "W " << counter << '\n';
        T* i = new T( rand() );
        if(!q.enqueue(i, 1))
            std::cout << "Fail to push at step " << counter << '\n';
        ++counter;
    }
}
 
template <typename T>
void read_queue_values_timed(SharedQueue<T>& q){
    int counter = 0;
    while (true)
    {
        std::cout << "R" << counter << '\n';
        T* i = nullptr;
        if((i = q.dequeue(1)) == nullptr)
            std::cout << "Fail to pop at step " << counter << '\n';
        delete i;
        --counter;
    }
}
 
template <typename T>
void testQueueBasic(size_t queueSize){
    SharedQueue<T>& q = getSharedQueue<T>(queueSize);
    std::thread t1(add_queue_values<T>, std::ref(q));
    std::thread t2(read_queue_values<T>, std::ref(q));
    t1.join();
    t2.join();
}
 
template <typename T>
void testQueueTimed(size_t queueSize){
    SharedQueue<T>& q = getSharedQueue<T>(queueSize);
    std::thread t1(add_queue_values_timed<T>, std::ref(q));
    std::thread t2(read_queue_values_timed<T>, std::ref(q));
    t1.join();
    t2.join();
}
 
int main(int argc, char* argv[]) {
    
    try
	{
        testQueueTimed<int>(128);
        //testQueueTimed<long long>(128);
        //testQueueTimed<double>(128);
        //testQueueTimed<TestItem>(128);
        //testQueueTimed<HeavyItem>(128);
    }
    catch(const std::exception& e){
        std::cout << e.what() << '\n';
    }
    return 0;
}