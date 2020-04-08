all: simulation

COMPFLAGS = --std=c++17 -O3 
COMP = g++
LIBS = -lstdc++fs `libpng-config --ldflags`

util.o: util.hpp util.cpp
	${COMP} ${COMPFLAGS} util.cpp -c

entities.o: entities.hpp entities.cpp
	${COMP} ${COMPFLAGS} entities.cpp -c

parameters.o: parameters.hpp parameters.cpp
	${COMP} ${COMPFLAGS} parameters.cpp -c

simulation.o: simulation.hpp simulation.cpp
	${COMP} ${COMPFLAGS} simulation.cpp -c

postprocess.o: postprocess.hpp postprocess.cpp
	${COMP} ${COMPFLAGS} postprocess.cpp -c

main.o: main.cpp
	${COMP} ${COMPFLAGS} main.cpp -c

clean:
	rm -f *.o
	rm -f simulation

simulation: clean util.o entities.o parameters.o simulation.o postprocess.o main.o
	g++ ${COMPFLAGS} util.o entities.o parameters.o simulation.o postprocess.o main.o -o simulation ${LIBS}


testit: util.o entities.o parameters.o simulation.o  main.o
	g++ ${COMPFLAGS} util.o entities.o parameters.o simulation.o main.o -o simulation



simulation_single: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp util.hpp util.cpp
	g++ --std=c++17 -O3 util.cpp entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation


debug: main.cpp entities.hpp entities.cpp parameters.hpp parameters.cpp simulation.hpp simulation.cpp postprocess.hpp postprocess.cpp import.hpp
	g++ --std=c++17 -O0 -g -Wall entities.cpp parameters.cpp simulation.cpp postprocess.cpp main.cpp -o simulation