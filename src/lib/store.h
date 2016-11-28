#ifndef _MAD_STORE_H_
#define _MAD_STORE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include <vector>
#include <cstring>
#include "easylogging++.h"
#include "base.h"
#include "specs.h"

struct Store: BaseStore {

  std::map<std::string, std::unique_ptr<BaseTable>> tables;
  StoreSpec& spec;

  Store(StoreSpec& spec);

  inline BaseTable& GetTable(std::string& table) {
    auto it = tables.find(table);
    if (it == tables.end()) {
      throw std::invalid_argument("No such table: " + table);
    }
    return *it->second.get();
  }

  void GetStats(json& stats);
  void OptimizeMemUsage();
  void Read(InputSpec& spec);
  void RunQuery(json& query_spec, std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>>& result);
#ifdef PERSIST
  void Persist();
#endif
};

#endif /* _MAD_STORE_H_ */
