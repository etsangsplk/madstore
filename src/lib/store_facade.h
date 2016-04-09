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
  Store_1_4* store_1_4;
  Store_2_4* store_2_4;
  Store_3_4* store_3_4;
  Store_4_4* store_4_4;
  Store_5_4* store_5_4;
  Store_6_4* store_6_4;
  Store_7_4* store_7_4;
  Store_8_4* store_8_4;
  Store_9_4* store_9_4;
  Store_10_4* store_10_4;

  StoreFacade(StoreSpec& store_spec)
    :store_spec(store_spec),
    store_1_4(NULL),store_2_4(NULL),
    store_3_4(NULL),store_4_4(NULL),
    store_5_4(NULL),store_6_4(NULL),
    store_7_4(NULL),store_8_4(NULL),
    store_9_4(NULL),store_10_4(NULL)
  {
    assert(("Maximum supported number of dimensions is 10", store_spec.DimsCount() <= 10));
    assert(("Maximum supported number of metrics is 4", store_spec.MetricsCount() <= 4));

    switch(store_spec.DimsCount()) {
      case 1: store_1_4 = new Store_1_4(store_spec); break;
      case 2: store_2_4 = new Store_2_4(store_spec); break;
      case 3: store_3_4 = new Store_3_4(store_spec); break;
      case 4: store_4_4 = new Store_4_4(store_spec); break;
      case 5: store_5_4 = new Store_5_4(store_spec); break;
      case 6: store_6_4 = new Store_6_4(store_spec); break;
      case 7: store_7_4 = new Store_7_4(store_spec); break;
      case 8: store_8_4 = new Store_8_4(store_spec); break;
      case 9: store_9_4 = new Store_9_4(store_spec); break;
      case 10: store_10_4 = new Store_10_4(store_spec); break;
    }
  }

  ~StoreFacade() {
    if (store_1_4) { delete store_1_4; }
    else if (store_2_4) { delete store_2_4; }
    else if (store_3_4) { delete store_3_4; }
    else if (store_4_4) { delete store_4_4; }
    else if (store_5_4) { delete store_5_4; }
    else if (store_6_4) { delete store_6_4; }
    else if (store_7_4) { delete store_7_4; }
    else if (store_8_4) { delete store_8_4; }
    else if (store_9_4) { delete store_9_4; }
    else if (store_10_4) { delete store_10_4; }
  }

  inline void Upsert(vector<uint32_t>& dim_indexes, vector<uint32_t>& metric_indexes, vector<string>& fields) {
    if (store_1_4) { store_1_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_2_4) { store_2_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_3_4) { store_3_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_4_4) { store_4_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_5_4) { store_5_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_6_4) { store_6_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_7_4) { store_7_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_8_4) { store_8_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_9_4) { store_9_4->Upsert(dim_indexes, metric_indexes, fields); }
    else if (store_10_4) { store_10_4->Upsert(dim_indexes, metric_indexes, fields); }
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

    fstream input(spec.filename);

    vector<string> fields(columns_num);
    for (string row; getline(input, row, spec.row_delimiter); ) {
      istringstream ss(row);
      int i = 0;
      for (string field; getline(ss, field, spec.field_delimiter); ) {
        fields[i++] = field;
      }
      Upsert(dim_indexes, metric_indexes, fields);
    }
  }

  void GetStats(json& stats) {
    if (store_1_4) { store_1_4->GetStats(stats); }
    else if (store_2_4) { store_2_4->GetStats(stats); }
    else if (store_3_4) { store_3_4->GetStats(stats); }
    else if (store_4_4) { store_4_4->GetStats(stats); }
    else if (store_5_4) { store_5_4->GetStats(stats); }
    else if (store_6_4) { store_6_4->GetStats(stats); }
    else if (store_7_4) { store_7_4->GetStats(stats); }
    else if (store_8_4) { store_8_4->GetStats(stats); }
    else if (store_9_4) { store_9_4->GetStats(stats); }
    else if (store_10_4) { store_10_4->GetStats(stats); }
  }

  void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result) {
    if (store_1_4) { QueryEngine<Store_1_4> engine; engine.RunQuery(*store_1_4, query_spec, result); }
    else if (store_2_4) { QueryEngine<Store_2_4> engine; engine.RunQuery(*store_2_4, query_spec, result); }
    else if (store_3_4) { QueryEngine<Store_3_4> engine; engine.RunQuery(*store_3_4, query_spec, result); }
    else if (store_4_4) { QueryEngine<Store_4_4> engine; engine.RunQuery(*store_4_4, query_spec, result); }
    else if (store_5_4) { QueryEngine<Store_5_4> engine; engine.RunQuery(*store_5_4, query_spec, result); }
    else if (store_6_4) { QueryEngine<Store_6_4> engine; engine.RunQuery(*store_6_4, query_spec, result); }
    else if (store_7_4) { QueryEngine<Store_7_4> engine; engine.RunQuery(*store_7_4, query_spec, result); }
    else if (store_8_4) { QueryEngine<Store_8_4> engine; engine.RunQuery(*store_8_4, query_spec, result); }
    else if (store_9_4) { QueryEngine<Store_9_4> engine; engine.RunQuery(*store_9_4, query_spec, result); }
    else if (store_10_4) { QueryEngine<Store_10_4> engine; engine.RunQuery(*store_10_4, query_spec, result); }
  }
};

#endif /* MAD_STORE_FACADE_H */
