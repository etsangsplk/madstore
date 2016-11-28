#include <cstring>
#include <cstdint>
#include <stdexcept>
#include <memory>
#include <sstream>
#include "rest_api.h"
#include "easylogging++.h"
#include "json.hpp"
#include "../lib/types.h"

using json = nlohmann::json;

void RestAPI::Start() {
   server.resource["/api/query"]["POST"] =
     [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      query_pool->push([=](int id) {
        try {
          json query = json::parse(request->content.string());
          std::vector<std::pair<std::vector<std::string>,std::vector<MetricType>>> result;
          store.RunQuery(query, result);
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
          std::string o = std::move(os.str());
          *response<<"HTTP/1.1 200 OK\r\nContent-Type: text/tab-separated-values\r\nContent-Length: "<<o.length()<<"\r\n\r\n" <<o;
        } catch (std::exception& e) {
          *response<<"HTTP/1.1 400 Bad Request\r\nContent-Length: "<<strlen(e.what())<<"\r\n\r\n"<<e.what();
        }
      });
   };

   server.resource["/api/load"]["POST"] =
     [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      write_pool->push([=](int id) {
        try {
          json spec = json::parse(request->content.string());
          InputSpec input_spec(spec);
          store.Read(input_spec);
          *response<<"HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        } catch (std::exception& e) {
          *response<<"HTTP/1.1 400 Bad Request\r\nContent-Length: "<<strlen(e.what())<<"\r\n\r\n"<<e.what();
        }
      });
   };

   server.resource["/api/optimize-memory"]["POST"] =
     [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      write_pool->push([=](int id) {
        try {
          store.OptimizeMemUsage();
          *response<<"HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        } catch (std::exception& e) {
          *response<<"HTTP/1.1 400 Bad Request\r\nContent-Length: "<<strlen(e.what())<<"\r\n\r\n"<<e.what();
        }
      });
   };

#ifdef PERSIST
   server.resource["/api/persist"]["POST"] =
     [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      write_pool->push([=](int id) {
        try {
          store.Persist();
          *response<<"HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        } catch (std::exception& e) {
          *response<<"HTTP/1.1 400 Bad Request\r\nContent-Length: "<<strlen(e.what())<<"\r\n\r\n"<<e.what();
        }
      });
   };
#endif

   server.resource["/api/stats"]["GET"] =
     [&](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
      query_pool->push([=](int id) {
        try {
          json stats;
          store.GetStats(stats);
          std::string o = std::move(stats.dump(2));
          *response<<"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "<<o.length()<<"\r\n\r\n" <<o;
        } catch (std::exception& e) {
          *response<<"HTTP/1.1 400 Bad Request\r\nContent-Length: "<<strlen(e.what())<<"\r\n\r\n"<<e.what();
        }
     });
   };

   server.start();
}

