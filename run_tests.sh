#!/bin/bash

# run all tests set
echo "Run timed queue unit tests"
./bin/timed_queue_functional_test
./bin/timed_queue_performance_test
./bin/timed_queue_load_test

