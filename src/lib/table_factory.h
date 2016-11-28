#ifndef _MAD_TABLE_FACTORY_H_
#define _MAD_TABLE_FACTORY_H_

struct TableSpec;
struct BaseTable;

struct TableFactory {
  static BaseTable* Create(TableSpec& spec);
};

#endif /* _MAD_TABLE_FACTORY_H_ */
