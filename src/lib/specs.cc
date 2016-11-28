#include <stdexcept>
#include "specs.h"

InputSpec::InputSpec(json& spec):field_delimiter('\t'),row_delimiter('\n') {
  file = spec["file"];
  table = spec["table"];
  columns = spec["columns"].get<std::vector<std::string>>();
  std::string fmt = spec["format"];
  if (fmt == "tsv") {
    format = Format::TSV;
    if (spec.find("field_delimiter") != spec.end()) {
      field_delimiter = spec["field_delimiter"].get<std::string>().at(0);
    }
    if (spec.find("row_delimiter") != spec.end()) {
      row_delimiter = spec["row_delimiter"].get<std::string>().at(0);
    }
  } else {
    throw std::invalid_argument("Unsupported format: " + std::to_string(format));
  }
  if (spec.find("skip_bad_rows") != spec.end()) {
    skip_bad_rows = spec["skip_bad_rows"];
  }
}

TableSpec::TableSpec(const std::string& table, json& spec):table(table) {
  for (auto column : spec["dimensions"]) {
    Dim::ValueType type = Dim::ValueType::String;
    offset_t watermark_step = 0;
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

StoreSpec::StoreSpec(json& spec) {
  json& j = spec["tables"];
  for (json::iterator it = j.begin(); it != j.end(); ++it) {
    tables[it.key()] = std::unique_ptr<TableSpec>(new TableSpec(it.key(), it.value()));
  }
}

UpsertSpec::UpsertSpec(InputSpec& input_spec, StoreSpec& store_spec) {
  TableSpec& table_spec = store_spec.GetTable(input_spec.table);

  std::vector<std::string>& columns = input_spec.columns;
  uint8_t columns_num = columns.size();
  for (uint8_t i = 0; i < columns_num; ++i) {
    std::string &column = columns[i];
    int dim_index;
    if ((dim_index = table_spec.GetDimIndex(column)) != -1) {
      dim_cols.push_back(i);
      if (table_spec.GetWatermarkStep((uint8_t&)dim_index) > 0) {
        watermark_dims.push_back((uint8_t)dim_index);
      }
    } else if (table_spec.GetMetricIndex(column) != -1) {
      metric_cols.push_back(i);
    } else {
      throw std::invalid_argument("Unknown column: " + column);
    }
  }

  assert(dim_cols.size() == table_spec.DimsCount());
  assert(metric_cols.size() <= table_spec.MetricsCount());
}

