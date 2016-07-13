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
 * C++14 compiler (g++ >= 5.1 or clang++ >= 3.5.x)
 * libevent >= 2.0
 * sparsehash >= 2.0

When using persistence (`--enable-persistence` configure flag), you must also have:

 * Boost System >= 1.55

When using select expressions (`--enable-expressions` configure flag), you must also have:

 * [asmjit] >= 6467c73

To build the binary, run:

      ~$ ./buildconf.sh
      ~$ ./configure
      ~$ make -j 4

### Testing

See [Examples](examples/README.md) section.

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

#### Getting Statistics

To get store statistics, issue a GET query to the `/api/stats` endpoint, like this:

     ~$ curl -sS http://localhost:5555/api/stats
     
The result will be returned in JSON format.


[asmjit]:https://github.com/kobalicek/asmjit

