#include "Scheduler.hpp"
#include "DesignByContract.hpp"

#include <iostream>

#include <time.h>

int main() {
   Scheduler s(3,5);
   clock_t start = clock();
   if(s.solve(Scheduler::ONE) > 0) {
      Scheduler::solution_set solution = s.solutions();
      std::cout << solution[0];
   } else {
      std::cout << "No Solution Found" << std::endl;
   }
   std::cout << "Run time: " << static_cast<float>( clock() - start ) / CLOCKS_PER_SEC << " seconds" << std::endl;
}
