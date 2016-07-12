#ifndef MAD_INPUT_SPEC_H
#define MAD_INPUT_SPEC_H

#include "../3rdparty/json.hpp"

using json = nlohmann::json;

struct InputSpec {
  enum Format {TSV};

  Format format;
  std::string file;
  std::vector<std::string> columns;
  char field_delimiter;
  char row_delimiter;

  InputSpec(json& spec);
};

#endif /* MAD_INPUT_SPEC_H */
