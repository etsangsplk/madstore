#include <stdexcept>
#include "input_spec.h"

using namespace std;

InputSpec::InputSpec(json& spec):field_delimiter('\t'),row_delimiter('\n') {
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
    throw invalid_argument("Unsupported format: " + to_string(format));
  }
}

