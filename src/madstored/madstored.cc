#include <fstream>
#include "rest_api.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr<<"USAGE: "<<argv[0]<<" <store spec file>"<<endl;
    exit(1);
  }

  ifstream spec_file(argv[1]);
  json store_spec_json = json::parse(spec_file);
  StoreSpec store_spec(store_spec_json);
  StoreFacade store(store_spec);

  RestAPI rest_api(store_spec_json["port"], store);
  rest_api.Start();
}
