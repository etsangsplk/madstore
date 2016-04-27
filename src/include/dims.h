#ifndef MAD_DIMS_H
#define MAD_DIMS_H

#include <stdint.h>
#include <vector>
#include <cstring>
#include <google/dense_hash_map>
#include "types.h"

using namespace std;
using google::dense_hash_map;

/**
 * Dimension (column) definition
 */
struct Dim {
  enum ValueType { String, Integer };

  ValueType type;
  string name;
  uint32_t watermark_step;

  Dim(ValueType type, string name, uint32_t watermark_step)
    :type(type),name(name),watermark_step(watermark_step) {}
};

/**
 * Dictionary for translating dimension values to integer codes and back
 */
template<typename DimCodeType,size_t DimsCount>
struct DimDict {

  vector<Dim> dims;
  array<dense_hash_map<string,DimCodeType>,DimsCount> value_to_code;
  array<vector<string>,DimsCount> code_to_value;

  DimDict(vector<Dim>& dims):dims(dims) {
    string empty;
    // String containing the only CTRL+A character:
    empty.push_back(1);
    for (auto & d : value_to_code) {
      d.set_empty_key(empty);
    }
  };

  DimDict(const DimDict& that) = delete;

  /**
   * Translates dimension values to their codes
   */
  void Encode(array<string,DimsCount>& values, array<DimCodeType,DimsCount>& codes) {
    for (int i = 0; i < DimsCount; ++i) {
      if (dims[i].type == Dim::ValueType::Integer) {
        codes[i] = stoi(values[i]);
      } else {
        string& value = values[i];
        dense_hash_map<string,DimCodeType>& vc = value_to_code[i];
        auto existing = vc.find(value);
        if (existing == vc.end()) {
          DimCodeType code = vc.size();
          codes[i] = vc[value] = code;
          vector<string>& cv = code_to_value[i];
          cv.insert(cv.begin() + code, value);
        } else {
          codes[i] = existing->second;
        }
      }
    }
  }

  /**
   * Translates dimension code to its original value
   */
  inline void Decode(const uint8_t& dim_index, const DimCodeType& code, string& value) {
    Dim& dim = dims[dim_index];
    if (dim.type == Dim::ValueType::Integer) {
      value = to_string(code);
    } else {
      value = code_to_value[dim_index][code];
    }
  }

  /**
   * Finds dimension code by the original value
   */
  inline DimCodeType GetCode(const uint8_t& dim_index, const string& value) {
    dense_hash_map<string,DimCodeType>& dict = value_to_code[dim_index];
    auto it = dict.find(value);
    if (it != dict.end()) {
      return it->second;
    }
    return 0;
  }

  /**
   * Returns dictionary statistics
   */
  void GetStats(json& stats) {
    unsigned long mem_usage = 0;
    for (int i = 0; i < DimsCount; ++i) {
      auto & vc = value_to_code[i];
      auto & cv = code_to_value[i];
      unsigned long vc_usage = vc.size() * sizeof(dense_hash_map<string,DimCodeType>);
      for (auto & c : vc) {
        vc_usage += c.first.size() + sizeof(DimCodeType);
      }
      // Multiply by dense_hash_map overhead:
      mem_usage += vc_usage * 1.78;
      unsigned long cv_usage = cv.size() * sizeof(string);
      for (auto & v : cv) {
        cv_usage += v.size() + sizeof(string);
      }
      mem_usage += cv_usage;
      stats["cardinality"][dims[i].name] = cv.size();
    }
    stats["dims_usage_mb"] = mem_usage/(1024*1024);
  }
};

#endif /* MAD_DIMS_H */
