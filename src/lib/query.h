#ifndef _MAD_QUERY_H_
#define _MAD_QUERY_H_

#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include "iterables_map.h"
#include "table.h"
#include "materializer.h"
#include "json.hpp"

static inline void prefetch_range(const void *addr, size_t len) {
#ifdef ARCH_H_AS_PREFETCH
  char *end = addr + len;
  for (char* cp = addr; cp < end; cp += PREFETCH_STRIDE)
    __builtin_prefetch(cp, 0, 0)
#endif
}

using json = nlohmann::json;

template<typename Table>
struct QueryEngine: BaseQueryEngine {

  static const auto DIMS_COUNT = Table::DIMS_COUNT;

  using DimCodes = typename Table::DimCodes;
  using Metrics = typename Table::Metrics;
  using Record = typename Table::Record;
  using Records = typename Table::Records;
  using Result = std::vector<std::pair<std::vector<std::string>,Metrics>>;
  using GroupedMetrics = IterablesMap<std::vector<DimCodeType>,Metrics>;

  Table& table;
  Records& records;

  QueryEngine(Table& table, Records& records):table(table),records(records) {}

  struct Filter {
    Table &table;
    std::array<DimCodeType,DIMS_COUNT> watermark_vals;

    Filter(Table& table): table(table), watermark_vals({}) {}
    virtual ~Filter() {}
    virtual bool Apply(const DimCodes& dims) const = 0;
    virtual int GetPrecedence() const = 0;
  };

  struct Query {
    Table& table;
    Records& records;
    const Filter* filter;

    Query(Table& table, Records& records, const Filter* filter):
      table(table),records(records),filter(filter) {}

    virtual ~Query() {
      delete filter;
    }
    virtual void Run(Result& result) = 0;
  };

  struct GroupByQuery: Query {
    Materializer<Table>* materializer;

    GroupByQuery(Table& table, json& query_spec, Records& records, Filter* filter):
      materializer(Materializer<Table>::Create(table, query_spec)),Query(table, records, filter) {}

    ~GroupByQuery() {
      delete materializer;
    }

    void Run(Result& result) {
      auto& column_indices = materializer->GetColumnIndices();
      uint8_t dims_count = column_indices.size();
      GroupedMetrics grouped_metrics;
      std::vector<DimCodeType> empty(dims_count, -1);
      grouped_metrics.set_empty_key(empty);

      offset_t start_offset = 0;
      std::vector<offset_t> watermark_offsets;
      watermark_offsets.reserve(DIMS_COUNT);
      for (uint8_t d = 0; d < DIMS_COUNT; ++d) {
        const DimCodeType& watermark_value = Query::filter->watermark_vals[d];
        if (watermark_value > 0) {
          watermark_offsets.push_back(Query::table.watermarks[d].GetOffset(watermark_value));
        }
      }
      if (!watermark_offsets.empty()) {
        start_offset = *std::min_element(watermark_offsets.begin(), watermark_offsets.end());
      }

      auto volumes = Query::records.GetVolumes(start_offset);
      for (auto volume : volumes) {
        auto offset = volume.first;
        auto records = volume.second;
        auto records_num = records->size();
        CLOG(DEBUG, "Query")<<"Iterating volume "<<static_cast<void*>(records)<<" from offset: "<<offset;
        for (; offset < records_num; ++offset) {
          const auto & record = (*records)[offset];
          const auto & row_dims = record.first;
          if (Query::filter->Apply(row_dims)) {
            std::vector<DimCodeType> v(dims_count);
            for (uint8_t i = 0; i < dims_count; ++i) {
              v[i] = row_dims[column_indices[i]];
            }
            grouped_metrics[v] += record.second;
          }
          prefetch_range(&record + sizeof(Record), 10);
        }
      }

      materializer->Materialize(grouped_metrics, result);
    }
  };

  struct EqualsFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    EqualsFilter(Table &table, const uint8_t& dim_index, const std::string& value)
      :Filter(table),dim_index(dim_index) {
      value_code = table.dict.GetCode(dim_index, value);

      if (table.spec.GetWatermarkStep(dim_index) > 0) {
        Filter::watermark_vals[dim_index] = value_code;
      }
    }

    ~EqualsFilter() {}

    bool Apply(const DimCodes &dims) const {
      return dims[dim_index] == value_code;
    }

    int GetPrecedence() const {
      return 1;
    }
  };

  struct InFilter: Filter {
    uint8_t dim_index;
    std::unordered_set<DimCodeType> value_codes;

    InFilter(Table &table, const uint8_t& dim_index, const std::vector<std::string>& values)
      :Filter(table),dim_index(dim_index) {

      for (auto & value : values) {
        DimCodeType value_code = table.dict.GetCode(dim_index, value);
        value_codes.insert(value_code);
      }

      if (table.spec.GetWatermarkStep(dim_index) > 0) {
        Filter::watermark_vals[dim_index] = *std::min_element(value_codes.begin(), value_codes.end());
      }
    }

    ~InFilter() {}

    bool Apply(const DimCodes &dims) const {
      return value_codes.find(dims[dim_index]) != value_codes.end();
    }

    int GetPrecedence() const {
      return 4;
    }
  };

  struct GreaterThanFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    GreaterThanFilter(Table &table, uint8_t dim_index, const std::string& value)
      :Filter(table),dim_index(dim_index) {
      table.dict.Encode(dim_index, value, value_code);

      if (table.spec.GetWatermarkStep(dim_index) > 0) {
        Filter::watermark_vals[dim_index] = value_code;
      }
    }

    ~GreaterThanFilter() {}

    bool Apply(const DimCodes &dims) const {
      return dims[dim_index] > value_code;
    }

    int GetPrecedence() const {
      return 1;
    }
  };

  struct LessThanFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    LessThanFilter(Table &table, uint8_t dim_index, const std::string& value)
      :Filter(table),dim_index(dim_index) {
      table.dict.Encode(dim_index, value, value_code);
    }

    ~LessThanFilter() {}

    bool Apply(const DimCodes &dims) const {
      return dims[dim_index] < value_code;
    }

    int GetPrecedence() const {
      return 1;
    }
  };

  struct LogicalFilter: Filter {
    enum Op { And, Or };

    Op operation;
    const std::vector<Filter*> filters;

    LogicalFilter(Table &table, Op operation, const std::vector<Filter*>& filters)
      :Filter(table),operation(operation),filters(filters) {
      
      for (uint8_t i = 0; i < DIMS_COUNT; ++i) {
        std::vector<DimCodeType> watermark_vals;
        watermark_vals.reserve(filters.size());
        for (auto filter : filters) {
          watermark_vals.push_back(filter->watermark_vals[i]);
        }
        Filter::watermark_vals[i] = operation == Op::And ?
          *std::max_element(watermark_vals.begin(), watermark_vals.end())
          : *std::min_element(watermark_vals.begin(), watermark_vals.end());
      }
    }
    
    ~LogicalFilter() {
      for (auto filter : filters) {
        delete filter;
      }
    }

    bool Apply(const DimCodes &dims) const {
      switch (operation) {
        case Op::And:
          for (auto filter : filters) {
            if (!filter->Apply(dims)) {
              return false;
            }
          }
          return true;
        case Op::Or:
          for (auto filter : filters) {
            if (filter->Apply(dims)) {
              return true;
            }
          }
          return false;
      }
    }

    int GetPrecedence() const {
      return operation == Op::And ? 2 : 3;
    }
  };

  struct NotFilter: Filter {
    const Filter* filter;

    NotFilter(Table &table, const Filter* filter)
      :Filter(table),filter(filter) {
    }
    
    ~NotFilter() {
      delete filter;
    }

    bool Apply(const DimCodes &dims) const {
      return !filter->Apply(dims);
    }

    int GetPrecedence() const {
      return filter->GetPrecedence();
    }
  };

  struct FilterBuilder {
    Table &table;

    FilterBuilder(Table &table): table(table) {}

    Filter* Build(json& filter_spec) {
      std::string op = filter_spec["operator"];
      if (op == "and" || op == "or") {
        std::vector<Filter*> filters;
        for (auto & filter : filter_spec["filters"]) {
          filters.push_back(Build(filter));
        }
        sort(filters.begin(), filters.end(), [] (const Filter* a, const Filter* b) -> bool { 
          return a->GetPrecedence() < b->GetPrecedence();
        });
        return new LogicalFilter(table,
            op == "and" ? LogicalFilter::Op::And : LogicalFilter::Op::Or, filters);
      }
      if (op == "not") {
        return new NotFilter(table, Build(filter_spec["filter"]));
      }
      if (op == "equals") {
        std::string column = filter_spec["column"];
        return new EqualsFilter(table, table.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "greater") {
        std::string column = filter_spec["column"];
        return new GreaterThanFilter(table, table.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "less") {
        std::string column = filter_spec["column"];
        return new LessThanFilter(table, table.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "in") {
        std::string column = filter_spec["column"];
        return new InFilter(table, table.spec.GetDimIndex(column),
            filter_spec["values"].get<std::vector<std::string>>());
      }
      throw std::invalid_argument("Unknown filter operation: " + op);
    }
  };

  struct QueryBuilder {
    Table &table;
    Records& records;

    QueryBuilder(Table &table, Records& records): table(table),records(records) {}

    Query* Build(json& query_spec) {
      if (query_spec.find("filter") == query_spec.end()) {
        throw std::invalid_argument("Filter must be specified");
      }
      FilterBuilder filter_builder(table);
      Filter* filter = filter_builder.Build(query_spec["filter"]);
      std::string type = query_spec["type"];
      if (type == "groupBy") {
        return new GroupByQuery(table, query_spec, records, filter);
      }
      throw std::invalid_argument("Unknown query type: " + type);
    }
  };

  void RunQuery(json& query_spec, Result& result) {
    QueryBuilder builder(table, records);
    Query* query = builder.Build(query_spec);
    query->Run(result);
    delete query;
  }

  void RunQuery(json& query_spec, std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>>& result) {
    Result internal_result;
    RunQuery(query_spec, internal_result);
    for (auto & r : internal_result) {
      result.push_back(
          std::pair<std::vector<std::string>,std::vector<MetricType>>(
            r.first, std::vector<MetricType>(r.second.begin(), r.second.end())));
    }
  }
};

#endif /* _MAD_QUERY_H_ */
