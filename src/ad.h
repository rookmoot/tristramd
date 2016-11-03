#ifndef AD_H_
#define AD_H_

#include "sql.h"

struct ad {
  long int id;
  char filename[128];
  char url[1024];

  uint32_t clicked;
  
  SQL_DEFAULT_FIELD;
};


struct sql_schema *ad_schema_get(void);

struct ad *ad_new(void);
void ad_free(struct ad *ad);

#endif
