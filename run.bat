@echo off
echo Building the C++ project
make

if %errorlevel% neq 0 (
    echo Build failed. Exiting.
    exit /b %errorlevel%
)

echo Running the C++ program...
./out/MapReduce