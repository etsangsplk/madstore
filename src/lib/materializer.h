#ifndef _MAD_MATERIALIZER_H_
#define _MAD_MATERIALIZER_H_

#include <vector>
#include <string>
#include <set>
#include "json.hpp"
#include "store.h"
#ifdef EXPRESSIONS
# include "lua.h"
#endif

using json = nlohmann::json;

template<typename Store>
struct ColumnsMaterializer;

template<typename Store>
struct SelectMaterializer;

template<typename Store>
struct Materializer {
  using Metrics = typename Store::Metrics;
  using Result = std::vector<std::pair<std::vector<std::string>,Metrics>>;
  using GroupedMetrics = IterablesMap<std::vector<DimCodeType>,Metrics>;

  std::vector<uint8_t> column_indices;

  virtual ~Materializer() {}
  // Translate dimensions back to original values:
  virtual void Materialize(GroupedMetrics& grouped_metrics, Result& result) = 0;
  virtual std::vector<std::string>& GetColumnNames() = 0;

  std::vector<uint8_t>& GetColumnIndices() {
    return column_indices;
  }

  static Materializer* Create(Store& store, json& query_spec) {
    if (query_spec.find("columns") != query_spec.end()) {
      std::vector<std::string> columns = query_spec["columns"].get<std::vector<std::string>>();
      return new ColumnsMaterializer<Store>(store, columns);
    }
    if (query_spec.find("select") != query_spec.end()) {
#ifdef EXPRESSIONS
      return new SelectMaterializer<Store>(store, query_spec["select"]);
#else
      throw std::invalid_argument("Select expressions are not supported");
#endif
    }
    throw std::invalid_argument("Missing either 'columns' or 'select' section");
  }
};

template<typename Store>
struct ColumnsMaterializer: Materializer<Store> {

  Store& store;
  std::vector<std::string> columns;

  ColumnsMaterializer(Store& store, std::vector<std::string>& columns):store(store),columns(columns) {
    store.spec.GetDimIndices(columns, Materializer<Store>::column_indices);
  }

  void Materialize(typename Materializer<Store>::GroupedMetrics& grouped_metrics, typename Materializer<Store>::Result& result) {
    auto dims_count = Materializer<Store>::column_indices.size();
    for (const auto & r : grouped_metrics) {
      std::vector<std::string> v(dims_count);
      for (int i = 0; i < dims_count; ++i) {
        store.dict.Decode(Materializer<Store>::column_indices[i], r.first[i], v[i]);
      }
      result.push_back(std::pair<std::vector<std::string>,typename Store::Metrics>(v, r.second));
    }
  }

  std::vector<std::string>& GetColumnNames() {
    return columns;
  }
};

#ifdef EXPRESSIONS
template<typename Store>
struct SelectMaterializer: Materializer<Store> {

  Lua lua;
  Store& store;
  std::vector<std::string> output_columns;
  std::vector<const LuaFunction*> functions;
  std::vector<std::vector<uint8_t>> arg_indices;

  SelectMaterializer(Store& store, json& select):store(store),functions(select.size(), nullptr) {
    output_columns.reserve(select.size());
    arg_indices.resize(select.size());

    std::vector<std::vector<std::string>> arg_names;
    arg_names.resize(select.size());

    std::set<std::string> columns_set;
    uint8_t column_index = 0;
    for (auto s : select) {
      const std::string& name = s["name"];
      output_columns.push_back(name);

      if (s.find("expr") != s.end()) {
        std::vector<std::string> select_fields = s["fields"].get<std::vector<std::string>>();
        functions[column_index] = &lua.Compile(s["expr"], select_fields);
        arg_names[column_index] = select_fields;
        columns_set.insert(select_fields.begin(), select_fields.end());
      } else {
        columns_set.insert(name);
        arg_names[column_index].push_back(name);
      }
      column_index++;
    }

    std::vector<std::string> select_columns(columns_set.begin(), columns_set.end());
    store.spec.GetDimIndices(select_columns, Materializer<Store>::column_indices);

    for (uint8_t func_idx = 0; func_idx < arg_names.size(); ++func_idx) {
      auto args = arg_names[func_idx];
      for (uint8_t arg_idx = 0; arg_idx < args.size(); ++arg_idx) {
        for (uint8_t s = 0; s < select_columns.size(); ++s) {
          if (args[arg_idx] == select_columns[s]) {
            arg_indices[func_idx].push_back(s);
            break;
          }
        }
      }
    }
  }

  void Materialize(typename Materializer<Store>::GroupedMetrics& grouped_metrics, typename Materializer<Store>::Result& result) {
    std::map<std::vector<std::string>, typename Store::Metrics> post_agg;
    auto columns_num = output_columns.size();
    for (const auto & r : grouped_metrics) {
      auto& dim_codes = r.first;
      std::vector<std::string> values(columns_num);
      for (int column_idx = 0; column_idx < columns_num; ++column_idx) {
        store.dict.Decode(Materializer<Store>::column_indices[column_idx], dim_codes[column_idx], values[column_idx]);
      }

      std::vector<std::string> translated(columns_num);
      for (int column_idx = 0; column_idx < columns_num; ++column_idx) {
        const LuaFunction* function = functions[column_idx];
        if (function != nullptr) {
          std::vector<std::string> args;
          args.reserve(arg_indices[column_idx].size());
          for (auto& arg_idx : arg_indices[column_idx]) {
            args.push_back(values[arg_idx]);
          }
          translated[column_idx] = (*function)(args);
        } else {
          translated[column_idx] = values[arg_indices[column_idx][0]];
        }
      }

      post_agg[translated] += r.second;
    }

    for (auto it = post_agg.begin(); it != post_agg.end(); ++it) {
      result.push_back(std::pair<std::vector<std::string>,typename Store::Metrics>(it->first, it->second));
    }
  }

  std::vector<std::string>& GetColumnNames() {
    return output_columns;
  }
};
#endif /* EXPRESSIONS */

#endif /* _MAD_MATERIALIZER_H_ */
