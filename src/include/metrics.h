#ifndef MAD_METRICS_H
#define MAD_METRICS_H

#include <stdint.h>

using namespace std;

/**
 * Metric (column) definition
 */
struct Metric {
  enum ValueType { Long, Double };

  ValueType type;
  string name;

  Metric(ValueType type, string name)
    :type(type),name(name) {}
};

template<typename MetricType,size_t MetricsCount>
struct MetricsArray: array<MetricType,MetricsCount> {

  inline const MetricsArray& operator += (const MetricsArray &other) {
    for (int i = 0; i < MetricsCount; ++i) {
      (*this)[i] += other[i];
    }
    return (*this);
  }
};

#endif /* MAD_METRICS_H */
