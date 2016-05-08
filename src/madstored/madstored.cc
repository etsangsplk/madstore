#include <fstream>
#include "../3rdparty/json.hpp"
#include "rest_api.h"
#include "logger.h"

using namespace std;
using json = nlohmann::json;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"USAGE: "<<argv[0]<<" <store spec file>"<<endl;
    exit(1);
  }

  Logger::Init();

  json store_spec_json = json::parse(ifstream(argv[1]));
  StoreSpec store_spec(store_spec_json);
  StoreFacade store(store_spec);

  RestAPI rest_api(store_spec_json["port"], store);
  rest_api.Start();
}
