#ifndef MAD_TYPES_H
#define MAD_TYPES_H

#include <stdint.h>
#include <google/dense_hash_map>
#include <map>
#include "json.hpp"

using google::dense_hash_map;
using namespace std;

template<typename T>
struct IterableHash {
  inline size_t operator()(const T& iterable) const {
    size_t value = 0x34578;
    for (auto const& e : iterable) {
      value = (1000003 * value) ^ e;
    }
    return value;
  }
};

template<typename T>
struct Eq {
  inline bool operator()(const T& arg1, const T& arg2) const {
    return arg1 == arg2;
  }
};

template<size_t T>
struct IntArray: array<uint32_t,T> {
  using array<uint32_t,T>::array;
};

struct IntVector: vector<uint32_t> {
  using vector<uint32_t>::vector;
};

template<typename T,typename V>
struct IterablesMap: dense_hash_map<T,V,IterableHash<T>,Eq<T>> {};

using json = nlohmann::json;

#endif /* MAD_TYPES_H */
