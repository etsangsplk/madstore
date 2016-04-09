#ifndef MAD_STORE_H
#define MAD_STORE_H

#include <stdint.h>
#include <vector>
#include <cstring>
#include <google/dense_hash_map>
#include "types.h"
#include "dims.h"
#include "metrics.h"
#include "store_spec.h"

using google::dense_hash_map;
using namespace std;

template<size_t DimsCount,typename MetricType,size_t MetricsCount>
struct Store {

  using DimCodes = array<uint32_t,DimsCount>;
  using Metrics = MetricsArray<MetricType,MetricsCount>;
  using MetricType_ = MetricType;

  StoreSpec& spec;

#ifdef VECTOR_STORE
  vector<pair<DimCodes,Metrics>> records;
  IterablesMap<DimCodes,uint32_t> record_offsets;
#else
  IterablesMap<DimCodes,Metrics> records;
#endif

  DimDict<DimsCount> dict;

  Store(StoreSpec& spec):spec(spec),dict(spec.dims) {
    DimCodes empty;
    for (int i = 0; i < DimsCount; ++i) {
      empty[i] = -1;
    }
#ifdef VECTOR_STORE
    record_offsets.set_empty_key(empty);
#else
    records.set_empty_key(empty);
#endif
  }

  Store(const Store& that) = delete;

  void Upsert(array<string,DimsCount>& dims, const Metrics& metrics) {
    DimCodes codes;
    dict.Encode(dims, codes);
#ifdef VECTOR_STORE
    auto it = record_offsets.find(codes);
    if (it == record_offsets.end()) {
      records.push_back(pair<DimCodes,Metrics>(codes, metrics));
      record_offsets[codes] = records.size() - 1;
    } else {
      records[it->second].second += metrics;
    }
#else
    records[codes] += metrics;
#endif
  }

  void Upsert(vector<uint32_t>& dim_indexes, vector<uint32_t>& metric_indexes, vector<string>& fields) {
    array<string,DimsCount> dims;
    int dim_idx = 0;
    Metrics metrics = {};
    int metric_idx = 0; 

    for (auto d : dim_indexes) {
      dims[dim_idx++] = fields[d];
    }
    for (auto m : metric_indexes) {
      metrics[metric_idx++] = stoi(fields[m]);
    }
    Upsert(dims, metrics);
  }

  void GetStats(json& stats) {
    stats["records"] = to_string(records.size());

    unsigned long records_mem_usage = records.size() * (spec.DimsCount()*sizeof(uint32_t) + spec.MetricsCount()*sizeof(MetricType));
#ifdef VECTOR_STORE
    records_mem_usage += record_offsets.size() * (spec.DimsCount()*sizeof(uint32_t) + sizeof(uint32_t));
#endif
    stats["records_usage_mb"] = records_mem_usage/(1024*1024);

    dict.GetStats(stats);
  }
};

#endif /* MAD_STORE_H */
