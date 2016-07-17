#ifndef _MAD_WATERMARKS_H_
#define _MAD_WATERMARKS_H_

#include <stdio.h>
#include <stdint.h>
#include <algorithm>

struct Watermarks {
  offset_t step;
  std::vector<DimCodeType> values;
  std::vector<offset_t> offsets;

  Watermarks():step(0) {}
  Watermarks(offset_t& step):step(step) {}

  inline void Create(const offset_t& offset, const DimCodeType& value) {
    if (values.empty() || value > values.back() + step) {
      CLOG(DEBUG, "Store")<<"Creating watermark [dim code:"<<value<<", offset:"<<offset<<"]";
      values.push_back(value);
      offsets.push_back(offset);
    }
  }

  offset_t GetOffset(const DimCodeType& value) {
    auto i = prev(lower_bound(values.begin(), values.end(), value)) - values.begin();
    return i < 0 ? 0 : offsets[i];
  }

#ifdef PERSIST
  bool SaveToFile(FILE* fp) {
    size_t size = values.size();
    if (fwrite(&size, sizeof(size_t), 1, fp) != 1) {
      return false;
    }
    for (size_t i = 0; i < size; ++i) {
      if (fwrite(&values[i], sizeof(DimCodeType), 1, fp) != 1) {
        return false;
      }
      if (fwrite(&offsets[i], sizeof(offset_t), 1, fp) != 1) {
        return false;
      }
    }
    return true;
  }

  bool LoadFromFile(FILE* fp) {
    size_t size;
    if (fread(&size, sizeof(size_t), 1, fp) != 1) {
      return false;
    }
    values.resize(size);
    offsets.resize(size);
    for (size_t i = 0; i < size; ++i) {
      if (fread(&values[i], sizeof(DimCodeType), 1, fp) != 1) {
        return false;
      }
      if (fread(&offsets[i], sizeof(offset_t), 1, fp) != 1) {
        return false;
      }
    }
    return true;
  }
#endif /* PERSIST */
};

#endif /* _MAD_WATERMARKS_H_ */
