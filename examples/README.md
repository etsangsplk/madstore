Testing madstore with fake data
================================

### Pre-requisites

     ~$ pip install fake-factory
     ~$ cd examples/

### Generating Fake Data

     ~$ ./generate.py > data.tsv

By default this will generate 2M of events for 1 year time frame. To tweak these parameters,
edit the script, and set `num_entries` and `period_hrs` constants accordingly.

### Starting Madstore

     ~$ ../src/madstored/madstored store.json

This process will stay running in this terminal, therefore you have to go to another for loading,
and querying data. You can come back and observe statistics regarding timing of every operation.

### Loading Fake Data

     ~$ curl -v -X POST -d@input.json http://localhost:5555/api/load

### Querying

The following query returns events from April 2016, aggregated by edit time and country:

     ~$ curl -v -X POST -d@query.json http://localhost:5555/api/query

### Getting Statistics

     ~$ curl -sS http://localhost:5555/api/stats

