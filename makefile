CC = g++ -std=c++20 -lstdc++
CFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror 

all: MapReduce 

.outputFolder:
	mkdir -p out
	
MapReduce: .outputFolder
	$(CC) $(CFLAGS) MapReduce.cpp -o out/MapReduce 
	./out/MapReduce

Tests: .outputFolder
	g++ -std=c++17 -lstdc++ -lm Tests.cpp -Wall -Wextra -Wno-unused-parameter -Werror -o out/Tests
	./out/Tests

clean:
	rm -rf out
