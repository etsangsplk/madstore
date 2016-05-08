madstore
=========

In-memory data store for dimensions and metrics.

## How it works?

The implementation is naive, though using mechanical sympathy can provide good query results in some cases.

## Usage

### Building

You must have the following prerequisites:

 * Autoconf >= 2.69
 * Automake >= 1.14
 * G++ >= 4.9
 * libevent >= 2.0
 * sparsehash >= 2.0

When using persistence (`--enable-persistence` configure flag), you must also have:

 * Boost System >= 1.55

To build the binary, run:

      ~$ ./buildconf.sh
      ~$ ./configure
      ~$ make -j 4

