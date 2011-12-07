#ifndef MATHUTILS_HPP
#define MATHUTILS_HPP

#include <map>

namespace MathUtils {

inline size_t fact(size_t n) {
   if(n == 1) return 1;
   return n * fact(n-1);
}

inline size_t fall(size_t n, size_t m) {
   if(m == 1) return n;
   return n * fall(n-1, m-1);
}


inline size_t comb(size_t n, size_t r) {
   return fall(n, r) / fact(r);
}

} // end namespace MathUtils

#endif // MATHUTILS_HPP
