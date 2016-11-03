#ifndef SQL_H_
#define SQL_H_

#include <inttypes.h>
#include <my_global.h>
#include <mysql.h>

#include "sql_schema.h"

#define SQL_DEFAULT_FIELD \
  unsigned int created_at;  \
  unsigned int updated_at;

typedef void *(*sql_load_cb)(uint64_t id);

int sql_init(void);
void sql_shutdown(void);

MYSQL_RES *sql_query(char *fmt, ...);
int sql_query_simple(char *fmt, ...);

Eina_List *sql_select(const char *table, const char *where, ...);
const void *sql_select_one(const char *table, const char *where, ...);

int sql_insert(const char *table, void **data);
int sql_update(const char *table, const void *data);

#endif
