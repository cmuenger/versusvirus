all: simulation

util.o: util.hpp util.cpp
	g++ --std=c++17 -O3 -g util.cpp -c

entities.o: entities.hpp entities.cpp
	g++ --std=c++17 -O3 -g entities.cpp -c

parameters.o: parameters.hpp parameters.cpp
	g++ --std=c++17 -O3 -g parameters.cpp -c

simulation.o: simulation.hpp simulation.cpp
	g++ --std=c++17 -O3 -g simulation.cpp -c

postprocess.o: postprocess.hpp postprocess.cpp
	g++ --std=c++17 -O3 -g postprocess.cpp -c -lstdc++fs

main.o: main.cpp
	g++ --std=c++17 -O3 -g  main.cpp -c


simulation: util.o entities.o parameters.o simulation.o main.o
	g++ --std=c++17 -O3 -g util.o entities.o parameters.o simulation.o postprocess.o main.o -o simulation -lstdc++fs

clean:
	rm -f *.o
	rm -f simulation

simulation_single: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp util.hpp util.cpp
	g++ --std=c++17 -O3 util.cpp entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation


debug: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp
	g++ --std=c++17 -O0 -g -Wall entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation