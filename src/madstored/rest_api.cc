#include <cstring>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <sstream>
#include <ctime>
#include "rest_api.h"

using namespace std;

StoreFacade* RestAPI::store;

void RestAPI::Response(evhttp_request* req, const char content_type[], string content) {
  auto* output_buf = evhttp_request_get_output_buffer(req);
  evhttp_add_header(req->output_headers, "Content-Type", content_type);
  evhttp_add_header(req->output_headers, "Content-Length", to_string(content.size()).c_str());
  evbuffer_add_printf(output_buf, "%s", content.c_str());
  evhttp_send_reply(req, HTTP_OK, nullptr, output_buf);
}

void RestAPI::Handler(evhttp_request* req, void*) {
  try {
    string path = string(evhttp_request_uri(req));

    if (evhttp_request_get_command(req) == EVHTTP_REQ_POST) {
      auto* input_buf = evhttp_request_get_input_buffer(req);
      int req_length = evbuffer_get_length(input_buf);
      char req_body[req_length+1] = {0};
      evbuffer_copyout(input_buf, req_body, req_length);

      try {
        json request = json::parse(req_body);
        if (path == "/api/query") {
          vector<pair<vector<string>,vector<unsigned long>>> result;
          clock_t begin = clock();
          store->RunQuery(request, result);
          clock_t end = clock();
          cout<<"Run query in "<<double(end - begin) / CLOCKS_PER_SEC<<" secs"<<endl;
          ostringstream os;
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
            os<<endl;
          }
          RestAPI::Response(req, "text/tab-separated-values", os.str());
        }
        else if (path == "/api/load") {
          InputSpec input_spec(request);
          clock_t begin = clock();
          store->Read(input_spec);
          clock_t end = clock();
          cout<<"Loaded data in "<<double(end - begin) / CLOCKS_PER_SEC<<" secs"<<endl;
          evhttp_send_reply(req, HTTP_OK, nullptr, nullptr);
        }
        else {
          evhttp_send_reply(req, HTTP_NOTFOUND, nullptr, nullptr);
        }
      } catch (invalid_argument& e) {
        evhttp_send_reply(req, HTTP_BADREQUEST, e.what(), nullptr);
      }
    }
    else if (evhttp_request_get_command(req) == EVHTTP_REQ_GET) {
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
  } catch (exception& e) {
    evhttp_send_reply(req, HTTP_INTERNAL, e.what(), nullptr);
  }
}

void RestAPI::Start() {
  event_init();
  server = evhttp_start("0.0.0.0", port);
  if (!server) {
    throw runtime_error("Couldn't to initialize HTTP server!");
  }

  evhttp_set_gencb(server, Handler, nullptr);
  event_dispatch();
}

