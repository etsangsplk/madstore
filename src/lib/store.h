#ifndef _MAD_STORE_H_
#define _MAD_STORE_H_

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <cstring>
#include "easylogging++.h"
#include "base.h"
#include "iterables_map.h"
#include "dims.h"
#include "records.h"
#include "store_spec.h"
#include "watermarks.h"

template<typename Store>
struct QueryEngine;

template<uint8_t DimsCount,uint8_t MetricsCount>
struct Store: BaseStore {

  static const uint8_t DIMS_COUNT = DimsCount;
  static constexpr char const* FILE_NAME = "meta.mad";

  using DimCodes = std::array<DimCodeType,DimsCount>;
  using Metrics = MetricsArray<MetricsCount>;
  using Record = std::pair<DimCodes,Metrics>;
#ifdef PERSIST
  using Records = PersistentRecords<Record,Metrics>;
#else
  using Records = InMemoryRecords<Record,Metrics>;
#endif

  StoreSpec& spec;
  Records records;
  IterablesMap<DimCodes,offset_t> record_offsets;
  DimDict<DimsCount> dict;
  std::array<Watermarks,DimsCount> watermarks;

  Store(StoreSpec& spec):spec(spec),dict(spec.dims) {
    DimCodes empty;
    for (uint8_t i = 0; i < DimsCount; ++i) {
      empty[i] = -1;

      auto watermark_step = spec.GetWatermarkStep(i);
      if (watermark_step > 0) {
        watermarks[i] = Watermarks(watermark_step);
      }
    }
    record_offsets.set_empty_key(empty);

#ifdef PERSIST
    if (!UnPersist()) {
      CLOG(WARNING, "Store")<<"Can't load store!";
    }
#endif
  }

  Store(const Store& that) = delete;

  void Upsert(UpsertSpec& spec, std::vector<std::string>& values) {
    std::array<std::string,DimsCount> dims;
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
      auto offset = records.AddRecord(Record(codes, metrics));
      record_offsets[codes] = offset;

      for (auto & d : spec.watermark_dims) {
        watermarks[d].Create(offset, codes[d]);
      }
    } else {
      records.UpdateRecord(it->second, metrics);
    }
  }

  void GetStats(json& stats) {
    records.GetStats(stats);
    unsigned long offsets_usage = record_offsets.size() * (sizeof(DimCodes) + sizeof(offset_t)) * 1.78;
    stats["records_usage_mb"] = stats["records_usage_mb"].get<int>() + offsets_usage / (1024 * 1024);
    dict.GetStats(stats);
  }

  BaseQueryEngine* CreateQueryEngine() {
    return new QueryEngine<Store>(*this, records);
  }

  void OptimizeMemUsage() {
    records.OptimizeMemUsage();
  }

#ifdef PERSIST
  bool Persist() {
    remove(FILE_NAME);
    FILE* fp = fopen(FILE_NAME, "wb");
    if (fp == nullptr) {
      return false;
    }
    if (!dict.SaveToFile(fp)) {
      fclose(fp);
      return false;
    }
    for (auto & w : watermarks) {
      if (!w.SaveToFile(fp)) {
        fclose(fp);
        return false;
      }
    }
    fclose(fp);
    return true;
  }

  bool UnPersist() {
    FILE* fp = fopen(FILE_NAME, "rb");
    if (fp == nullptr) {
      return true;
    }
    if (!dict.LoadFromFile(fp)) {
      fclose(fp);
      return false;
    }
    for (auto & w : watermarks) {
      if (!w.LoadFromFile(fp)) {
        fclose(fp);
        return false;
      }
    }
    fclose(fp);
    {
      TIMED_SCOPE(timerObj, "rebuilding indexes");
      auto volumes = records.GetVolumes(0);
      offset_t offset = 0;
      for (auto volume : volumes) {
        auto records = volume.second;
        for (offset_t i = 0; i < records->size(); ++i) {
          auto & record = (*records)[i];
          record_offsets[record.first] = offset++;
        }
      }
    }
    return true;
  }
#endif /* PERSIST */
};

#endif /* _MAD_STORE_H_ */
