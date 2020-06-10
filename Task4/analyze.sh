#!/bin/bash

MyPass=./MyPass.so
clang -Xclang -load -Xclang $MyPass -g -c TestMe.c
