#ifndef MAD_QUERY_H
#define MAD_QUERY_H

#include <stdexcept>
#include <unordered_set>
#include <algorithm>
#include "iterables_map.h"
#include "store.h"
#include "../3rdparty/json.hpp"

static inline void prefetch_range(const void *addr, size_t len) {
#ifdef ARCH_HAS_PREFETCH
  char *end = addr + len;
  for (char* cp = addr; cp < end; cp += PREFETCH_STRIDE)
    __builtin_prefetch(cp, 0, 0)
#endif
}

using json = nlohmann::json;

template<typename Store>
struct QueryEngine: BaseQueryEngine {

  static const auto DIMS_COUNT = Store::DIMS_COUNT;

  using DimCodes = typename Store::DimCodes;
  using Metrics = typename Store::Metrics;
  using Result = std::vector<std::pair<std::vector<std::string>,Metrics>>;
  using Record = typename Store::Record;
  using Records = typename Store::Records;

  Store& store;
  Records& records;

  QueryEngine(Store& store, Records& records):store(store),records(records) {}

  struct Filter {
    Store &store;
    std::array<DimCodeType,DIMS_COUNT> watermark_vals;

    Filter(Store& store): store(store), watermark_vals({}) {}
    virtual ~Filter() {}
    virtual bool Apply(const DimCodes& dims) const = 0;
    virtual int GetPrecedence() const = 0;
  };

  struct Query {
    Store& store;
    Records& records;
    const Filter* filter;

    Query(Store& store, Records& records, const Filter* filter):
      store(store),records(records),filter(filter) {}

    virtual ~Query() {
      delete filter;
    }
    virtual void Run(Result& result) = 0;
  };

  struct GroupByQuery: Query {
    std::vector<uint8_t> column_indices;

    GroupByQuery(std::vector<uint8_t> column_indices, Store& store,
        Records& records, Filter* filter):
      column_indices(column_indices), Query(store, records, filter) {}

    void Run(Result& result) {
      uint8_t dims_count = column_indices.size();
      IterablesMap<std::vector<DimCodeType>,Metrics> grouped_metrics;
      std::vector<DimCodeType> empty(dims_count, -1);
      grouped_metrics.set_empty_key(empty);

      offset_t start_offset = 0;
      if (Query::filter != nullptr) {
        std::vector<offset_t> watermark_offsets;
        watermark_offsets.reserve(DIMS_COUNT);
        for (uint8_t d = 0; d < DIMS_COUNT; ++d) {
          const DimCodeType& watermark_value = Query::filter->watermark_vals[d];
          if (watermark_value > 0) {
            watermark_offsets.push_back(Query::store.watermarks[d].GetOffset(watermark_value));
          }
        }
        if (!watermark_offsets.empty()) {
          start_offset = *std::min_element(watermark_offsets.begin(), watermark_offsets.end());
        }
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
          if (!Query::filter || Query::filter->Apply(row_dims)) {
            std::vector<DimCodeType> v(dims_count);
            for (uint8_t i = 0; i < dims_count; ++i) {
              v[i] = row_dims[column_indices[i]];
            }
            grouped_metrics[v] += record.second;
          }
          prefetch_range(&record + sizeof(Record), 10);
        }
      }

      // Translate dimensions back to original values:
      for (const auto & r : grouped_metrics) {
        std::vector<std::string> v(dims_count);
        for (int i = 0; i < dims_count; ++i) {
          Query::store.dict.Decode(column_indices[i], r.first[i], v[i]);
        }
        result.push_back(std::pair<std::vector<std::string>,Metrics>(v, r.second));
      }
    }
  };

  struct EqualsFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    EqualsFilter(Store &store, const uint8_t& dim_index, const std::string& value)
      :Filter(store),dim_index(dim_index) {
      value_code = store.dict.GetCode(dim_index, value);

      if (store.spec.GetWatermarkStep(dim_index) > 0) {
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

    InFilter(Store &store, const uint8_t& dim_index, const std::vector<std::string>& values)
      :Filter(store),dim_index(dim_index) {

      for (auto & value : values) {
        DimCodeType value_code = store.dict.GetCode(dim_index, value);
        value_codes.insert(value_code);
      }

      if (store.spec.GetWatermarkStep(dim_index) > 0) {
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

    GreaterThanFilter(Store &store, uint8_t dim_index, const std::string& value)
      :Filter(store),dim_index(dim_index) {
      store.dict.Encode(dim_index, value, value_code);

      if (store.spec.GetWatermarkStep(dim_index) > 0) {
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

    LessThanFilter(Store &store, uint8_t dim_index, const std::string& value)
      :Filter(store),dim_index(dim_index) {
      store.dict.Encode(dim_index, value, value_code);
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

    LogicalFilter(Store &store, Op operation, const std::vector<Filter*>& filters)
      :Filter(store),operation(operation),filters(filters) {
      
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

    NotFilter(Store &store, const Filter* filter)
      :Filter(store),filter(filter) {
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
    Store &store;

    FilterBuilder(Store &store): store(store) {}

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
        return new LogicalFilter(store,
            op == "and" ? LogicalFilter::Op::And : LogicalFilter::Op::Or, filters);
      }
      if (op == "not") {
        return new NotFilter(store, Build(filter_spec["filter"]));
      }
      if (op == "equals") {
        std::string column = filter_spec["column"];
        return new EqualsFilter(store, store.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "greater") {
        std::string column = filter_spec["column"];
        return new GreaterThanFilter(store, store.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "less") {
        std::string column = filter_spec["column"];
        return new LessThanFilter(store, store.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "in") {
        std::string column = filter_spec["column"];
        return new InFilter(store, store.spec.GetDimIndex(column),
            filter_spec["values"].get<std::vector<std::string>>());
      }
      throw std::invalid_argument("Unknown filter operation: " + op);
    }
  };

  struct QueryBuilder {
    Store &store;
    Records& records;

    QueryBuilder(Store &store, Records& records): store(store),records(records) {}

    Query* Build(json& query_spec) {
      Filter* filter = nullptr;
      if (query_spec.find("filter") != query_spec.end()) {
        FilterBuilder filter_builder(store);
        filter = filter_builder.Build(query_spec["filter"]);
      }
      std::string type = query_spec["type"];
      if (type == "groupBy") {
        std::vector<std::string> columns = query_spec["columns"].get<std::vector<std::string>>();
        std::vector<uint8_t> column_indices;
        store.spec.GetDimIndices(columns, column_indices);
        return new GroupByQuery(column_indices, store, records, filter);
      }
      throw std::invalid_argument("Unknown query type: " + type);
    }
  };

  void RunQuery(json& query_spec, Result& result) {
    QueryBuilder builder(store, records);
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

#endif /* MAD_QUERY_H */
