#include <stdexcept>
#include "input_spec.h"

InputSpec::InputSpec(json& spec):field_delimiter('\t'),row_delimiter('\n') {
  file = spec["file"];
  columns = spec["columns"].get<std::vector<std::string>>();
  std::string fmt = spec["format"];
  if (fmt == "tsv") {
    format = Format::TSV;
    if (spec.find("field_delimiter") != spec.end()) {
      field_delimiter = spec["field_delimiter"].get<std::string>().at(0);
    }
    if (spec.find("row_delimiter") != spec.end()) {
      row_delimiter = spec["row_delimiter"].get<std::string>().at(0);
    }
  } else {
    throw std::invalid_argument("Unsupported format: " + std::to_string(format));
  }
  if (spec.find("skip_bad_rows") != spec.end()) {
    skip_bad_rows = spec["skip_bad_rows"];
  }
}

