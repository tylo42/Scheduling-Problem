#include "Scheduler.hpp"
#include "DesignByContract.hpp"

#include <iostream>

int main() {
   Scheduler s(2, 2);
   std::cout << s.solve() << std::endl;
}
