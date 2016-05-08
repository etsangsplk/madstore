#ifndef MAD_STORE_FACADE_H
#define MAD_STORE_FACADE_H

#include "../3rdparty/json.hpp"
#include "store_spec.h"
#include "base.h"

using namespace std;
using json = nlohmann::json;

/**
 * This is facade for working with different store types depending on a number of dimensions/metrics
 */
struct StoreFacade {

  StoreSpec &store_spec;
  BaseStore* store;

  StoreFacade(StoreSpec& store_spec);
  ~StoreFacade();
  StoreFacade(const StoreFacade& that) = delete;

  inline void Upsert(UpsertSpec& spec, vector<string>& values) {
    store->Upsert(spec, values);
  }

  void Read(InputSpec& spec);
  void GetStats(json& stats);
  void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result);
  void OptimizeMemUsage();
#ifdef PERSIST
  void Persist();
#endif
};

#endif /* MAD_STORE_FACADE_H */
