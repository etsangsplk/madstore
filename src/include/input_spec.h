#ifndef MAD_INPUT_SPEC_H
#define MAD_INPUT_SPEC_H

#include "../3rdparty/json.hpp"

using namespace std;
using json = nlohmann::json;

struct InputSpec {
  enum Format {TSV};

  Format format;
  string file;
  vector<string> columns;
  char field_delimiter;
  char row_delimiter;

  InputSpec(json& spec);
};

#endif /* MAD_INPUT_SPEC_H */
