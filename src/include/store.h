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
#include "watermarks.h"

using google::dense_hash_map;
using namespace std;

template<typename MetricType>
struct BaseQueryEngine;

template<typename Store>
struct QueryEngine;

template<typename DimCodeType,typename MetricType>
struct BaseStore {
  virtual void Upsert(UpsertSpec& spec, vector<string>& values) = 0;
  virtual void GetStats(json& stats) = 0;
  virtual BaseQueryEngine<MetricType>* CreateQueryEngine() = 0;
};

template<typename DimCodeType,size_t DimsCount,typename MetricType,size_t MetricsCount>
struct Store: BaseStore<DimCodeType,MetricType> {

  static const size_t DIMS_COUNT = DimsCount;

  using DimCodes = array<DimCodeType,DimsCount>;
  using Metrics = MetricsArray<MetricType,MetricsCount>;
  using MetricType_ = MetricType;
  using DimCodeType_ = DimCodeType;
  using Record = pair<DimCodes,Metrics>;

  StoreSpec& spec;
  vector<Record> records;
  IterablesMap<DimCodes,size_t> record_offsets;
  DimDict<DimCodeType,DimsCount> dict;
  array<Watermarks<DimCodeType>,DimsCount> watermarks;

  Store(StoreSpec& spec):spec(spec),dict(spec.dims) {
    DimCodes empty;
    for (uint8_t i = 0; i < DimsCount; ++i) {
      empty[i] = -1;

      auto watermark_step = spec.GetWatermarkStep(i);
      if (watermark_step > 0) {
        watermarks[i] = Watermarks<DimCodeType>(watermark_step);
      }
    }
    record_offsets.set_empty_key(empty);
  }

  Store(const Store& that) = delete;

  void Upsert(UpsertSpec& spec, vector<string>& values) {
    array<string,DimsCount> dims;
    size_t dim_idx = 0;
    Metrics metrics = {};
    size_t metric_idx = 0; 

    for (auto dim_column : spec.dim_cols) {
      dims[dim_idx++] = values[dim_column];
    }
    for (auto metric_column : spec.metric_cols) {
      metrics[metric_idx++] = stoi(values[metric_column]);
    }

    DimCodes codes;
    dict.Encode(dims, codes);

    auto it = record_offsets.find(codes);
    if (it == record_offsets.end()) {
      records.push_back(Record(codes, metrics));
      size_t offset = records.size() - 1;
      record_offsets[codes] = offset;

      for (auto & d : spec.watermark_dims) {
        watermarks[d].Create(offset, codes[d]);
      }
    } else {
      records[it->second].second += metrics;
    }
  }

  void GetStats(json& stats) {
    stats["records"] = to_string(records.size());

    unsigned long mem_usage = records.size() * sizeof(Record);
    unsigned long offsets_usage = record_offsets.size() * (sizeof(DimCodes) + sizeof(size_t));
    // Multiply by dense_hash_map overhead:
    mem_usage += offsets_usage * 1.78;
    stats["records_usage_mb"] = mem_usage/(1024*1024);

    dict.GetStats(stats);
  }

  BaseQueryEngine<MetricType>* CreateQueryEngine() {
    return new QueryEngine<Store>(*this);
  }
};

#endif /* MAD_STORE_H */
