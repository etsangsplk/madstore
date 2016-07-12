#ifndef MAD_TYPES_H
#define MAD_TYPES_H

#include <stdint.h>

using MetricType = unsigned long;

/* This allows dimension cardinality for up to 2^32 */
using DimCodeType = uint32_t;

using offset_t = uint32_t;

#endif /* MAD_TYPES_H */
