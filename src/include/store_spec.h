#ifndef MAD_STORE_SPEC_H
#define MAD_STORE_SPEC_H

#include <stdint.h>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "types.h"
#include "dims.h"
#include "metrics.h"
#include "input_spec.h"

using namespace std;

struct StoreSpec {
  vector<Dim> dims;
  vector<Metric> metrics;

  StoreSpec(json& spec) {
    for (auto column : spec["dimensions"]) {
      Dim::ValueType type = Dim::ValueType::String;
      uint32_t watermark_step = 0;
      if (column.find("type") != column.end()) {
        if (column["type"] == "integer") {
          type = Dim::ValueType::Integer;
          if (column.find("watermark_step") != column.end()) {
            watermark_step = column["watermark_step"];
          }
        }
      }
      dims.push_back(Dim(type, column["name"], watermark_step));
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

  inline uint8_t DimsCount() { return dims.size(); }

  inline int GetDimIndex(string& column) {
    for (int i = 0; i < dims.size(); ++i) {
      if (dims[i].name == column) {
        return i;
      }
    }
    return -1;
  }

  inline void GetDimIndices(vector<string>& columns, vector<uint8_t>& indices) {
    indices.reserve(columns.size());
    for (auto & c : columns) {
      indices.push_back(GetDimIndex(c));
    }
  }

  inline int GetMetricIndex(string& column) {
    for (int i = 0; i < metrics.size(); ++i) {
      if (metrics[i].name == column) {
        return i;
      }
    }
    return -1;
  }

  inline uint8_t MetricsCount() { return metrics.size(); }

  inline uint32_t GetWatermarkStep(const uint8_t& dim_index) {
    return dims[dim_index].watermark_step;
  }
};

struct UpsertSpec {
  vector<uint8_t> dim_cols;
  vector<uint8_t> metric_cols;
  vector<uint8_t> watermark_dims;

  UpsertSpec(InputSpec& input_spec, StoreSpec& store_spec) {
    vector<string>& columns = input_spec.columns;
    uint8_t columns_num = columns.size();
    for (uint8_t i = 0; i < columns_num; ++i) {
      string &column = columns[i];
      int dim_index;
      if ((dim_index = store_spec.GetDimIndex(column)) != -1) {
        dim_cols.push_back(i);
        if (store_spec.GetWatermarkStep((uint8_t&)dim_index) > 0) {
          watermark_dims.push_back((uint8_t)dim_index);
        }
      } else if (store_spec.GetMetricIndex(column) != -1) {
        metric_cols.push_back(i);
      } else {
        throw invalid_argument("Unknown column: " + column);
      }
    }

    assert(dim_cols.size() == store_spec.DimsCount());
    assert(metric_cols.size() <= store_spec.MetricsCount());
  }
};

#endif /* MAD_STORE_SPEC_H */
