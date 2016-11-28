#include "store.h"
#include "table_factory.h"

Store::Store(StoreSpec& spec):spec(spec) {
  for (auto it = spec.tables.begin(); it != spec.tables.end(); ++it) {
    tables[it->first] = std::unique_ptr<BaseTable>(TableFactory::Create(*it->second.get()));
  }
}

void Store::GetStats(json& stats) {
  for (auto it = tables.begin(); it != tables.end(); ++it) {
    json table_stats;
    it->second->GetStats(table_stats);
    stats[it->first] = table_stats;
  }
}

void Store::OptimizeMemUsage() {
  for (auto it = tables.begin(); it != tables.end(); ++it) {
    it->second->OptimizeMemUsage();
  }
}

void Store::Read(InputSpec& input_spec) { 
  TIMED_SCOPE(timerObj, "loading data into " + input_spec.table);

  UpsertSpec upsert_spec(input_spec, spec);

  std::ifstream input(input_spec.file);
  if (!input.good()) {
    throw std::runtime_error("Input file not accessible: " + input_spec.file);
  }

  BaseTable& table = GetTable(input_spec.table);

  auto columns_num = input_spec.columns.size();
  std::vector<std::string> values(columns_num);
  for (std::string row; std::getline(input, row, input_spec.row_delimiter); ) {
    std::istringstream ss(row);
    size_t i = 0;
    for (std::string value; std::getline(ss, value, input_spec.field_delimiter); ) {
      if (i >= columns_num) {
        if (!input_spec.skip_bad_rows) {
          throw std::runtime_error("Wrong input row (number of columns doesn't match schema):\n" + row);
        }
      } else {
        values[i++] = value;
      }
    }
    table.Upsert(upsert_spec, values);
  }
#ifdef PERSIST
  table.Persist();
#endif
}

void Store::RunQuery(json& query_spec, std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>>& result) {
  std::string target_table = query_spec["table"];
  TIMED_SCOPE(timerObj, "running query on " + target_table);

  BaseTable& table = GetTable(target_table);
  BaseQueryEngine* query_engine = table.CreateQueryEngine();
  query_engine->RunQuery(query_spec, result);
  delete query_engine;
}

#ifdef PERSIST
void Store::Persist() {
  for (auto it = tables.begin(); it != tables.end(); ++it) {
    if (!it->second->Persist()) {
      throw std::runtime_error(strerror(errno));
    }
  }
}
#endif

