#include "Scheduler.hpp"
#include "Schedule.hpp"

#include <boost/test/minimal.hpp>

static void test_scheduler();
static void test_schedule();

int test_main(int, char *[]) {
   test_scheduler();
   test_schedule();
   return 0;
}

static void test_one_scheduler(size_t people, size_t groups, size_t num_solutions) {
   Scheduler s(people, groups);
   BOOST_CHECK(num_solutions == s.solve());

   Scheduler::solution_set solutions = s.solutions();
   BOOST_CHECK(num_solutions == solutions.size());

   BOOST_CHECK(1 == s.solve(Scheduler::ONE));
}

static void test_scheduler() {
   test_one_scheduler(2, 2, 2);
   test_one_scheduler(2, 3, 48);
   test_one_scheduler(3, 3, 72);
}

static void test_schedule() {
   {
      Schedule s(2, 2, 3);
      BOOST_CHECK(0 == s.round_size());

      s.push_round(Schedule::round_type({Schedule::group_type({ 1, 2 }),  Schedule::group_type({ 3, 4 })}));
      BOOST_CHECK(1 == s.round_size());

      BOOST_CHECK(s.valid_group(Schedule::group_type({1, 3})));
      BOOST_CHECK(!s.valid_group(Schedule::group_type({1, 2})));

      s.push_round(Schedule::round_type({Schedule::group_type({ 1, 3 }),  Schedule::group_type({ 2, 4 })}));
      BOOST_CHECK(2 == s.round_size());

      s.push_round(Schedule::round_type({Schedule::group_type({ 1, 4 }),  Schedule::group_type({ 2, 3 })}));
      BOOST_CHECK(3 == s.round_size());

      // Just print for now for visual check, add better checking later
      std::cout << s;

      s.pop_round();
      BOOST_CHECK(2 == s.round_size());

      s.pop_round();
      BOOST_CHECK(1 == s.round_size());

      s.pop_round();
      BOOST_CHECK(0 == s.round_size());
   }
}
