echo "Run timed queue unit tests"
echo "RELASE configuration required!"
cd bin/Release
timed_queue_functional_test.exe
timed_queue_performance_test.exe
timed_queue_load_test.exe
