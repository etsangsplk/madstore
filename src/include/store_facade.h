#ifndef MAD_STORE_FACADE_H
#define MAD_STORE_FACADE_H

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <map>
#include "types.h"
#include "store.h"
#include "store_factory.h"
#include "input_spec.h"
#include "query.h"

using namespace std;

/**
 * This is facade for working with different store types depending on a number of dimensions/metrics
 */
struct StoreFacade {

  typedef unsigned long MetricType;

  StoreSpec &store_spec;
  BaseStore<MetricType>* store;

  StoreFacade(StoreSpec& store_spec):store_spec(store_spec) {
    store = StoreFactory<MetricType>::Create(store_spec);
  }

  ~StoreFacade() {
    delete store;
  }

  StoreFacade(const StoreFacade& that) = delete;

  inline void Upsert(vector<uint32_t>& dim_indexes, vector<uint32_t>& metric_indexes, vector<string>& fields) {
    store->Upsert(dim_indexes, metric_indexes, fields);
  }

  void Read(InputSpec& spec) { 
    vector<string>& columns = spec.columns;
    uint32_t columns_num = columns.size();
    vector<uint32_t> dim_indexes;
    vector<uint32_t> metric_indexes;
    for (int i = 0; i < columns_num; ++i) {
      string &column = columns[i];
      if (store_spec.GetDimIndex(column) != -1) {
        dim_indexes.push_back(i);
      } else if (store_spec.GetMetricIndex(column) != -1) {
        metric_indexes.push_back(i);
      } else {
        throw invalid_argument("Unknown column: " + column);
      }
    }

    fstream input(spec.file);

    for (string row; getline(input, row, spec.row_delimiter); ) {
      istringstream ss(row);
      vector<string> fields;
      fields.reserve(columns_num);
      for (string field; getline(ss, field, spec.field_delimiter); ) {
        fields.push_back(field);
      }
      Upsert(dim_indexes, metric_indexes, fields);
    }
  }

  void GetStats(json& stats) {
    store->GetStats(stats);
  }

  void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result) {
    BaseQueryEngine<MetricType>* query_engine = store->CreateQueryEngine();
    query_engine->RunQuery(query_spec, result);
    delete query_engine;
  }
};

#endif /* MAD_STORE_FACADE_H */
