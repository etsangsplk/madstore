#ifndef _MAD_STORE_SPEC_H_
#define _MAD_STORE_SPEC_H_

#include <stdint.h>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include "json.hpp"
#include "dims.h"
#include "metrics.h"
#include "input_spec.h"

using json = nlohmann::json;

struct StoreSpec {
  std::vector<Dim> dims;
  std::vector<Metric> metrics;

  StoreSpec(json& spec);
  inline uint8_t DimsCount() { return dims.size(); }

  inline int GetDimIndex(std::string& column) {
    for (int i = 0; i < dims.size(); ++i) {
      if (dims[i].name == column) {
        return i;
      }
    }
    return -1;
  }

  inline void GetDimIndices(std::vector<std::string>& columns, std::vector<uint8_t>& indices) {
    indices.reserve(columns.size());
    for (auto & c : columns) {
      indices.push_back(GetDimIndex(c));
    }
  }

  inline int GetMetricIndex(std::string& column) {
    for (int i = 0; i < metrics.size(); ++i) {
      if (metrics[i].name == column) {
        return i;
      }
    }
    return -1;
  }

  inline uint8_t MetricsCount() { return metrics.size(); }

  inline offset_t GetWatermarkStep(const uint8_t& dim_index) {
    return dims[dim_index].watermark_step;
  }
};

struct UpsertSpec {
  std::vector<uint8_t> dim_cols;
  std::vector<uint8_t> metric_cols;
  std::vector<uint8_t> watermark_dims;

  UpsertSpec(InputSpec& input_spec, StoreSpec& store_spec);
};

#endif /* _MAD_STORE_SPEC_H_ */
