#!/bin/bash
echo Step 1 - generate file:
echo first@otus.owl > test-mr.txt
echo fist@otus.owl >> test-mr.txt
echo fisddt@otus.owl >> test-mr.txt
echo fist@otus.owl >> test-mr.txt
echo fissdsdfdst@otus.owl >> test-mr.txt
echo fsdfist@otus.owl >> test-mr.txt
echo fisdst@otus.owl >> test-mr.txt
echo fifghssdft@otus.owl >> test-mr.txt
echo fisfght@otus.owl >> test-mr.txt
echo fiasst@otus.owl >> test-mr.txt
echo fist@otus.owl >> test-mr.txt
echo fdjhisjt@otus.owl >> test-mr.txt
echo ghhgjfist@otus.owl >> test-mr.txt
echo fdhjist@otus.owl >> test-mr.txt
echo test-mr.txt
echo
echo Step 2 - mr
cmake-build-debug/mr -s test-mr.txt -m 4 -r 5
