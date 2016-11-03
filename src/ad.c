#include <stdio.h>
#include <Eina.h>

#include "ad.h"

struct sql_schema *ad_schema_get(void) {
  struct sql_schema *schema = NULL;

  schema = sql_schema_new("ads", SQL_ALLOC_CAST_CB(ad_new), SQL_FREE_CAST_CB(ad_free));
  schema_add_field(schema, "id",            SQL_TYPE_SERIAL,  8,    0, 0);
  schema_add_field(schema, "filename",      SQL_TYPE_STRING,  128,  1, "");
  schema_add_field(schema, "url",           SQL_TYPE_STRING,  1024, 1, "");
  schema_add_field(schema, "clicked",       SQL_TYPE_INT,     4,    1, "");
  schema_add_default_fields(schema);

  return schema;
  
}

struct ad *ad_new(void) {
  struct ad *ad = NULL;

  ad = malloc(sizeof(*ad));
  if (!ad) {
    EINA_LOG_CRIT("Failed to allocate ad memory");
  }
  memset(ad, 0, sizeof(*ad));

  return ad;
}

void ad_free(struct ad *ad) {
  if (ad) {
    free(ad);
  }
}
