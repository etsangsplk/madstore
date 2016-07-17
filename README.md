madstore
=========

In-memory data store for dimensions and metrics.

## Features

 * REST API interface.
 * Sum aggregation by dimensions (columns) set.
 * Persistence to disk using memory mapped files.
 * Lua scripting for post-aggregation.

## How it works?

The implementation is naive, though using mechanical sympathy can provide good query results in some cases.

## Usage

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

### REST API

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

### Building

You must have the following prerequisites:

 * Autoconf >= 2.69
 * Automake >= 1.14
 * C++14 compiler (g++ >= 5.1 or clang++ >= 3.5.x)
 * libevent >= 2.0
 * sparsehash >= 2.0

When using persistence (`--enable-persistence` configure flag), you must also have:

 * Boost System >= 1.55

When using select expressions (`--enable-expressions` configure flag), you must also have:

 * luajit >= 2.0.3

To build the binary, run:

      ~$ ./buildconf.sh
      ~$ ./configure
      ~$ make -j 4

### Testing

See [Examples](examples/README.md) section.

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

The Lua function is cached after running a query for the first time, so invocation becomes faster starting from running it again for the second time.

#### Getting Statistics

To get store statistics, issue a GET query to the `/api/stats` endpoint, like this:

     ~$ curl -sS http://localhost:5555/api/stats
     
The result will be returned in JSON format.


[asmjit]:https://github.com/kobalicek/asmjit

