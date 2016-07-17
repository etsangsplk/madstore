#ifndef _MAD_SERIALIZERS_H_
#define _MAD_SERIALIZERS_H_

#include "types.h"

struct StringToDimCodeSerializer {

  size_t entries;
  char* buf;

  StringToDimCodeSerializer(char* buf, size_t entries = 0)
    :buf(buf),entries(entries) {}

  bool operator()(FILE* fp, const std::pair<const std::string, DimCodeType>& value) {
    if (entries > 0) {
      const uint8_t size = value.first.length();
      if (fwrite(&size, sizeof(uint8_t), 1, fp) != 1) {
        return false;
      }
      if (size > 0 && fwrite(value.first.data(), size, 1, fp) != 1) {
        return false;
      }
      if (fwrite(&value.second, sizeof(DimCodeType), 1, fp) != 1) {
        return false;
      }
      --entries;
    }
    return true;
  }

  bool operator()(FILE* fp, std::pair<const std::string, DimCodeType>* value) {
    if (entries > 0) {
      uint8_t size;
      if (fread(&size, sizeof(uint8_t), 1, fp) != 1) {
        return false;
      }
      if (size > 0 && fread(buf, size, 1, fp) != 1) {
        return false;
      }
      void* ptr = (void *)(&value->first);
      new(ptr) std::string(buf, size);
      if (fread(const_cast<DimCodeType*>(&value->second), sizeof(DimCodeType), 1, fp) != 1) {
        return false;
      }
      --entries;
    }
    return true;
  }
};

#endif /* _MAD_SERIALIZERS_H_ */
