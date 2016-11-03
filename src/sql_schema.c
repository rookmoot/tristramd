#include <stdio.h>
#include <stdlib.h>
#include <Eina.h>

#include "sql.h"
#include "sql_schema.h"

static void sql_schema_field_free(struct sql_schema_field *field);

struct sql_schema *sql_schema_new(const char *table_name, sql_schema_alloc_cb alloc_cb, sql_schema_free_cb free_cb) {
  struct sql_schema *schema = NULL;

  schema = malloc(sizeof(*schema));
  if (!schema) {
    EINA_LOG_CRIT("Failed to allocate schema memory.");
    return NULL;
  }

  schema->table_name = strdup(table_name);
  schema->alloc_cb = alloc_cb;
  schema->free_cb = free_cb;

  schema->fields = NULL;  
  return schema;
}

void sql_schema_free(struct sql_schema *schema) {
  struct sql_schema_field *field = NULL;
  
  free(schema->table_name);
  EINA_LIST_FREE(schema->fields, field) {
    sql_schema_field_free(field);
  }
  free(schema);
}

void schema_add_field(struct sql_schema *schema, const char *field_name, enum sql_type type, unsigned int length, char required, void *default_value) {
  struct sql_schema_field *field = NULL;

  field = malloc(sizeof(*field));
  if (!field) {
    EINA_LOG_CRIT("Failed to allocate field schema memory.");
    return;
  }

  field->field_name = strdup(field_name);
  field->type = type;
  field->required = required;
  field->length = length;
  field->default_value = default_value;

  schema->fields = eina_list_append(schema->fields, field);
}

void schema_add_default_fields(struct sql_schema *schema) {
  schema_add_field(schema, "created_at", SQL_TYPE_INT, 4, 0, 0);
  schema_add_field(schema, "updated_at", SQL_TYPE_INT, 4, 0, 0);
}

static void sql_schema_field_free(struct sql_schema_field *field) {
  free(field->field_name);
  free(field);
}
