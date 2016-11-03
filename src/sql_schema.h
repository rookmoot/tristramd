#ifndef SQL_SCHEMA_H_
#define SQL_SCHEMA_H_

typedef void *(*sql_schema_alloc_cb)(void);
typedef void (*sql_schema_free_cb)(void *);

enum sql_type {
  SQL_TYPE_BOOL = 0,
  SQL_TYPE_SHORT = 1,
  SQL_TYPE_INT = 2,
  SQL_TYPE_LONG = 3,
  SQL_TYPE_SERIAL = 4,
  SQL_TYPE_STRING = 5,
};

struct sql_schema {
  char *table_name;

  sql_schema_alloc_cb alloc_cb;
  sql_schema_free_cb free_cb;
  
  Eina_List *fields;
};

struct sql_schema_field {
  char *field_name;
  enum sql_type type;
  unsigned int length;
  char required;
  void *default_value;
};

#define SQL_ALLOC_CAST_CB(cb) (void *(*)(void))cb
#define SQL_FREE_CAST_CB(cb) (void (*)(void *))cb

struct sql_schema *sql_schema_new(const char *table_name, sql_schema_alloc_cb alloc_cb, sql_schema_free_cb free_cb);
void sql_schema_free(struct sql_schema *schema);

void schema_add_field(struct sql_schema *schema, const char *field_name, enum sql_type type, unsigned int length, char required, void *default_value);
void schema_add_default_fields(struct sql_schema *schema);

#endif
