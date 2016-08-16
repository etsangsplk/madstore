#include "logger.h"

INITIALIZE_EASYLOGGINGPP

void Logger::Init() {
  el::Configurations log_conf;
  log_conf.setToDefault();
  log_conf.setGlobally(el::ConfigurationType::ToFile, "false");
  log_conf.setGlobally(el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s.%g} %level [%logger-%thread] %msg");

  el::Loggers::getLogger("RestAPI");
  el::Loggers::getLogger("Store");
  el::Loggers::getLogger("Query");
  el::Loggers::reconfigureAllLoggers(log_conf);
}
