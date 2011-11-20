#include "../Scheduler.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "C++ Unit Tests for Scheduler"
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp>

BOOST_AUTO_TEST_CASE(solve) {
   {
      Scheduler s(2, 2);
      BOOST_CHECK_EQUAL(2, s.solve());
   }

   {
      Scheduler s(2, 3);
      BOOST_CHECK_EQUAL(48, s.solve());
   }

   {
      Scheduler s(3, 3);
      BOOST_CHECK_EQUAL(72, s.solve());
   }
}
