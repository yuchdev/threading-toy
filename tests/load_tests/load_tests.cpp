#include <thread>
#include <numeric>
#include <iostream>
#include "timed_queue/timed_queue.h"
 

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
using namespace boost::unit_test;

////////////////////////////////////////////////////
// Load tests
// Two threads, check input/output CRC

struct HighLoadCheck
{

	HighLoadCheck(size_t passes) :_passes(passes), _control_push(), _control_pop(){}

	/// Testing push to the queue under heavy load
	void pushTestValues(TimedQueue<int>& q)
	{
		srand(static_cast<unsigned>(time(NULL)));
		for (size_t i = 0; i < _passes; ++i)
		{
			int* item = new int(rand());
			_control_push ^= *item;
			q.enqueue(item);
		}
	}

	/// Testing pop from the queue under heavy load
	void popTestValues(TimedQueue<int>& q)
	{
		for (size_t i = 0; i < _passes; ++i)
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

	// Number of push/pop calls
	size_t _passes;

	// CRC of these integers should be equal
	int _control_push;
	int _control_pop;
};

BOOST_AUTO_TEST_SUITE(TimedQueueLoadTests);


BOOST_AUTO_TEST_CASE(Performance_Measure_Test)
{
	const size_t queuePasses = 1000000;
	const size_t queueSize = 128;
	HighLoadCheck hlCheck(1000000);
	TimedQueue<int> q(queueSize);
	std::cout << "Running " << queuePasses << " iterations..." << std::endl;
	std::cout << "Check input-output consistency on the high load..." << std::endl;
	std::thread t1(&HighLoadCheck::pushTestValues, &hlCheck, std::ref(q));
	std::thread t2(&HighLoadCheck::popTestValues, &hlCheck, std::ref(q));
	t1.join();
	t2.join();
	BOOST_CHECK_EQUAL(hlCheck.isCorrect(), true);
}

BOOST_AUTO_TEST_SUITE_END()

