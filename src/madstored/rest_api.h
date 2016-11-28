#ifndef _MAD_REST_API_
#define _MAD_REST_API_

#include <memory.h>
#include "ctpl.h"
#include "store.h"
#include "server_http.hpp"

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

struct RestAPI {
  Store& store;
  std::unique_ptr<ctpl::thread_pool> query_pool;
  std::unique_ptr<ctpl::thread_pool> write_pool;
  HttpServer server;

  RestAPI(uint16_t port, Store& store):store(store),server(port) {
    query_pool = std::make_unique<ctpl::thread_pool>(1);
    write_pool = std::make_unique<ctpl::thread_pool>(1);
  }

  void Start();
};

#endif /* _MAD_REST_API_ */
