all: MapReduce 

.outputFolder:
	mkdir -p out
	
MapReduce: .outputFolder
	g++ -std=c++17 -lstdc++ -lm MapReduce.cpp -Wall -Wextra -Wno-unused-parameter -Werror -o out/MapReduce
	./out/MapReduce

Tests: .outputFolder
	g++ -std=c++17 -lstdc++ -lm Tests.cpp -Wall -Wextra -Wno-unused-parameter -Werror -o out/Tests
	./out/Tests