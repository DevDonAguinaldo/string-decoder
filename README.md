# String Decoder
A C/C++ application to demonstrate processes, interprocesses communication, and OS fundamentals.

## Steps To Run
### Submission 1
1. Build using command:
> g++ -std=c++11 main.cpp -o main

2. Run using command:
> ./main < _filename_.txt

### Submission 2
1. Build using command:
> g++ -std=c++11 server.cpp -o server && g++ -std=c++11 client.cpp -o client

2. Run using commands:
> ./server 4000
> ./client localhost 4000 < input.txt
