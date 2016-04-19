#ifndef MAD_INPUT_SPEC_H
#define MAD_INPUT_SPEC_H

#include <stdexcept>
#include "types.h"

using namespace std;

struct InputSpec {
  enum Format {TSV};

  Format format;
  string file;
  vector<string> columns;
  char field_delimiter = '\t';
  char row_delimiter = '\n';

  InputSpec(json& spec) {
    file = spec["file"];
    columns = spec["columns"].get<vector<string>>();
    string fmt = spec["format"];
    if (fmt == "tsv") {
      format = Format::TSV;
      if (spec.find("field_delimiter") != spec.end()) {
        field_delimiter = spec["field_delimiter"].get<string>().at(0);
      }
      if (spec.find("row_delimiter") != spec.end()) {
        row_delimiter = spec["row_delimiter"].get<string>().at(0);
      }
    } else {
      throw invalid_argument("Unsupported format: " + format);
    }
  }
};

#endif /* MAD_INPUT_SPEC_H */
