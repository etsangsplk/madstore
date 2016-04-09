#ifndef MAD_STORE_SPEC_H
#define MAD_STORE_SPEC_H

#include <stdint.h>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "types.h"
#include "dims.h"
#include "metrics.h"

using namespace std;

struct StoreSpec {
  vector<Dim> dims;
  vector<Metric> metrics;

  StoreSpec(json& spec) {
    for (auto column : spec["dimensions"]) {
      Dim::ValueType type = Dim::ValueType::String;
      if (column.find("type") != column.end()) {
        if (column["type"] == "integer") {
          type = Dim::ValueType::Integer;
        }
      }
      dims.push_back(Dim(type, column["name"]));
    }

    for (auto column : spec["metrics"]) {
      Metric::ValueType type = Metric::ValueType::Long;
      if (column.find("type") != column.end()) {
        if (column["type"] == "double") {
          type = Metric::ValueType::Double;
        }
      }
      metrics.push_back(Metric(type, column["name"]));
    }
  }

  inline uint32_t DimsCount() { return dims.size(); }

  inline uint32_t GetDimIndex(string& column) {
    for (uint32_t i = 0; i < dims.size(); ++i) {
      if (dims[i].name == column) {
        return i;
      }
    }
    return -1;
  }

  inline void GetDimIndices(vector<string>& columns, vector<uint32_t>& indices) {
    indices.reserve(columns.size());
    for (auto & c : columns) {
      indices.push_back(GetDimIndex(c));
    }
  }

  inline uint32_t GetMetricIndex(string& column) {
    for (uint32_t i = 0; i < metrics.size(); ++i) {
      if (metrics[i].name == column) {
        return i;
      }
    }
    return -1;
  }

  inline uint32_t MetricsCount() { return metrics.size(); }
};

#endif /* MAD_STORE_SPEC_H */
