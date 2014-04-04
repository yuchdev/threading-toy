#include <thread>
#include <numeric>
#include "timed_queue/timed_queue.h"
 
 
// Load tests
// Two threads, check input/output CRC
////////////////////////////////////////////////////
 
struct HighLoadCheck
{
	
	HighLoadCheck(size_t passes) :_passes(passes), _control_push(), _control_pop(){}
	
	void pushTestValues(SharedQueue<int>& q, size_t count)
	{
		srand(static_cast<unsigned>(time(NULL)));
		for (size_t i = 0; i < count; ++i)
		{
			int* item = new int(rand());
			_control_push ^= *item;
			q.enqueue(item);
		}
	}
 
	/// Testing single-threaded pop
	void popTestValues(SharedQueue<int>& q, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			int* item = q.dequeue();
			_control_pop ^= *item;
			delete item;
		}
	}
 
	bool isCorrect()
	{
		return (_control_push == _control_pop);
	}
 
	size_t _passes;
	int _control_push;
	int _control_pop;
};

BOOST_AUTO_TEST_SUITE(TimedQueuePerformanceTests);
template <typename T>
void testQueueTimed(size_t queueSize){
	const size_t queuePasses = 1000000;
	HighLoadCheck hlCheck(1000000);
	SharedQueue<T>& q = getSharedQueue<T>(queueSize);
	std::thread t1(&HighLoadCheck::pushTestValues, &hlCheck, std::ref(q));
	std::thread t2(&HighLoadCheck::popTestValues, &hlCheck, std::ref(q));
	t1.join();
	t2.join();
}

BOOST_AUTO_TEST_SUITE_END()
 
 
 
// Valgrind rusn: memcheck, thread check
