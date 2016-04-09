madstore
=========

# How it works?

## Table types

Every table type is predefined in terms of allowed number of dimensions and metrics.

## Updating metrics

When inserting/updating new metric the following algorithm is used:

1. For all dimensions, find their integer values in dimensions table, or create a new value if dimension is new.
2. Calculate hash code of resulted integer values.
3. Find target pointer containing existing structure in the address table.
4. If this hash code is new, insert new structure entry, and store it's pointer to the address table.
5. If the has code already exists, find existing structure, and increment its metrics.

# Usage

## Building

      ~$ ./configure
      ~$ make

