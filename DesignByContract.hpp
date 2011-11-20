#ifndef DESIGNBYCONTRACT_HPP
#define DESIGNBYCONTRACT_HPP

#include <iostream>

#ifndef DEBUG
#define ASSERT(x)
#else
#define ASSERT(x) \
   if (! (x)) \
   { \
      std::cerr << "*** Debug Assert: \"" << #x << "\" failed"; \
      std::cerr << " in " << __FILE__ << "(" <<  __LINE__ << ")" << std::endl; \
   }
#endif

#endif
