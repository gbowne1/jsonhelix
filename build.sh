#!/bin/bash

# Set environment variables
export CC=gcc
export CXX=g++

# Build the project
cmake -S . -B build
cmake --build build

# Run the application
./build/JsonHelix
