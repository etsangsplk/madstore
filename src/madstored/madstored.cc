#include <fstream>
#include "rest_api.h"
#include "../3rdparty/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"USAGE: "<<argv[0]<<" <store spec file>"<<endl;
    exit(1);
  }

  el::Configurations log_conf;
  log_conf.setToDefault();
  log_conf.setGlobally(el::ConfigurationType::ToFile, "false");
  el::Loggers::reconfigureAllLoggers(log_conf);

  ifstream spec_file(argv[1]);
  json store_spec_json = json::parse(spec_file);
  StoreSpec store_spec(store_spec_json);
  StoreFacade store(store_spec);

  RestAPI rest_api(store_spec_json["port"], store);
  rest_api.Start();
}
