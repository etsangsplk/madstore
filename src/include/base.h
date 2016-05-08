#ifndef MAD_BASE_H
#define MAD_BASE_H

#include "types.h"
#include "../3rdparty/json.hpp"

struct UpsertSpec;

using json = nlohmann::json;
using namespace std;

struct BaseQueryEngine {
  virtual ~BaseQueryEngine() {}
  virtual void RunQuery(json& query_spec, vector<pair<vector<string>,vector<MetricType>>>& result) = 0;
};

struct BaseStore {
  virtual ~BaseStore() {}
  virtual void Upsert(UpsertSpec& spec, vector<string>& values) = 0;
  virtual void GetStats(json& stats) = 0;
  virtual BaseQueryEngine* CreateQueryEngine() = 0;
  virtual void OptimizeMemUsage() = 0;
#ifdef PERSIST
  virtual bool Persist() = 0;
  virtual bool UnPersist() = 0;
#endif
};

#endif /* MAD_BASE_H */
