#ifndef MAD_DIMS_H
#define MAD_DIMS_H

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <google/dense_hash_map>
#include "../3rdparty/easylogging++.h"
#include "../3rdparty/json.hpp"
#include "serializers.h"

using google::dense_hash_map;
using json = nlohmann::json;

/**
 * Dimension (column) definition
 */
struct Dim {
  static const uint8_t SIZE_LIMIT = 250;
  enum ValueType { String, Integer };

  ValueType type;
  std::string name;
  offset_t watermark_step;

  Dim(ValueType type, std::string name, offset_t watermark_step)
    :type(type),name(name),watermark_step(watermark_step) {}
};

/**
 * Dictionary for translating dimension values to integer codes and back
 */
template<uint8_t DimsCount>
struct DimDict {

  std::vector<Dim> dims;
  std::array<dense_hash_map<std::string,DimCodeType>,DimsCount> value_to_code;
  std::array<std::vector<std::string>,DimsCount> code_to_value;

  DimDict(std::vector<Dim>& dims):dims(dims) {
    std::string empty;
    // String containing the only CTRL+A character:
    empty.push_back(1);
    for (auto & d : value_to_code) {
      d.set_empty_key(empty);
    }
  }

  DimDict(const DimDict& that) = delete;

  /**
   * Translates dimension values to their codes
   */
  void Encode(std::array<std::string,DimsCount>& values, std::array<DimCodeType,DimsCount>& codes) {
    for (uint8_t i = 0; i < DimsCount; ++i) {
      if (dims[i].type == Dim::ValueType::Integer) {
        codes[i] = stoi(values[i]);
      } else {
        std::string& value = values[i];
        if (value.length() > Dim::SIZE_LIMIT) {
          value.erase(Dim::SIZE_LIMIT);
        }
        dense_hash_map<std::string,DimCodeType>& vc = value_to_code[i];
        auto existing = vc.find(value);
        if (existing == vc.end()) {
          DimCodeType code = vc.size();
          codes[i] = vc[value] = code;
          std::vector<std::string>& cv = code_to_value[i];
          cv.insert(cv.begin() + code, value);
        } else {
          codes[i] = existing->second;
        }
      }
    }
  }

  /**
   * Translates dimension value to its code
   */
  inline void Encode(const uint8_t& dim_index, const std::string& value, DimCodeType& code) {
    auto & dim = dims[dim_index];
    if (dim.type == Dim::ValueType::Integer) {
      code = stoi(value);
    } else {
      code = value_to_code[dim_index][value];
    }
  }

  /**
   * Translates dimension code to its original value
   */
  inline void Decode(const uint8_t& dim_index, const DimCodeType& code, std::string& value) {
    auto & dim = dims[dim_index];
    if (dim.type == Dim::ValueType::Integer) {
      value = std::to_string(code);
    } else {
      value = code_to_value[dim_index][code];
    }
  }

  /**
   * Finds dimension code by the original value
   */
  inline DimCodeType GetCode(const uint8_t& dim_index, const std::string& value) {
    auto & dict = value_to_code[dim_index];
    auto it = dict.find(value);
    if (it != dict.end()) {
      return it->second;
    }
    return 0;
  }

  void GetStats(json& stats) {
    unsigned long mem_usage = 0;
    for (int i = 0; i < DimsCount; ++i) {
      auto & vc = value_to_code[i];
      auto & cv = code_to_value[i];
      unsigned long vc_usage = vc.size() * sizeof(dense_hash_map<std::string,DimCodeType>);
      for (auto & c : vc) {
        vc_usage += c.first.size() + sizeof(DimCodeType);
      }
      // Multiply by dense_hash_map overhead:
      mem_usage += vc_usage * 1.78;
      unsigned long cv_usage = cv.size() * sizeof(std::string);
      for (auto & v : cv) {
        cv_usage += v.size() + sizeof(std::string);
      }
      mem_usage += cv_usage;
      stats["cardinality"][dims[i].name] = cv.size();
    }
    stats["dims_usage_mb"] = mem_usage/(1024*1024);
  }

#ifdef PERSIST
  bool SaveToFile(FILE* fp) {
    TIMED_SCOPE(timerObj, "saving dictionary");

    unique_ptr<char[]> buf = make_unique<char[]>(Dim::SIZE_LIMIT+1);
    StringToDimCodeSerializer serializer(buf.get());
    for (uint8_t i = 0; i < DimsCount; ++i) {
      auto & cv = code_to_value[i];
      size_t cv_size = cv.size();
      if (fwrite(&cv_size, sizeof(size_t), 1, fp) != 1) {
        return false;
      }
      for (auto & str : cv) {
        const uint8_t str_len = str.length();
        if (fwrite(&str_len, sizeof(uint8_t), 1, fp) != 1) {
          return false;
        }
        if (str_len > 0 && fwrite(str.data(), str_len, 1, fp) != 1) {
          return false;
        }
      }
    }
    return true;
  }

  bool LoadFromFile(FILE* fp) {
    TIMED_SCOPE(timerObj, "loading dictionary");

    unique_ptr<char[]> buf = make_unique<char[]>(Dim::SIZE_LIMIT+1);
    StringToDimCodeSerializer serializer(buf.get());
    for (uint8_t i = 0; i < DimsCount; ++i) {
      auto & cv = code_to_value[i];
      auto & vc = value_to_code[i];
      size_t cv_size;
      if (fread(&cv_size, sizeof(size_t), 1, fp) != 1) {
        return false;
      }
      cv.reserve(cv_size);
      while (cv_size-- > 0) {
        uint8_t str_len;
        if (fread(&str_len, sizeof(uint8_t), 1, fp) != 1) {
          return false;
        }
        if (str_len > 0 && fread(serializer.buf, str_len, 1, fp) != 1) {
          return false;
        }
        std::string value(serializer.buf, str_len);
        vc[value] = cv.size();
        cv.push_back(move(value));
      }
    }
    return true;
  }
#endif /* PERSIST */
};

#endif /* MAD_DIMS_H */
