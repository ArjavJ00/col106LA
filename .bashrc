#!/bin/bash
# build and run script for the project

g++ -std=c++17 -Wall -Wextra -o tfs.exe *.cpp
if [ $? -eq 0 ]; then
    echo "Build successful. Running program..."
    ./tfs.exe
else
    echo "Build failed."
fi
