#include <iostream>
#include <ctime>
#include "../lib/types.h"
#include "../lib/store_facade.h"
#include "../lib/input_spec.h"
#include "../lib/query.h"

using namespace std;

int main(int argc, char** arv) {
  json store_spec_json = json::parse(R"eof(
    {
      "dimensions": [
        {"name": "install_date", "type": "integer"},
        {"name": "publisher"},
        {"name": "partner"},
        {"name": "campaign"},
        {"name": "country"},
        {"name": "event_name"}
      ],
      "metrics": [
        {"name": "event_count"},
        {"name": "event_new"},
        {"name": "revenue"}
      ]
    }
  )eof");

  StoreSpec store_spec(store_spec_json);
  StoreFacade store(store_spec);

  json input_spec_json = json::parse(R"eof(
    {
      "file": "/home/michael/qihoo-inapps.tsv",
      "columns": ["install_date","publisher","partner","campaign","country", "event_name", "event_count","event_new","revenue"],
      "format": "tsv"
    }
  )eof");

  InputSpec input_spec(input_spec_json);
  cout<<"Loading data into memory..."<<endl;

  clock_t begin = clock();
  store.Read(input_spec);
  clock_t end = clock();
  cout<<"Loaded data in "<<double(end - begin) / CLOCKS_PER_SEC<<" secs"<<endl;
  
  json stats;
  store.GetStats(stats);
  cout<<stats.dump(2)<<endl;
  
  cout<<"To run the query, press ENTER"<<endl;
  cin.get();

  json query_spec_json = json::parse(R"eof(
    {
      "type": "groupBy",
      "columns": ["publisher", "country"],
      "filter": {
        "operator": "and",
        "filters": [
          {"operator": "in", "column": "country", "values": ["US", "CH"]},
          {"operator": "greater", "column": "install_date", "value": "20160300"},
          {"operator": "less", "column": "install_date", "value": "20160400"}
        ]
      }
    }
  )eof");

  vector<pair<vector<string>,vector<unsigned long>>> result;
  begin = clock();
  store.RunQuery(query_spec_json, result);
  end = clock();
  cout<<"Run query in "<<double(end - begin) / CLOCKS_PER_SEC<<" secs"<<endl;

  for (const auto& row : result) {
    cout<<row.first<<row.second<<endl;
  }
}
