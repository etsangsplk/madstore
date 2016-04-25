#ifndef MAD_QUERY_H
#define MAD_QUERY_H

#include <stdexcept>
#include <set>
#include "types.h"
#include "store.h"

static inline void prefetch_range(const void *addr, size_t len) {
#ifdef ARCH_HAS_PREFETCH
  char *end = addr + len;
  for (char* cp = addr; cp < end; cp += PREFETCH_STRIDE)
    __builtin_prefetch(cp, 0, 0)
#endif
}

using namespace std;

template<typename MetricType>
struct BaseQueryEngine {
  virtual void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result) = 0;
};

template<typename Store>
struct QueryEngine: BaseQueryEngine<typename Store::MetricType_> {

  using DimCodes = typename Store::DimCodes;
  using DimCodeType = typename Store::DimCodeType_;
  using Metrics = typename Store::Metrics;
  using MetricType = typename Store::MetricType_;
  using Result = vector<pair<vector<string>,Metrics>>;

  Store& store;

  QueryEngine(Store& store):store(store) {}

  struct Filter {
    Store &store;

    Filter(Store& store): store(store) {}
    virtual ~Filter() {}
    virtual bool Apply(const DimCodes& dims) const = 0;
  };

  struct Query {
    Store& store;
    const Filter* filter;

    Query(Store& store, const Filter* filter):
      store(store),filter(filter) {}

    virtual ~Query() {
      delete filter;
    }
    virtual void Run(Result& result) = 0;
  };

  struct GroupByQuery: Query {
    vector<uint8_t> column_indices;

    GroupByQuery(vector<uint8_t> column_indices, Store& store, Filter* filter):
      column_indices(column_indices), Query(store, filter) {}

    void Run(Result& result) {
      uint8_t dims_count = column_indices.size();
      IterablesMap<vector<DimCodeType>,Metrics> grouped_metrics;
      vector<DimCodeType> empty(dims_count, -1);
      grouped_metrics.set_empty_key(empty);

      for(const auto & r : Query::store.records) {
        const DimCodes& row_dims = r.first;
        if (!Query::filter || Query::filter->Apply(row_dims)) {
          vector<DimCodeType> v(dims_count);
          for (uint8_t i = 0; i < dims_count; ++i) {
            v[i] = row_dims[column_indices[i]];
          }
          grouped_metrics[v] += r.second;
        }
        prefetch_range(&r + sizeof(pair<DimCodes,Metrics>), 10);
      }

      // Translate dimensions back to original values:
      for (const auto & r : grouped_metrics) {
        vector<string> v(dims_count);
        for (int i = 0; i < dims_count; ++i) {
          Query::store.dict.Decode(column_indices[i], r.first[i], v[i]);
        }
        result.push_back(pair<vector<string>,Metrics>(v, r.second));
      }
    }
  };

  struct EqualsFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    EqualsFilter(Store &store, const uint8_t& dim_index, const string& value)
      :Filter(store),dim_index(dim_index) {
      value_code = store.dict.GetCode(dim_index, value);
    }

    ~EqualsFilter() {}

    bool Apply(const DimCodes &dims) const {
      return dims[dim_index] == value_code;
    }
  };

  struct InFilter: Filter {
    uint8_t dim_index;
    set<DimCodeType> value_codes;

    InFilter(Store &store, const uint8_t& dim_index, const vector<string>& values)
      :Filter(store),dim_index(dim_index) {
      for (auto & value : values) {
        value_codes.insert(store.dict.GetCode(dim_index, value));
      }
    }

    ~InFilter() {}

    bool Apply(const DimCodes &dims) const {
      return value_codes.find(dims[dim_index]) != value_codes.end();
    }
  };

  struct GreaterThanFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    GreaterThanFilter(Store &store, uint8_t dim_index, const string& value)
      :Filter(store),dim_index(dim_index) {
      value_code = stoi(value);
    }

    ~GreaterThanFilter() {}

    bool Apply(const DimCodes &dims) const {
      return dims[dim_index] > value_code;
    }
  };

  struct LessThanFilter: Filter {
    uint8_t dim_index;
    DimCodeType value_code;

    LessThanFilter(Store &store, uint8_t dim_index, const string& value)
      :Filter(store),dim_index(dim_index) {
      value_code = stoi(value);
    }

    ~LessThanFilter() {}

    bool Apply(const DimCodes &dims) const {
      return dims[dim_index] < value_code;
    }
  };

  struct LogicalFilter: Filter {
    enum Op { And, Or };

    Op operation;
    const vector<Filter*> filters;

    LogicalFilter(Store &store, Op operation, const vector<Filter*>& filters)
      :Filter(store),operation(operation),filters(filters) {}
    
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
  };

  struct NotFilter: Filter {
    const Filter* filter;

    NotFilter(Store &store, const Filter* filter)
      :Filter(store),filter(filter) {}
    
    ~NotFilter() {
      delete filter;
    }

    bool Apply(const DimCodes &dims) const {
      return !filter->Apply(dims);
    }
  };

  struct FilterBuilder {
    Store &store;

    FilterBuilder(Store &store): store(store) {}

    Filter* Build(json& filter_spec) {
      string op = filter_spec["operator"];
      if (op == "and" || op == "or") {
        vector<Filter*> filters;
        for (auto & filter : filter_spec["filters"]) {
          filters.push_back(Build(filter));
        }
        return new LogicalFilter(store,
            op == "and" ? LogicalFilter::Op::And : LogicalFilter::Op::Or, filters);
      }
      if (op == "not") {
        return new NotFilter(store, Build(filter_spec["filter"]));
      }
      if (op == "equals") {
        string column = filter_spec["column"];
        return new EqualsFilter(store, store.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "greater") {
        string column = filter_spec["column"];
        return new GreaterThanFilter(store, store.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "less") {
        string column = filter_spec["column"];
        return new LessThanFilter(store, store.spec.GetDimIndex(column), filter_spec["value"]);
      }
      if (op == "in") {
        string column = filter_spec["column"];
        return new InFilter(store, store.spec.GetDimIndex(column),
            filter_spec["values"].get<vector<string>>());
      }
      throw invalid_argument("Unknown filter operation: " + op);
    }
  };

  struct QueryBuilder {
    Store &store;

    QueryBuilder(Store &store): store(store) {}

    Query* Build(json& query_spec) {
      Filter* filter = nullptr;
      if (query_spec.find("filter") != query_spec.end()) {
        FilterBuilder filter_builder(store);
        filter = filter_builder.Build(query_spec["filter"]);
      }
      string type = query_spec["type"];
      if (type == "groupBy") {
        vector<string> columns = query_spec["columns"].get<vector<string>>();
        vector<uint8_t> column_indices;
        store.spec.GetDimIndices(columns, column_indices);
        return new GroupByQuery(column_indices, store, filter);
      }
      throw invalid_argument("Unknown query type: " + type);
    }
  };

  void RunQuery(json& query_spec, Result& result) {
    QueryBuilder builder(store);
    Query* query = builder.Build(query_spec);
    query->Run(result);
    delete query;
  }

  void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result) {
    Result internal_result;
    RunQuery(query_spec, internal_result);
    for (auto & r : internal_result) {
      result.push_back(
          pair<vector<string>,vector<MetricType>>(
            r.first, vector<MetricType>(r.second.begin(), r.second.end())));
    }
  }
};

#endif /* MAD_QUERY_H */
