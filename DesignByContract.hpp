#ifndef DESIGNBYCONTRACT_HPP
#define DESIGNBYCONTRACT_HPP

#include <iostream>

#ifndef DEBUG
#define ASSERT(x)
#define FAIL_WITH_MESSAGE(x);
#else
#define ASSERT(x) \
   if (! (x)) \
   { \
      std::cerr << "*** Debug Assert: \"" << #x << "\" failed"; \
      std::cerr << " in " << __FILE__ << "(" <<  __LINE__ << ")" << std::endl; \
   }

#define FAIL_WITH_MESSAGE(x) \
   std::cerr << "*** Debug Assert: \"" << x << "\" failed"; \
   std::cerr << " in " << __FILE__ << "(" <<  __LINE__ << ")" << std::endl;

#endif

#endif
