# Set environment variables
$env:CC = "cl"
$env:CXX = "cl"

# Build the project
cmake -S . -B build
cmake --build build

# Run the application
Start-Process "build\JsonHelix.exe"
