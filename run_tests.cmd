echo "Run timed queue unit tests"
cd bin/Release
timed_queue_functional_test.exe
timed_queue_performance_test.exe
timed_queue_load_test.exe