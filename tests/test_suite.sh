#!/bin/bash
# ================================
# CMSC 125 LAB 2 FULL TEST SUITE
# schedsim CPU Scheduler Tester
# ================================

echo "======================================"
echo "   SchedSim Full Test Suite"
echo "======================================"
echo ""

PASSED=0
FAILED=0

# Build first
echo "[BUILD] Compiling project..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "BUILD FAILED"
    exit 1
fi

echo "BUILD OK"
echo ""

# =========================
# HELPER FUNCTION
# =========================
run_test() {
    NAME=$1
    CMD=$2

    echo -n "[TEST] $NAME ... "
    eval $CMD > /tmp/out.txt 2>&1

    if [ $? -eq 0 ]; then
        echo "PASS"
        ((PASSED++))
    else
        echo "FAIL"
        ((FAILED++))
    fi
}

# =========================
# 1. BASIC ALGORITHMS
# =========================

echo "=== BASIC ALGORITHMS ==="

run_test "FCFS" "./schedsim --algorithm=FCFS --input=tests/workload1.txt"
run_test "SJF"  "./schedsim --algorithm=SJF --input=tests/workload1.txt"
run_test "STCF" "./schedsim --algorithm=STCF --input=tests/workload1.txt"
run_test "RR"   "./schedsim --algorithm=RR --quantum=30 --input=tests/workload1.txt"
run_test "MLFQ" "./schedsim --algorithm=MLFQ --mlfq-config=tests/mlfq_config.txt --input=tests/workload1.txt"

echo ""

# =========================
# 2. EDGE CASES
# =========================

echo "=== EDGE CASES ==="

run_test "Single Process FCFS" "./schedsim --algorithm=FCFS --processes='A:0:10'"
run_test "Single Process SJF"  "./schedsim --algorithm=SJF --processes='A:0:10'"
run_test "Zero Burst"          "./schedsim --algorithm=STCF --processes='A:0:0'"
run_test "Simultaneous Arrival FCFS" "./schedsim --algorithm=FCFS --input=tests/workload_simultaneous.txt"
run_test "Identical Burst SJF" "./schedsim --algorithm=SJF --input=tests/workload_identical.txt"

echo ""

# =========================
# 3. QUIZ VALIDATION
# =========================

echo "=== QUIZ 3 VALIDATION ==="

run_test "FCFS Quiz3" "./schedsim --algorithm=FCFS --input=tests/quiz3.txt"
run_test "SJF Quiz3"  "./schedsim --algorithm=SJF --input=tests/quiz3.txt"
run_test "STCF Quiz3" "./schedsim --algorithm=STCF --input=tests/quiz3.txt"
run_test "RR Quiz3"   "./schedsim --algorithm=RR --quantum=30 --input=tests/quiz3.txt"
run_test "MLFQ Quiz3" "./schedsim --algorithm=MLFQ --input=tests/quiz3.txt"

echo ""

# =========================
# 4. COMPARISON MODE
# =========================

echo "=== COMPARISON MODE ==="

run_test "Compare All Algorithms" "./schedsim --compare --input=tests/workload1.txt"

echo ""

# =========================
# 5. LARGE WORKLOAD
# =========================

echo "=== LARGE WORKLOAD ==="

run_test "100+ Process Test" "./schedsim --algorithm=FCFS --input=tests/workload_large.txt"

echo ""

# =========================
# SUMMARY
# =========================

echo "======================================"
echo "RESULTS:"
echo "PASSED: $PASSED"
echo "FAILED: $FAILED"
echo "======================================"

if [ $FAILED -gt 0 ]; then
    exit 1
fi

exit 0