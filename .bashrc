#!/bin/bash
# build and run script for the project

g++ -std=c++17 -Wall -Wextra -o gitvc.exe main.cpp file.cpp tree.cpp map.cpp 
    echo "Build successful. Running program..."
    ./gitvc.exe
else
    echo "Build failed."
fi
