#!/bin/bash

MyPass=$HOME/TuringAdvancedProgramming19B/Task1/MyPass.so
clang -Xclang -load -Xclang $MyPass -c TestMe.c
