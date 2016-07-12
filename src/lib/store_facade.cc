#include <stdexcept>
#include <sstream>
#include <fstream>
#include <errno.h>
#include "../3rdparty/easylogging++.h"
#include "store_facade.h"
#include "store_factory.h"

StoreFacade::StoreFacade(StoreSpec& store_spec):store_spec(store_spec) {
  store = StoreFactory::Create(store_spec);
}

StoreFacade::~StoreFacade() {
  delete store;
}

void StoreFacade::Read(InputSpec& spec) { 
  TIMED_SCOPE(timerObj, "loading data");

  UpsertSpec upsert_spec(spec, store_spec);

  std::ifstream input(spec.file);
  if (!input.good()) {
    throw std::runtime_error("Input file not accessible: " + spec.file);
  }

  std::vector<std::string> values(spec.columns.size());
  for (std::string row; std::getline(input, row, spec.row_delimiter); ) {
    std::istringstream ss(row);
    size_t i = 0;
    for (std::string value; std::getline(ss, value, spec.field_delimiter); ) {
      values[i++] = value;
    }
    Upsert(upsert_spec, values);
  }
}

void StoreFacade::GetStats(json& stats) {
  store->GetStats(stats);
}

void StoreFacade::RunQuery(json& query_spec, std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>>& result) {
  TIMED_SCOPE(timerObj, "running query");

  BaseQueryEngine* query_engine = store->CreateQueryEngine();
  query_engine->RunQuery(query_spec, result);
  delete query_engine;
}

void StoreFacade::OptimizeMemUsage() {
  store->OptimizeMemUsage();
}

#ifdef PERSIST
void StoreFacade::Persist() {
  if (!store->Persist()) {
    throw std::runtime_error(strerror(errno));
  }
}
#endif

