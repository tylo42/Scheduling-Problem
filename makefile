$(CXX) = g++

CXX += -std=c++0x -Wall -Werror

all: CXX += -O2
all: SchedulingProblem test
	./unit_test/test

debug: CXX += -DDEBUG -g
debug: SchedulingProblem test
	./unit_test/test

test: Scheduler.o test_Scheduler.o
	$(CXX) unit_test/test_Scheduler.o Scheduler.o -o unit_test/test

SchedulingProblem: main.o Scheduler.o
	$(CXX) main.o Scheduler.o -o SchedulingProblem

main.o: main.cpp Scheduler.hpp DesignByContract.hpp
	$(CXX) -c main.cpp

Scheduler.o: Scheduler.cpp Scheduler.hpp DesignByContract.hpp
	$(CXX) -c Scheduler.cpp

test_Scheduler.o:
	$(CXX) -lboost_unit_test_framwork -c unit_test/test_Scheduler.cpp -o unit_test/test_Scheduler.o

clean:
	rm -rf *.o SchedulingProblem unit_test/*.o unit_test/test
