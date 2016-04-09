#ifndef MAD_INPUT_SPEC_H
#define MAD_INPUT_SPEC_H

using namespace std;

struct InputSpec {
  string filename;
  vector<string> columns;
  const char field_delimiter = '\t';
  const char row_delimiter = '\n';

  InputSpec(string filename, vector<string> columns)
    :filename(filename),columns(columns) {}
};

#endif /* MAD_INPUT_SPEC_H */
