#include "Scheduler.hpp"
#include "DesignByContract.hpp"

#include <boost/thread.hpp>

#include <iostream>

int main() {
   Scheduler s(2, 10);
   s.solve();
}