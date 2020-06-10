#!/bin/bash

MyPass=./MyPass.so
clang -mllvm -mfs=10 -mllvm -mis=5 -mllvm -min_conf=85 -Xclang -load -Xclang $MyPass -g -c TestMe.c