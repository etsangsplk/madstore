#ifndef MAD_STORE_FACADE_H
#define MAD_STORE_FACADE_H

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <map>
#include "types.h"
#include "store.h"
#include "input_spec.h"
#include "query.h"

using namespace std;

/**
 * This is facade for working with different store types depending on a number of dimensions/metrics
 */
struct StoreFacade {

  typedef unsigned long MetricType;

  typedef Store<1,MetricType,4> Store_1_4;
  typedef Store<2,MetricType,4> Store_2_4;
  typedef Store<3,MetricType,4> Store_3_4;
  typedef Store<4,MetricType,4> Store_4_4;
  typedef Store<5,MetricType,4> Store_5_4;
  typedef Store<6,MetricType,4> Store_6_4;
  typedef Store<7,MetricType,4> Store_7_4;
  typedef Store<8,MetricType,4> Store_8_4;
  typedef Store<9,MetricType,4> Store_9_4;
  typedef Store<10,MetricType,4> Store_10_4;

  StoreSpec &store_spec;
  BaseStore<MetricType>* store;

  StoreFacade(StoreSpec& store_spec):store_spec(store_spec) {
    assert(("Maximum supported number of dimensions is 10", store_spec.DimsCount() <= 10));
    assert(("Maximum supported number of metrics is 4", store_spec.MetricsCount() <= 4));

    switch(store_spec.DimsCount()) {
      case 1: store = new Store_1_4(store_spec); break;
      case 2: store = new Store_2_4(store_spec); break;
      case 3: store = new Store_3_4(store_spec); break;
      case 4: store = new Store_4_4(store_spec); break;
      case 5: store = new Store_5_4(store_spec); break;
      case 6: store = new Store_6_4(store_spec); break;
      case 7: store = new Store_7_4(store_spec); break;
      case 8: store = new Store_8_4(store_spec); break;
      case 9: store = new Store_9_4(store_spec); break;
      case 10: store = new Store_10_4(store_spec); break;
    }
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
