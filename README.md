# Usage:

This Project is made to analyze a Book ("War and Peace", by Leo Tolstoy) based on the thematics of its chapters.
A Chapter with as many or more peace related terms is defined as a 


# Project Instructions

## Setup:

1. Be sure you can run C++20

You may replace the following files:
- Book.txt with a different Book:
1. the area that should be analyzed should start with 
    '*** START OF PROJECT' 
    and end with 
    '*** END OF PROJECT'
2. Each new Chapter should start with 
    'Chapter'

- Peace.txt and War.txt with other word lists (you may not change their name)
each word needs to be seperated by a new line and may include apostrophes (ex: "don't")


## Automatic Testing and Running the Program:

### Windows:
1. Run 'Set-ExecutionPolicy RemoteSinged' in Powershell to grant permissions if necessary.
2. Execute './run.bat' to run the script.

### Linux:
1. Run 'chmod -x run.sh' in Terminal to give the script the needed permissions.
2. Execute './run.sh' to run the script.

## Specific Testing  Commands in Linux:

1. Manually build solutions:
- make

2. Execute a specific solution:
### MapReduce:
- out/MapReduce
### automatic Tests:
- out/Test

3. Reduce the program to only its necessary files:
- make clean


TO DO:
[X] ReadMe
[X] Tests
[X] Do Tokenisation in Threads
[ ] PowerPoint
[X] Time