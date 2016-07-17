#ifndef _MAD_INPUT_SPEC_H_
#define _MAD_INPUT_SPEC_H_

#include "json.hpp"

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

#endif /* _MAD_INPUT_SPEC_H_ */
