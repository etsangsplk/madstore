#include <cstring>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <sstream>
#include "rest_api.h"
#include "easylogging++.h"
#include "json.hpp"

using json = nlohmann::json;

StoreFacade* RestAPI::store;

void RestAPI::Response(evhttp_request* req, const char content_type[], std::string content) {
  auto* output_buf = evhttp_request_get_output_buffer(req);
  evhttp_add_header(req->output_headers, "Content-Type", content_type);
  evhttp_add_header(req->output_headers, "Content-Length", std::to_string(content.size()).c_str());
  evbuffer_add_printf(output_buf, "%s", content.c_str());
  evhttp_send_reply(req, HTTP_OK, nullptr, output_buf);
}

void RestAPI::Handler(evhttp_request* req, void*) {
  try {
    evhttp_cmd_type method = evhttp_request_get_command(req);
    std::string path = std::string(evhttp_request_uri(req));

    if (method == EVHTTP_REQ_POST) {
      CLOG(INFO, "RestAPI")<<"Recieved request [POST] "<<path;
      auto* input_buf = evhttp_request_get_input_buffer(req);
      int req_length = evbuffer_get_length(input_buf);
      char req_body[req_length+1];
      memset(req_body, 0, sizeof(req_body));
      evbuffer_copyout(input_buf, req_body, req_length);

      try {
        if (path == "/api/query") {
          json request = json::parse(req_body);
          std::vector<std::pair<std::vector<std::string>,std::vector<unsigned long>>> result;
          store->RunQuery(request, result);
          std::ostringstream os;
          for (const auto & row : result) {
            for (const auto & v : row.first) {
              os<<v<<"\t";
            }
            bool first = true;
            for (const auto & v : row.second) {
              if (!first) {
                os<<"\t";
              } else {
                first = false;
              }
              os<<v;
            }
            os<<std::endl;
          }
          RestAPI::Response(req, "text/tab-separated-values", os.str());
        }
        else if (path == "/api/load") {
          json request = json::parse(req_body);
          InputSpec input_spec(request);
          store->Read(input_spec);
          evhttp_send_reply(req, HTTP_OK, nullptr, nullptr);
        }
        else if (path == "/api/optimize-memory") {
          store->OptimizeMemUsage();
          evhttp_send_reply(req, HTTP_OK, nullptr, nullptr);
        }
#ifdef PERSIST
        else if (path == "/api/persist") {
          store->Persist();
          evhttp_send_reply(req, HTTP_OK, nullptr, nullptr);
        }
#endif
        else {
          evhttp_send_reply(req, HTTP_NOTFOUND, nullptr, nullptr);
        }
      } catch (std::invalid_argument& e) {
        evhttp_send_reply(req, HTTP_BADREQUEST, e.what(), nullptr);
      }
    }
    else if (method == EVHTTP_REQ_GET) {
      CLOG(INFO, "RestAPI")<<"Recieved request [GET] "<<path;
      if (path == "/api/stats") {
        json stats;
        store->GetStats(stats);
        RestAPI::Response(req, "application/json", stats.dump(2));
      }
      else {
        evhttp_send_reply(req, HTTP_NOTFOUND, nullptr, nullptr);
      }
    }
    else {
      evhttp_send_reply(req, HTTP_BADMETHOD, nullptr, nullptr);
    }
  } catch (std::exception& e) {
    evhttp_send_reply(req, HTTP_INTERNAL, e.what(), nullptr);
  }
}

void RestAPI::Start() {
  event_init();
  server = evhttp_start("0.0.0.0", port);
  if (!server) {
    throw std::runtime_error("Couldn't to initialize HTTP server!");
  }

  evhttp_set_gencb(server, Handler, nullptr);
  event_dispatch();
}

