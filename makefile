$(CXX) = g++

CXX += -std=c++0x -Wall -Werror

all: CXX += -O2
all: SchedulingProblem

debug: CXX += -DDEBUG -g
debug: SchedulingProblem

SchedulingProblem: main.o Scheduler.o
	$(CXX) main.o Scheduler.o -o SchedulingProblem

main.o: main.cpp Scheduler.hpp DesignByContract.hpp
	$(CXX) -c main.cpp

Scheduler.o: Scheduler.cpp Scheduler.hpp DesignByContract.hpp
	$(CXX) -c Scheduler.cpp

clean:
	rm -rf *.o SchedulingProblem
