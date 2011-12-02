#include "Scheduler.hpp"
#include "Schedule.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/test/minimal.hpp>

static void test_scheduler(std::ostream & os);
static void test_schedule(std::ostream & os);

int test_main(int, char *[]) {
   clock_t start = clock();

   std::ofstream file("test_actual.txt");
   BOOST_CHECK(file.is_open());
   test_scheduler(file);
   test_schedule(file);
   file.close();

   std::ifstream actual("test_actual.txt");
   std::ifstream expected("test_expected.txt");
   BOOST_CHECK(actual.is_open());
   BOOST_CHECK(expected.is_open());

   while(!actual.eof() && !expected.eof()) {
      std::string actual_line;
      std::string expected_line;
      actual >> actual_line;
      expected >> expected_line;
      BOOST_CHECK(actual_line == expected_line);
   }

   actual.close();
   expected.close();

   std::cout << "Run time: " << static_cast<double>( clock() - start ) / CLOCKS_PER_SEC << " seconds" << std::endl;
   return 0;
}

static void print_schedules(const Scheduler::solution_set & schedules, std::ostream & os) {
   for(auto it = schedules.begin(); it != schedules.end(); ++it) {
      os << *it;
      os << std::endl;
   }
}

static void test_all_scheduler(size_t people, size_t groups, size_t num_solutions, std::ostream & os) {
   Scheduler s(people, groups);
   BOOST_CHECK(num_solutions == s.solve());
   BOOST_CHECK(num_solutions == s.solutions().size());

   print_schedules(s.solutions(), os);

   BOOST_CHECK(1 == s.solve(Scheduler::ONE));
   BOOST_CHECK(1 == s.solutions().size());

   print_schedules(s.solutions(), os);
}

static void test_one_scheduler(size_t people, size_t groups, std::ostream & os) {
   Scheduler s(people, groups);
   BOOST_CHECK(1 == s.solve(Scheduler::ONE));
   BOOST_CHECK(1 == s.solutions().size());

   print_schedules(s.solutions(), os);
}

static void test_scheduler(std::ostream & os) {
   test_all_scheduler(2, 2, 1, os);
   test_all_scheduler(2, 3, 2, os);
   test_all_scheduler(3, 3, 12, os);
   test_one_scheduler(2, 4, os);
   test_one_scheduler(4, 4, os);
   test_one_scheduler(2, 5, os);
   test_one_scheduler(5, 5, os);
}

static Schedule::round_type make_round(const Schedule::group_type & g1, const Schedule::group_type & g2) {
   return Schedule::round_type({std::make_shared<Schedule::group_type>(g1), std::make_shared<Schedule::group_type>(g2)});
}

static void test_schedule(std::ostream & os) {
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
      os << s;

      s.pop_round();
      BOOST_CHECK(2 == s.round_size());

      s.pop_round();
      BOOST_CHECK(1 == s.round_size());

      s.pop_round();
      BOOST_CHECK(0 == s.round_size());
   }
}
