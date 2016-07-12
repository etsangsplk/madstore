#ifndef MAD_BASE_H
#define MAD_BASE_H

#include "types.h"
#include "../3rdparty/json.hpp"

struct UpsertSpec;

using json = nlohmann::json;

struct BaseQueryEngine {
  virtual ~BaseQueryEngine() {}
  virtual void RunQuery(json& query_spec, std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>>& result) = 0;
};

struct BaseStore {
  virtual ~BaseStore() {}
  virtual void Upsert(UpsertSpec& spec, std::vector<std::string>& values) = 0;
  virtual void GetStats(json& stats) = 0;
  virtual BaseQueryEngine* CreateQueryEngine() = 0;
  virtual void OptimizeMemUsage() = 0;
#ifdef PERSIST
  virtual bool Persist() = 0;
  virtual bool UnPersist() = 0;
#endif
};

#endif /* MAD_BASE_H */
