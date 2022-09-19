#!/bin/sh
## Acceptance Test Script for metronome

echo "********************************************"
echo "Unit Test [1]"
echo "./metronome"
echo "Expected: usage message.\n"

echo "Output:"
./metronome
echo "********************************************\n\n"
sleep 5


echo "********************************************"
echo "Unit Test [2]"
echo "./metronome 120 2 4 &"
echo "Expected: [metronome: 120 beats/min, time signature 2/4, secs-per-interval: 0.25, nanoSecs: 250000000]"
echo "Expected: API of metronome resmgr\n"
echo "Output:"
./metronome 120 2 4 &
sleep 10
echo "\n********************************************\n\n"


echo "********************************************"
echo "Unit Test [3]"
echo "cat /dev/local/metronome"
echo "Expected: [metronome: 120 beats/min, time signature 2/4, secs-per-interval: 0.25, nanoSecs: 250000000]\n"
echo "Output:"
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [4] \ncat /dev/local/metronome-help"
echo "Expected: information regarding the metronome resmgr’s API, as seen above\n"
echo "Output:"
cat /dev/local/metronome-help
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [5]"
echo "echo set 100 2 4 > /dev/local/metronome"
echo "cat /dev/local/metronome"
echo "Expected:  metronome regmgr changes settings to: 100 bpm in 2/4 time; run-time behaviour of 
metronome changes to 100 bpm in 2/4 time\n"
echo "Output:"
echo set 100 2 4 > /dev/local/metronome
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [6]"
echo "cat /dev/local/metronome"
echo "Expected: [metronome: 100 beats/min, time signature 2/4, secs-per- interval: 0.30, nanoSecs: 300000000]\n"
echo "Output:"
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [7]"
echo "echo set 200 5 4 > /dev/local/metronome"
echo "cat /dev/local/metronome"
echo "Excepted: metronome regmgr changes settings to: 200 bpm in 5/4 time; run-time behaviour of metronome changes to 200 bpm in 5/4 time\n"
echo "Output:"
echo set 200 5 4 > /dev/local/metronome
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [8]"
echo "cat /dev/local/metronome"
echo "Expected: [metronome: 200 beats/min, time signature 5/4, secs-per- interval: 0.15, nanoSecs: 150000000]\n"
echo "Output:"
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [9]"
echo "echo stop > /dev/local/metronome"
echo "Expected: metronome stops running; metronome resmgr is still running as a process: pidin | grep metronome\n"
echo "Output:"
echo stop > /dev/local/metronome
pidin | grep metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [10]"
echo "echo start > /dev/local/metronome"
echo "cat /dev/local/metronome"
echo "Expected: metronome starts running again at 200 bpm in 5/4 time, which is the last setting; metronome resmgr is still running as a process: pidin | grep metronome\n"
echo "Output:"
echo start > /dev/local/metronome
pidin | grep metronome
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [11]"
echo "cat /dev/local/metronome"
echo "Expected: [metronome: 200 beats/min, time signature 5/4, secs-per- interval: 0.15, nanoSecs: 150000000]\n"
echo "Output:"
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [12]"
echo "echo stop > /dev/local/metronome"
echo "Expected: metronome stops running; metronome resmgr is still running as a process: pidin | grep metronome.\n"
echo "Output:"
echo stop > /dev/local/metronome
pidin | grep metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [13]"
echo "echo stop > /dev/local/metronome"
echo "Expected: metronome remains stopped; metronome resmgr is still running as a process: pidin | grep metronome\n"
echo "Output:"
echo stop > /dev/local/metronome
pidin | grep metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [14]"
echo "echo start > /dev/local/metronome"
echo "cat /dev/local/metronome"
echo "Expected: metronome starts running again at 200 bpm in 5/4 time, which is the last setting; metronome resmgr is still running as a process: pidin | grep metronome\n"
echo "Output:"
echo start > /dev/local/metronome
pidin | grep metronome
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [15]"
echo "echo start > /dev/local/metronome"
echo "cat /dev/local/metronome"
echo "Expected: metronome is still running again at 200 bpm in 5/4 time, which is the last setting; metronome resmgr is still running as a process: pidin | grep metronome\n"
echo "Output:"
echo start > /dev/local/metronome
pidin | grep metronome
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [16]"
echo "cat /dev/local/metronome"
echo "Expected: [metronome: 200 beats/min, time signature 5/4, secs-per- interval: 0.15, nanoSecs: 150000000]\n"
echo "Output:"
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [17]"
echo "echo pause 3 > /dev/local/metronome"
echo "Expected: metronome continues on next beat (not next measure)\n"
echo "Output:"
echo pause 3 > /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [18]"
echo "echo pause 10 > /dev/local/metronome"
echo "Expected: properly formatted error message, and metronome continues to run\n"
echo "Output:"
echo pause 10 > /dev/local/metronome
sleep 5
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [19]"
echo "echo bogus > /dev/local/metronome"
echo "Expected: properly formatted error message, and metronome continues to run\n"
echo "Output:"
echo bogus > /dev/local/metronome
sleep 5
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [20]"
echo "echo set 120 2 4 > /dev/local/metronome"
echo "cat /dev/local/metronome"
echo "cat /dev/local/metronome-help"
echo "Expected: [metronome: 120 beats/min, time signature 2/4, secs-per-interval: 0.25, nanoSecs: 250000000]\n"
echo "Output:"
echo set 120 2 4 > /dev/local/metronome
cat /dev/local/metronome
cat /dev/local/metronome-help
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [21]"
echo "cat /dev/local/metronome"
echo "Expected: [metronome: 120 beats/min, time signature 2/4, secs-per-interval: 0.25, nanoSecs: 250000000]\n"
echo "Output:"
cat /dev/local/metronome
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [22] \ncat /dev/local/metronome-help"
echo "Expected: information regarding the metronome resmgr’s API, as seen above\n"
echo "Output:"
cat /dev/local/metronome-help
sleep 10
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [23]"
echo "echo Writes-Not-Allowed > /dev/local/metronome-help"
echo "Expected: Error Message - Cannot take an input.\n"
echo "Output:"
echo Writes-Not-Allowed > /dev/local/metronome-help
sleep 5
echo "********************************************\n\n"


echo "********************************************"
echo "Unit Test [24]"
echo "echo quit > /dev/local/metronome && pidin | grep metronome"
echo "metronome gracefully terminates\n"
echo "Output:"
echo quit > /dev/local/metronome && pidin | grep metronome
sleep 5
echo "********************************************\n\n"

exit 0
