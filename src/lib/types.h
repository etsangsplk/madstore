#ifndef _MAD_TYPES_H_
#define _MAD_TYPES_H_

#include <stdint.h>

using MetricType = unsigned long;

/* This allows dimension cardinality for up to 2^32 */
using DimCodeType = uint32_t;

using offset_t = uint32_t;

#endif /* _MAD_TYPES_H_ */
