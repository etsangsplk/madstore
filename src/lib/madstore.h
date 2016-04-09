#ifndef MADSTORE_H
#define MADSTORE_H

#include <stdint.h>
#include <google/dense_hash_map>
#include <vector>
#include <cstring>

using google::dense_hash_map;
using namespace std;

template<size_t Dim_Count,typename Metric_Type,size_t Metric_Count>
struct Store {

  typedef array<string,Dim_Count> Dims;

  struct Dims_Trans: array<uint32_t,Dim_Count> {
    typedef struct {
      inline size_t operator()(const Dims_Trans& dims) const {
        size_t value = 0x34578;
        for (auto d : dims) {
          value = (1000003 * value) ^ d;
        }
        value ^= Dim_Count;
        return value;
      }
    } Hash;

    typedef struct {
      inline bool operator()(const Dims_Trans& dims1, const Dims_Trans& dims2) const {
        return dims1 == dims2;
      }
    } Eq;
  };

  struct Metrics: array<Metric_Type,Metric_Count> {
    inline const Metrics& operator += (const Metrics &other) {
      for (int i = 0; i < Metric_Count; ++i) {
        (*this)[i] += other[i];
      }
      return (*this);
    }
  };

  struct Dictionary: dense_hash_map<string,uint32_t> {
    Dictionary() {
      string empty;
      empty.push_back(1);
      set_empty_key(empty);
    }
  };

  typedef dense_hash_map<Dims_Trans,Metrics,typename Dims_Trans::Hash,typename Dims_Trans::Eq> Record_Map;

  size_t dims = Dim_Count;
  size_t metrics = Metric_Count;
  array<Dictionary,Dim_Count> dicts;
  Record_Map records;

  Store() {
    records.set_empty_key(Dims_Trans());
  }

  void upsert(Dims &dims, const Metrics &metrics) {
    Dims_Trans tr;

    // Translate dimensions:
    int i = 0;
    for (int i = 0; i < Dim_Count; ++i) {
      string &dim = dims[i];
      Dictionary &dict = dicts[i];
      typename dense_hash_map<string,uint32_t>::const_iterator existing = dict.find(dim);
      if (existing == dict.end()) {
        tr[i] = dict[dim] = dict.size();
      } else {
        tr[i] = existing->second;
      }
    }

    // Increment or insert new metrics:
    records[tr] += metrics;
  }
};

#endif /* MADSTORE_H */
