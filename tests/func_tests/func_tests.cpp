#include <thread>
#include <numeric>
#include <iostream>
#include <threading_toy/timed_queue.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

using namespace boost::unit_test;
 
// Functional tests
/*
* Push and check count
* Pop and check count
* Push concurrently and check count
* Pop concurrently and check count
* Push with delay and check count
* Push with delay concurrently and check count
* Fill the queue and wait push for 1000 milliseconds
* Empty the queue and wait pop for 1000 milliseconds
*/
 
BOOST_AUTO_TEST_SUITE(TimedQueueFunctionalTests);
 
///////////////////////////////////
// Helper functions and classes
 
/// Testing single-threaded push
template <typename T>
void pushTestValues(TimedQueue<T>& q, size_t count)
{
	srand(static_cast<unsigned>(time(NULL)));
	for (size_t i = 0; i < count; ++i)
	{
		int* item = new int(rand());
		q.enqueue(item);
	}
}
 
/// Testing single-threaded pop
template <typename T>
void popTestValues(TimedQueue<T>& q, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		int* item = q.dequeue();
		delete item;
	}
}
 
/// Testing concurrent push and pop
template <typename T>
void testValuesConcurrently()
{
	TimedQueue<int> q(32);
	BOOST_CHECK_EQUAL(q.count(), 0);
 
	std::thread t1(pushTestValues<int>, std::ref(q), 3);
	std::thread t2(pushTestValues<int>, std::ref(q), 3);
	t1.join();
	t2.join();
	BOOST_CHECK_EQUAL(q.count(), 6);
 
	std::thread t3(popTestValues<int>, std::ref(q), 2);
	std::thread t4(popTestValues<int>, std::ref(q), 2);
	t3.join();
	t4.join();
	BOOST_CHECK_EQUAL(q.count(), 2);
}
 
///////////////////////////////////
// Test cases
 
/// Testing single-threaded push
BOOST_AUTO_TEST_CASE(Push_Pop_Check_Count)
{
	TimedQueue<int> q(32);
	std::cout << "Single-threaded tests..." << std::endl;
	
	// Check empty queue
	BOOST_CHECK_EQUAL(q.count(), 0);
 
	// Push 3 elenemts
	pushTestValues(q, 3);
	BOOST_CHECK_EQUAL(q.count(), 3);
 
	// Pop 2 elenemts
	popTestValues(q, 2);
	BOOST_CHECK_EQUAL(q.count(), 1);
}

// Test concurrent push/pop
BOOST_AUTO_TEST_CASE(Push_Pop_Check_Count_Concurrently)
{
	std::cout << "Concurrent tests..." << std::endl;
	testValuesConcurrently<int>();
	testValuesConcurrently<long long>();
	testValuesConcurrently<double>();
	testValuesConcurrently<void*>();
}

// Test push to the locked queue (maximum of elements)
BOOST_AUTO_TEST_CASE(Push_Check_Count_Timed)
{
	std::cout << "Timed push test..." << std::endl;
	typedef std::chrono::high_resolution_clock clock_t;
	typedef std::chrono::milliseconds ms_t;
	typedef std::chrono::duration<double> delta_t;
 
	// create empty queue
	const size_t queueSize = 128;
	TimedQueue<int> q(queueSize);
	BOOST_CHECK_EQUAL(q.count(), 0);
 
	// fill it completely
	pushTestValues(q, queueSize);
 
	int* item = new int(rand());
	auto t0 = clock_t::now();
 
	// add one more item, shold time out (~1 second)
	bool ret = q.enqueue(item, 1000);
	delete item;
	delta_t delta = clock_t::now() - t0;
	BOOST_CHECK_EQUAL(ret, false);
	BOOST_REQUIRE_CLOSE( delta.count(), 1.0, 10.0);
}
 
// Test pop from the empty queue 
BOOST_AUTO_TEST_CASE(Pop_Check_Count_Timed)
{
	std::cout << "Timed pop test..." << std::endl;
	typedef std::chrono::high_resolution_clock clock_t;
	typedef std::chrono::milliseconds ms_t;
	typedef std::chrono::duration<double> delta_t;
 
	const size_t queueSize = 128;
	TimedQueue<int> q(queueSize);
	auto t0 = clock_t::now();
	int* item = q.dequeue(1000);
	int* zero = nullptr;
	delta_t delta = clock_t::now() - t0;
	BOOST_CHECK_EQUAL(zero, item);
	BOOST_REQUIRE_CLOSE(delta.count(), 1.0, 10.0);
}
 
BOOST_AUTO_TEST_SUITE_END()
 
