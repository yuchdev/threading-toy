#include <thread>
#include <numeric>
#include "timed_queue/timed_queue.h"
 

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
using namespace boost::unit_test;
 
// Performance tests
// Add timestamp to the item: fill at push, check at pop
// Calculate min, max, middle, median
// Two threads, one million packages, calculate push and pop misses
BOOST_AUTO_TEST_SUITE(TimedQueuePerformanceTests);
 
struct TimedObject
{
	typedef std::chrono::microseconds mcs;
	typedef std::chrono::high_resolution_clock clock;
	std::chrono::system_clock::time_point _timestamp;
	TimedObject() :_timestamp(clock::now()){}
	mcs delay(){ return std::chrono::duration_cast<mcs>(clock::now() - _timestamp); }
};
 
struct PerformacnceTest
{
	PerformacnceTest(size_t passes) :_passes(),_min(0.0), _max(10e9), _average(), _total_latency(), _median()
	{
		_delays.reserve(_passes);
	}
 
	void add_queue_values(SharedQueue<TimedObject>& q)
	{
		for (size_t i = 0; i < _passes; ++i)
		{
			q.enqueue(new TimedObject());
		}
	}
 
	void read_queue_values(SharedQueue<TimedObject>& q){
		for (size_t i = 0; i < _passes; ++i)
		{
			TimedObject* tmo = q.dequeue();
			std::chrono::microseconds ltncy = tmo->delay();
			_delays[i] = ltncy.count();
			delete tmo;
		}
	}
 
	void calcDelay(double latency)
	{
		if (latency < _min) _min = latency;
		if (latency > _max) _max = latency;
		_total_latency += latency;
	}
 
	void calculateStat()
	{
		std::sort(_delays.begin(), _delays.end());
		
		_average = calculateAvg();
		_median = calculateMedian();
	}
 
	double calculateAvg()
	{
		// copy from big to small for better precision
		return std::accumulate(_delays.rbegin(), _delays.rend(), 0) / _passes;
	}
 
	double calculateMedian()
	{
		size_t size = _delays.size();
		if (size % 2 == 0)
		{
			return (_delays[size / 2 - 1] + _delays[size / 2]) / 2;
		}
		else
		{
			return  _delays[size / 2];
		}
	}
 
	std::vector<double> _delays;
	size_t _passes;
	double _min;
	double _max;
	double _average;
	double _total_latency;
	double _median;
};
 
 
 
BOOST_AUTO_TEST_CASE(Performance_Measure_Test)
{
	const size_t queueSize = 1;
	const size_t queuePasses = 1000000;
	SharedQueue<TimedObject> q(queueSize);
	PerformacnceTest test(1000000);
	std::thread t1(&PerformacnceTest::add_queue_values, &test, std::ref(q));
	std::thread t2(&PerformacnceTest::read_queue_values, &test, std::ref(q));
	t1.join();
	t2.join();
 
	BOOST_TEST_MESSAGE("Minimal latency time:" << test._min);
	BOOST_TEST_MESSAGE("Maximal latency time:" << test._max);
	BOOST_TEST_MESSAGE("Average latency time:" << test._average);
	BOOST_TEST_MESSAGE("Median latency time:" << test._median);
}
 
BOOST_AUTO_TEST_SUITE_END()
 
 
