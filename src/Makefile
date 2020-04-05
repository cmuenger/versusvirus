all: simulation

util.o: util.hpp util.cpp
	g++ --std=c++17 -O3 util.cpp -c

entities.o: entities.hpp entities.cpp
	g++ --std=c++17 -O3 entities.cpp -c

parameters.o: parameters.hpp parameters.cpp
	g++ --std=c++17 -O3 parameters.cpp -c

simulation.o: simulation.hpp simulation.cpp
	g++ --std=c++17 -O3 simulation.cpp -c

main.o: main.cpp
	g++ --std=c++17 -O3 main.cpp -c

simulation: util.o entities.o parameters.o simulation.o main.o
	g++ --std=c++17 -O3 util.o entities.o parameters.o simulation.o main.o -o simulation

clean:
	rm -f *.o
	rm -f simulation

simulation_single: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp util.hpp util.cpp
	g++ --std=c++17 -O3 util.cpp entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation


debug: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp
	g++ --std=c++17 -O0 -g -Wall entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation