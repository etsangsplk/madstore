#ifndef _MAD_STORE_FACADE_H_
#define _MAD_STORE_FACADE_H_

#include "json.hpp"
#include "store_spec.h"
#include "base.h"

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

  inline void Upsert(UpsertSpec& spec, std::vector<std::string>& values) {
    store->Upsert(spec, values);
  }

  void Read(InputSpec& spec);
  void GetStats(json& stats);
  void RunQuery(json& query_spec, std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>>& result);
  void OptimizeMemUsage();
#ifdef PERSIST
  void Persist();
#endif
};

#endif /* _MAD_STORE_FACADE_H_ */
