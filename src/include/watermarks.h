#ifndef MAD_WATERMARKS_H
#define MAD_WATERMARKS_H

#include <stdint.h>
#include <algorithm>
#include "../3rdparty/easylogging++.h"

using namespace std;

template<typename DimCodeType>
struct Watermarks {
  uint32_t step;
  vector<DimCodeType> values;
  vector<size_t> offsets;

  Watermarks():step(0) {}
  Watermarks(uint32_t& step):step(step) {}

  inline void Create(const size_t& offset, const DimCodeType& value) {
    if (values.empty() || value > values.back() + step) {
      CLOG(DEBUG, "Store")<<"Creating watermark [dim code:"<<value<<", offset:"<<offset<<"]";
      values.push_back(value);
      offsets.push_back(offset);
    }
  }

  size_t GetOffset(const DimCodeType& value) {
    size_t i = prev(lower_bound(values.begin(), values.end(), value)) - values.begin();
    return offsets[i < 0 ? 0 : i];
  }
};

#endif /* MAD_WATERMARKS_H */
