$(CXX) = g++

CXX += -std=c++0x -Wall -Werror

all: CXX += -O3
all: SchedulingProblem test
	./test

debug: CXX += -DDEBUG -g
debug: SchedulingProblem test
	./test

SchedulingProblem: main.o Scheduler.o Schedule.o
	$(CXX) main.o Scheduler.o Schedule.o -o SchedulingProblem

main.o: main.cpp Scheduler.hpp Schedule.hpp DesignByContract.hpp
	$(CXX) -c main.cpp

Scheduler.o: Scheduler.cpp Scheduler.hpp Schedule.hpp DesignByContract.hpp
	$(CXX) -c Scheduler.cpp

Schedule.o: Schedule.cpp Schedule.hpp DesignByContract.hpp MathUtils.hpp
	$(CXX) -c Schedule.cpp

test: test_main.o Scheduler.o Schedule.o 
	$(CXX) test_main.o Scheduler.o Schedule.o -o test

test_main.o: test_main.cpp Scheduler.hpp Schedule.hpp
	$(CXX) -c test_main.cpp -o test_main.o

clean:
	rm -rf *.o SchedulingProblem test test_actual.txt
