#include <stdexcept>
#include "table_factory.h"
#include "table.h"

BaseTable* TableFactory::Create(TableSpec& spec) {

  size_t dims_count = spec.DimsCount();
  size_t metrics_count = spec.MetricsCount();




  if (dims_count == 8 && metrics_count == 3) {
    return new Table<8,3>(spec);
  }


  throw std::invalid_argument(
      "This store supports tables with following sizes only: 8x3"
  );
}
