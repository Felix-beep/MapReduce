CC = g++ -std=c++20 -lstdc++
CFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror 

all: MapReduce Tests

.outputFolder:
	mkdir -p out
	mkdir -p ResultFolder
	
MapReduce: .outputFolder
	$(CC) $(CFLAGS) MapReduce.cpp -o out/MapReduce 

Tests: .outputFolder
	$(CC) $(CFLAGS) Tests.cpp -o out/Tests
	./out/Tests

clean:
	rm -rf out
	rm -rf ResultFolder
