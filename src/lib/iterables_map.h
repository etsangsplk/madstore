#ifndef ITERABLES_MAP_H
#define ITERABLES_MAP_H

#include <stdint.h>
#include <google/dense_hash_map>

using google::dense_hash_map;

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

#endif /* ITERABLES_MAP_H */
