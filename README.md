madstore
=========

In-memory data store for dimensions and metrics.

## How it works?

The implementation is naive, though using mechanical sympathy can provide better query results in some cases.

## Usage

### Building

You must have the following prerequisites:

 * Autoconf >= 2.69
 * Automake >= 1.14
 * G++ >= 4.9
 * libevent >= 2.0
 * sparsehash >= 2.0

To build the binary, run:

      ~$ ./buildconf.sh
      ~$ ./configure
      ~$ make -j 4

