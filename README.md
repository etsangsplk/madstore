madstore
=========

In-memory data store for dimensions and metrics.

## Table of Contents

 * [Features](#features)
 * [How it works?](#how)
 * [Usage](#usage)
   * [Running](#running)
   * [REST API](#restapi)
     * [Loading data](#loading)
     * [Querying](#querying)
     * [Getting statistics](#stats)
   * [Building](#building)
     * [Configuration options](#configure)
   * [Testing](#testing)

<a name="features">
## Features

 * REST API interface.
 * Sum aggregation by dimensions (columns) set.
 * Persistence to disk using memory mapped files.
 * Lua scripting for post-aggregation.

<a name="how">
## How it works?

The implementation is naive, though using mechanical sympathy can provide good query results in some cases.

<a name="usage">
## Usage

<a name="running">
### Running

To run the madstore, pass store specification file as the only argument:

     ~$ madstore store.json

Store specification JSON file defines dimensions and metrics that this instance can store. For example:

```javascript
{
  "dimensions": [
    {"name": "edit_time", "type": "integer", "watermark_step": 730},
    {"name": "editor_name"},
    {"name": "article_url"},
    {"name": "article_title"},
    {"name": "language"},
    {"name": "user_agent"},
    {"name": "country"},
    {"name": "city"}
  ],
  "metrics": [
    {"name": "count"},
    {"name": "lines_changed"},
    {"name": "time_spent"}
  ],
  "port": 5555
}
```

Explanation:

* `port` is a REST API port number
* `watermark_step` can be used bucketing records based on semi-oredered time series columns.

<a name="restapi">
### REST API

<a name="loading">
#### Loading Data

Loading data can be done using POST request to the `/api/load` endpoint. Example:

     ~$ curl -v -X POST -d@input.json http://localhost:5555/api/load

Input specification JSON file contains source information as well as column names defined in the source. For example:

```javascript
{
  "file": "data.tsv",
  "columns": [
    "edit_time",
    "editor_name",
    "article_url",
    "article_title",
    "language",
    "user_agent",
    "country",
    "city",
    "count",
    "lines_changed",
    "time_spent"
  ],
  "format": "tsv"
}
```

* For now only TSV format is supported.

<a name="querying">
#### Querying Data

Querying data can be done using POST request to the `/api/query` endpoint. Example:

     ~$ curl -v -X POST -d@query.json http://localhost:5555/api/query
     
Query specification JSON file contains the query itself in a very intuitive format. For example:

```javascript
{
  "type": "groupBy",
  "columns": ["edit_time", "country"],
  "filter": {
    "operator": "and",
    "filters": [
      {"operator": "greater", "column": "edit_time", "value": "16030000"},
      {"operator": "less", "column": "edit_time", "value": "16040000"}
    ]
  }
}
```

It's possible to use Lua expressions when selecting columns. For example:

```javascript
{
  "type": "groupBy",
  "select": [
    {"name": "year", "expr": "\"20\" .. string.sub(edit_time, 0, 2)", "fields": ["edit_time"]},
    {"name": "country"}
  ],
  "filter": {
    "operator": "and",
    "filters": [
      {"operator": "greater", "column": "edit_time", "value": "16030000"},
      {"operator": "less", "column": "edit_time", "value": "16040000"}
    ]
  }
}
```

In the example above, new field called `year` is added, and it's computed using specified Lua expression and source fields (`edit_time` in this case).
`country` field is selected as is as it has no expression defined. 

Lua functions are compiled using JIT compiler, and cached after running a query for the first time, so invocation becomes faster starting from running it again for the second time.

<a name="stats">
#### Getting Statistics

To get store statistics, issue a GET query to the `/api/stats` endpoint, like this:

     ~$ curl -sS http://localhost:5555/api/stats
     
The result will be returned in JSON format.

<a name="building">
### Building

You must have the following prerequisites:

 * Autoconf >= 2.69
 * Automake >= 1.14
 * C++14 compiler (g++ >= 5.1 or clang++ >= 3.5.x)
 * libevent >= 2.0
 * sparsehash >= 2.0
 * Boost >= 1.55 (required modules: system, thread, filesystem, datetime)
 * Python 2.7 with Jinja2 >= 2.1

When using select expressions (`--enable-expressions` configure flag), you must also have:

 * luajit >= 2.0.3

To build the binary, run:

      ~$ ./buildconf.sh
      ~$ ./configure
      ~$ make -j 4

<a name="configure">
#### Configuration options
Option               | Description
-------------------- | ------------
--enable-persistence | Persist data to disk. For now, persisting dictionaries must be invoked explicitly using `/api/persist` POST request.
--enable-expressions | Allow using Lua scripts for query post-processing.
--with-dims=NUM      | Number of dimensions this store supports (default: 10).
--with-metrics=NUM   | Number of metrics this store supports (default: 5).

Number of supported dimensions and metrics is configured in compile time for performance reasons. To change defaults, use
`--with-dims` and `--with-metrics` configuration options respectively.


<a name="testing">
### Testing

See [Examples](examples/README.md) section.

[asmjit]:https://github.com/kobalicek/asmjit

