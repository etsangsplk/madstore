#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

using namespace std;

template<class T, size_t N>
ostream& operator<<(ostream& o, const array<T, N>& arr) {
  copy(arr.cbegin(), arr.cend(), ostream_iterator<T>(o, " "));
  return o;
}

#endif /* PRINT_H */
