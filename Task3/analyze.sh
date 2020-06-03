#!/bin/bash

MyPass=$HOME/src/TuringAdvancedProgramming19B/Task3/MyPass.so
clang -Xclang -load -Xclang $MyPass -c TestMe.c
