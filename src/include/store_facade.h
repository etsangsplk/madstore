#ifndef MAD_STORE_FACADE_H
#define MAD_STORE_FACADE_H

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <map>
#include "../3rdparty/easylogging++.h"
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
  typedef uint32_t DimCodeType;

  StoreSpec &store_spec;
  BaseStore<DimCodeType, MetricType>* store;

  StoreFacade(StoreSpec& store_spec):store_spec(store_spec) {
    store = StoreFactory<DimCodeType, MetricType>::Create(store_spec);
  }

  ~StoreFacade() {
    delete store;
  }

  StoreFacade(const StoreFacade& that) = delete;

  inline void Upsert(UpsertSpec& spec, vector<string>& values) {
    store->Upsert(spec, values);
  }

  void Read(InputSpec& spec) { 
    TIMED_SCOPE(timerObj, "loading data");

    UpsertSpec upsert_spec(spec, store_spec);

    ifstream input(spec.file);
    if (!input.good()) {
      throw runtime_error("Input file not accessible: " + spec.file);
    }

    vector<string> values(spec.columns.size());
    for (string row; getline(input, row, spec.row_delimiter); ) {
      istringstream ss(row);
      int i = 0;
      for (string value; getline(ss, value, spec.field_delimiter); ) {
        values[i++] = value;
      }
      Upsert(upsert_spec, values);
    }
  }

  void GetStats(json& stats) {
    store->GetStats(stats);
  }

  void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result) {
    TIMED_SCOPE(timerObj, "running query");

    BaseQueryEngine<MetricType>* query_engine = store->CreateQueryEngine();
    query_engine->RunQuery(query_spec, result);
    delete query_engine;
  }

  void OptimizeMemUsage() {
    store->OptimizeMemUsage();
  }
};

#endif /* MAD_STORE_FACADE_H */
