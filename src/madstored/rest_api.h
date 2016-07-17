#ifndef _MAD_REST_API_
#define _MAD_REST_API_

#include <evhttp.h>
#include "store_facade.h"

struct RestAPI {
  static StoreFacade* store;
  evhttp* server;
  uint16_t port;

  RestAPI(uint16_t port, StoreFacade& store):port(port),server(nullptr) {
    RestAPI::store = &store;   
  }

  ~RestAPI() {
    if (server) {
      evhttp_free(server);
    }
  }

  void Start();
  static void Response(evhttp_request*, const char [], std::string);
  static void Handler(evhttp_request*, void*);
};

#endif /* _MAD_REST_API_ */
