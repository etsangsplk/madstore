#ifndef _MAD_METRICS_H_
#define _MAD_METRICS_H_

#include <stdint.h>
#include "types.h"

/**
 * Metric (column) definition
 */
struct Metric {
  enum ValueType { Long, Double };

  ValueType type;
  std::string name;

  Metric(ValueType type, std::string name)
    :type(type),name(name) {}
};

template<size_t MetricsCount>
struct MetricsArray: std::array<MetricType,MetricsCount> {

  inline const MetricsArray& operator += (const MetricsArray &other) {
    for (uint8_t i = 0; i < MetricsCount; ++i) {
      (*this)[i] += other[i];
    }
    return (*this);
  }
};

#endif /* _MAD_METRICS_H_ */
