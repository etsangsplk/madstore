#ifndef MAD_STORE_FACTORY_H
#define MAD_STORE_FACTORY_H

#ifndef DIMS_X
# define DIMS_X 1
#endif

#ifndef METRICS_X
# define METRICS_X 1
#endif

struct StoreSpec;
struct BaseStore;

struct StoreFactory {
  static BaseStore* Create(StoreSpec& store_spec);
};

#endif /* MAD_STORE_FACTORY_H */
