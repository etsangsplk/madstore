#ifndef _MAD_STORE_FACTORY_H_
#define _MAD_STORE_FACTORY_H_

struct StoreSpec;
struct BaseStore;

struct StoreFactory {
  static BaseStore* Create(StoreSpec& store_spec);
};

#endif /* _MAD_STORE_FACTORY_H_ */
