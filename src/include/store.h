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

template<typename MetricType>
struct BaseQueryEngine;

template<typename Store>
struct QueryEngine;

template<typename DimCodeType,typename MetricType>
struct BaseStore {
  virtual void Upsert(vector<uint8_t>& dim_indexes, vector<uint8_t>& metric_indexes, vector<string>& fields) = 0;
  virtual void GetStats(json& stats) = 0;
  virtual BaseQueryEngine<MetricType>* CreateQueryEngine() = 0;
};

template<typename DimCodeType,size_t DimsCount,typename MetricType,size_t MetricsCount>
struct Store: BaseStore<DimCodeType,MetricType> {

  using DimCodes = array<DimCodeType,DimsCount>;
  using Metrics = MetricsArray<MetricType,MetricsCount>;
  using MetricType_ = MetricType;
  using DimCodeType_ = DimCodeType;

  StoreSpec& spec;
  vector<pair<DimCodes,Metrics>> records;
  IterablesMap<DimCodes,size_t> record_offsets;
  DimDict<DimCodeType,DimsCount> dict;

  Store(StoreSpec& spec):spec(spec),dict(spec.dims) {
    DimCodes empty;
    for (int i = 0; i < DimsCount; ++i) {
      empty[i] = -1;
    }
    record_offsets.set_empty_key(empty);
  }

  Store(const Store& that) = delete;

  void Upsert(array<string,DimsCount>& dims, const Metrics& metrics) {
    DimCodes codes;
    dict.Encode(dims, codes);
    auto it = record_offsets.find(codes);
    if (it == record_offsets.end()) {
      records.push_back(pair<DimCodes,Metrics>(codes, metrics));
      record_offsets[codes] = records.size() - 1;
    } else {
      records[it->second].second += metrics;
    }
  }

  void Upsert(vector<uint8_t>& dim_indexes, vector<uint8_t>& metric_indexes, vector<string>& fields) {
    array<string,DimsCount> dims;
    size_t dim_idx = 0;
    Metrics metrics = {};
    size_t metric_idx = 0; 

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

    unsigned long records_mem_usage = records.size() * (spec.DimsCount()*sizeof(DimCodeType) + spec.MetricsCount()*sizeof(MetricType));
    records_mem_usage += record_offsets.size() * (spec.DimsCount()*sizeof(DimCodeType) + sizeof(size_t));
    stats["records_usage_mb"] = records_mem_usage/(1024*1024);

    dict.GetStats(stats);
  }

  BaseQueryEngine<MetricType>* CreateQueryEngine() {
    return new QueryEngine<Store>(*this);
  }
};

#endif /* MAD_STORE_H */
