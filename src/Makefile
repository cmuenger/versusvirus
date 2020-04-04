all: simulation

simulation: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp
	g++ --std=c++17 -O3 entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation


debug: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp
	g++ --std=c++17 -O0 -g -Wall entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation