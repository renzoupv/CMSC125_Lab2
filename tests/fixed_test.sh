#!/bin/bash
# CMSC 125 Lab 2 Test Suite

echo "Running CMSC 125 Lab 2 Test Suite..."

# Test 1: FCFS Quiz 3
echo "Test 1: Lecture Quiz 3 Workload"
./schedsim --algorithm=FCFS --input=tests/quiz3.txt > tests/fcfs_result.txt
if grep -q "515" tests/fcfs_result.txt; then
    echo "  FCFS: PASS"
else
    echo "  FCFS: FAIL (expected avg TT = 515)"
fi

# Test 2: Zero Burst
echo "Test 2: Edge Cases"
./schedsim --algorithm=STCF --processes="A:0:0" > /dev/null
if [ $? -eq 0 ]; then
    echo "  Zero burst time: PASS"
else
    echo "  Zero burst time: FAIL"
fi

echo "Test suite complete."
