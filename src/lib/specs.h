#ifndef _MAD_SPECS_H_
#define _MAD_SPECS_H_

#include <vector>
#include "json.hpp"
#include "dims.h"
#include "metrics.h"

using json = nlohmann::json;

struct InputSpec {
  enum Format {TSV};

  Format format;
  std::string file;
  std::string table;
  std::vector<std::string> columns;
  char field_delimiter;
  char row_delimiter;
  bool skip_bad_rows;

  InputSpec(json& spec);
};

struct TableSpec {
  std::string table;
  std::vector<Dim> dims;
  std::vector<Metric> metrics;

  TableSpec(const std::string& table, json& spec);

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
      auto index = GetDimIndex(c);
      if (index == -1) {
        throw std::invalid_argument("Column doesn't exist: " + c);
      }
      indices.push_back(index);
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

struct StoreSpec {
  std::map<std::string, std::unique_ptr<TableSpec>> tables;
  StoreSpec(json& spec);

  inline TableSpec& GetTable(std::string& table) {
    auto it = tables.find(table);
    if (it == tables.end()) {
      throw std::invalid_argument("No such table: " + table);
    }
    return *it->second.get();
  }
};

struct UpsertSpec {
  std::vector<uint8_t> dim_cols;
  std::vector<uint8_t> metric_cols;
  std::vector<uint8_t> watermark_dims;

  UpsertSpec(InputSpec& input_spec, StoreSpec& store_spec);
};

#endif /* _MAD_SPECS_H_ */
