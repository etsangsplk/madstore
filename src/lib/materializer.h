#ifndef MAD_MATERIALIZER_H
#define MAD_MATERIALIZER_H

#include <vector>
#include <string>
#include <set>
#include "../3rdparty/json.hpp"
#include "store.h"
#ifdef JIT
# include "lisp.h"
# include "jit.h"
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
#ifdef JIT
      return new SelectMaterializer<Store>(store, query_spec["columns"]);
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

#ifdef JIT
template<typename Store>
struct SelectMaterializer: Materializer<Store> {

  Store& store;
  std::vector<std::string> columns;
  std::vector<std::string> output_columns;

  SelectMaterializer(Store& store, json& select):store(store) {
    output_columns.reserve(select.size());

    uint8_t column_index = 0;
    for (auto s : select) {
      output_columns.push_back(s["name"]);

      const std::string& type = s["type"];
      if (type == "column") {
      }
      else if (type == "expression") {
        SExprParser parser;
        SExpr expr = parser.Parse(s["expr"]);
        std::set<std::string> expr_columns_set;
        FindColumns(expr, expr_columns_set);
        std::vector<std::string> expr_columns(expr_columns_set.begin(), expr_columns_set.end()); 
        JitFunction function(expr_columns, expr);
      }
      else throw std::invalid_argument("Unknown select type: " + type);

      ++column_index;
    }
    store.spec.GetDimIndices(output_columns, Materializer<Store>::column_indices);
  }

  void FindColumns(SExpr& expr, std::set<std::string>& expr_columns) {
    if (expr.type == SExpr::Symbol) {
      expr_columns.insert(expr.val);
    } else if (expr.type == SExpr::List) {
      for (auto it = expr.list.begin() + 1; it != expr.list.end(); ++it) {
        FindColumns(*it, expr_columns);
      }
    }
  }

  void Materialize(typename Materializer<Store>::GroupedMetrics& grouped_metrics, typename Materializer<Store>::Result& result) {
  }

  std::vector<std::string>& GetColumnNames() {
    return output_columns;
  }
};
#endif /* JIT */

#endif /* MAD_MATERIALIZER_H */
