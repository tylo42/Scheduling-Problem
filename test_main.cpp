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

static void test_all_scheduler(size_t people, size_t groups, size_t num_solutions) {
   Scheduler s(people, groups);
   BOOST_CHECK(num_solutions == s.solve());
   BOOST_CHECK(num_solutions == s.solutions().size());

   BOOST_CHECK(1 == s.solve(Scheduler::ONE));
   BOOST_CHECK(1 == s.solutions().size());
}

static void test_one_scheduler(size_t people, size_t groups) {
   Scheduler s(people, groups);
   BOOST_CHECK(1 == s.solve(Scheduler::ONE));
   BOOST_CHECK(1 == s.solutions().size());
}

static void test_scheduler() {
   test_all_scheduler(2, 2, 1);
   test_all_scheduler(2, 3, 2);
   test_all_scheduler(3, 3, 12);
   test_one_scheduler(2, 4);
   test_one_scheduler(4, 4);
   test_one_scheduler(2, 5);
   //test_one_scheduler(3, 5)
   test_one_scheduler(5, 5);
}

static Schedule::round_type make_round(const Schedule::group_type & g1, const Schedule::group_type & g2) {
   return Schedule::round_type({std::make_shared<Schedule::group_type>(g1), std::make_shared<Schedule::group_type>(g2)});
}

static void test_schedule() {
   {
      Schedule s(2, 2, 3);
      BOOST_CHECK(0 == s.round_size());

      s.push_round(make_round(Schedule::group_type({ 0, 1 }), Schedule::group_type({ 2, 3 })));
      BOOST_CHECK(1 == s.round_size());

      BOOST_CHECK( s.valid_group(Schedule::group_type({0, 2}), 1));
      BOOST_CHECK(!s.valid_group(Schedule::group_type({0, 1}), 1));
      BOOST_CHECK( s.valid_group(Schedule::group_type({0, 1}), 2));

      s.push_round(make_round(Schedule::group_type({ 0, 2 }), Schedule::group_type({ 1, 3 })));
      BOOST_CHECK(2 == s.round_size());

      s.push_round(make_round(Schedule::group_type({ 0, 3 }), Schedule::group_type({ 1, 2 })));
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
