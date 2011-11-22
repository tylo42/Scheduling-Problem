#include "Scheduler.hpp"
#include "DesignByContract.hpp"

#include <iostream>

int main() {
   Scheduler s(5, 5);
   if(s.solve(Scheduler::ONE) > 0) {
      Scheduler::solution_set solution = s.solutions();
      std::cout << solution[0];
   } else {
      std::cout << "No Solution Found" << std::endl;
   }
}
