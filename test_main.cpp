#include "Scheduler.hpp"
#include "Schedule.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include <boost/test/minimal.hpp>

static void test_scheduler(std::ostream & os);
static void test_schedule(std::ostream & os);

static bool files_match(const std::string & expected, const std::string & actual) {
   std::ifstream expected_file(expected.c_str());
   std::ifstream actual_file(actual.c_str());
   BOOST_CHECK(expected_file.is_open());
   BOOST_CHECK(actual_file.is_open());

   bool match = true;
   while(!expected_file.eof() && !actual_file.eof()) {
      std::string expected_line;
      std::string actual_line;
      expected_file >> expected_line;
      actual_file >> actual_line;

      if(expected_line != actual_line) {
         match = false;
         break;
      }
   }

   if(!expected_file.eof() || !actual_file.eof()) {
      match = false;
   }

   actual_file.close();
   expected_file.close();

   return match;
}

int test_main(int, char *[]) {
   clock_t start = clock();

   std::ofstream file("test_actual.txt");
   BOOST_CHECK(file.is_open());
   test_scheduler(file);
   test_schedule(file);
   file.close();

   BOOST_CHECK(files_match("test_expected.txt", "test_actual.txt"));

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

   if(num_solutions > 0) {
      BOOST_CHECK(1 == s.solve(Scheduler::ONE));
      BOOST_CHECK(1 == s.solutions().size());
   } else {
      BOOST_CHECK(0 == s.solve(Scheduler::ONE));
      BOOST_CHECK(0 == s.solutions().size());
   }

   print_schedules(s.solutions(), os);
}

static void test_one_scheduler(size_t people, size_t groups, std::ostream & os) {
   Scheduler s(people, groups);
   BOOST_CHECK(1 == s.solve(Scheduler::ONE));
   BOOST_CHECK(1 == s.solutions().size());

   print_schedules(s.solutions(), os);
}

static void test_scheduler(std::ostream & os) {
   test_all_scheduler(0,  0,  0, os);
   test_all_scheduler(1,  0,  0, os);
   test_all_scheduler(0,  1,  0, os);

   test_all_scheduler(1,  1,  1, os);
   test_all_scheduler(1,  2,  0, os);
   test_all_scheduler(1,  3,  0, os);
   test_all_scheduler(1,  4,  0, os);
   test_all_scheduler(1,  5,  0, os);

   test_all_scheduler(2,  1,  1, os);
   test_all_scheduler(2,  2,  1, os);
   test_all_scheduler(2,  3,  2, os);
   test_one_scheduler(2,  4, os);
   test_one_scheduler(2,  5, os);
   test_one_scheduler(2,  6, os);
   test_one_scheduler(2,  7, os);
   test_one_scheduler(2,  8, os);
   test_one_scheduler(2,  9, os);
   test_one_scheduler(2, 10, os);

   test_all_scheduler(3,  1,  1, os);
   test_all_scheduler(3,  2,  0, os);
   test_all_scheduler(3,  3, 12, os);
   test_all_scheduler(3,  4,  0, os);
   //test_one_scheduler(3,  5, os); // for now this one takes too long to run after each compile

   test_all_scheduler(4,  1,  1, os);
   test_all_scheduler(4,  2,  0, os);
   test_all_scheduler(4,  3,  0, os);
   test_one_scheduler(4,  4, os);
   test_all_scheduler(4,  5,  0, os);

   test_all_scheduler(5,  1,  1, os);
   test_all_scheduler(5,  2,  0, os);
   test_all_scheduler(5,  3,  0, os);
   test_all_scheduler(5,  4,  0, os);
   test_one_scheduler(5,  5, os);
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
