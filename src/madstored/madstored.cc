#include <fstream>
#include "json.hpp"
#include "rest_api.h"
#include "logger.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr<<"USAGE: "<<argv[0]<<" <store spec file>"<<std::endl;
    exit(1);
  }

  Logger::Init();

  json store_spec_json = json::parse(std::ifstream(argv[1]));
  StoreSpec store_spec(store_spec_json);
  StoreFacade store(store_spec);

  RestAPI rest_api(store_spec_json["port"], store);
  rest_api.Start();
}
