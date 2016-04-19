#include <iostream>
#include <ctime>
#include <thread>
#include "../lib/types.h"
#include "../lib/store_facade.h"
#include "../lib/input_spec.h"
#include "../lib/query.h"
#include "rest_api.h"

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

  RestAPI rest_api(5555, store);
  rest_api.Start();
}
