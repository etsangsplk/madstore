#ifndef MAD_TYPES_H
#define MAD_TYPES_H

#include <stdint.h>
#include <google/dense_hash_map>
#include <map>
#include "../3rdparty/json.hpp"

using google::dense_hash_map;
using namespace std;

template<typename T>
struct IterableHash {
  inline size_t operator()(const T& iterable) const {
    size_t hash = 0;
    for (const auto & e : iterable) {
      hash = hash * 31 + e;
    }
    return hash;
  }
};

template<typename T>
struct Eq {
  inline bool operator()(const T& arg1, const T& arg2) const {
    return arg1 == arg2;
  }
};

template<typename T,typename V>
struct IterablesMap: dense_hash_map<T,V,IterableHash<T>,Eq<T>> {};

using json = nlohmann::json;

#endif /* MAD_TYPES_H */
