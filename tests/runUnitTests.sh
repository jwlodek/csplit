#!/bin/bash

make clean tests

export LD_LIBRARY_PATH=./criterion/lib:$LD_LIBARARY_PATH

./csplit_core_tests
