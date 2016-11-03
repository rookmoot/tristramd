#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <Eina.h>

#include "sql.h"
#include "tristram.h"

#include "ad.h"
#include "account.h"
#include "character.h"
#include "chat.h"

static MYSQL     *_mysql = NULL;
static Eina_Hash *_schema = NULL;

#define SQL_FIELD_ASSIGN_BOOL(data, dlength, value, length) ((char *)data)[dlength] = (value[0] == '0' ? 0 : 1); dlength += length; 
#define SQL_FIELD_ASSIGN_SHORT(data, dlength, value, length) short v = 0; v = atoi(value); memcpy(data + dlength, &v, length); dlength += length;
#define SQL_FIELD_ASSIGN_INT(data, dlength, value, length) int v = 0; v = atoi(value); memcpy(data + dlength, &v, length); dlength += length;
#define SQL_FIELD_ASSIGN_LONG(data, dlength, value, length) long long int v = 0; v = atol(value); memcpy(data + dlength, &v, length); dlength += length;

#define SQL_FIELD_GET_BOOL(data, dlength, length)  *(uint8_t *)(data+ dlength)
#define SQL_FIELD_GET_SHORT(data, dlength, length) *(uint16_t *)(data+ dlength)
#define SQL_FIELD_GET_INT(data, dlength, length)   *(uint32_t *)(data+ dlength)
#define SQL_FIELD_GET_LONG(data, dlength, length)  *(uint64_t *)(data+ dlength)

struct sql_caster {
  long int id;
};

int sql_init(void) {
  char *hostname;
  char *username;
  char *password;
  char *database;

  struct sql_schema *schema = NULL;

  if (mysql_library_init(0, NULL, NULL)) {
    EINA_LOG_CRIT("could not initialize MySQL library.");
    exit(1);
  }
  
  _mysql = mysql_init(NULL);
  if (_mysql == NULL) {
    EINA_LOG_CRIT("Failed to init mysql database.");
    return 0;
  }

  CONFIG_GET_STRING("database.hostname", &hostname);
  CONFIG_GET_STRING("database.username", &username);
  CONFIG_GET_STRING("database.password", &password);
  CONFIG_GET_STRING("database.database", &database);
  
  // now,  connect to mysql database.
  if (mysql_real_connect(_mysql, hostname, username, password, database, 0, NULL, 0) == NULL) {
    EINA_LOG_CRIT("MYSQL : Failed to connect to database.");
    EINA_LOG_CRIT("MYSQL : You may need to edit your configuration file,");
    EINA_LOG_CRIT("MYSQL : or create your database manualy.");
    return 0;
  }
  
  EINA_LOG_DBG("MYSQL: Connected to mysql %s:%s@%s", username, hostname, database);

  _schema = eina_hash_string_superfast_new(NULL);

  schema = ad_schema_get();
  eina_hash_add(_schema, schema->table_name, schema);

  schema = account_schema_get();
  eina_hash_add(_schema, schema->table_name, schema);

  schema = account_character_schema_get();
  eina_hash_add(_schema, schema->table_name, schema);
  
  return 1;
}

void sql_shutdown(void) {
  Eina_Iterator *it;
  struct sql_schema *schema = NULL;
  
  mysql_close(_mysql);
  
  it = eina_hash_iterator_data_new(_schema);
  while (eina_iterator_next(it, (void **)&schema)) {
    sql_schema_free(schema);
  }
  eina_iterator_free(it);
  eina_hash_free(_schema);

  mysql_library_end();
}

MYSQL_RES *sql_query(char *fmt, ...) {
  va_list ap;
  char buffer[4096];
  MYSQL_RES *result = NULL;
  
  memset(buffer, 0, 4096);

  va_start(ap, fmt);
  vsnprintf(buffer, 4096, fmt, ap);
  va_end(ap);

  EINA_LOG_DBG("Query: %s", buffer);

  if (mysql_query(_mysql, buffer)) {
    EINA_LOG_ERR("Failed to run query: %s with error: %s", buffer, mysql_error(_mysql));
    return NULL;
  }

  result = mysql_store_result(_mysql);
  if (result == NULL) {
    EINA_LOG_ERR("Failed to get result after query: %s", buffer);
  }  
  return result;
}

int sql_query_simple(char *fmt, ...) {
  int ret = 0;
  va_list ap;
  char buffer[4096];
  
  memset(buffer, 0, 4096);

  va_start(ap, fmt);
  vsnprintf(buffer, 4096, fmt, ap);
  va_end(ap);

  EINA_LOG_DBG("Query: %s", buffer);

  if ((ret = mysql_query(_mysql, buffer)) != 0) {
    EINA_LOG_ERR("Failed to run query: %s with error: %s", buffer, mysql_error(_mysql));
    return 0;
  }
  
  return 1;
}

Eina_List *sql_select(const char *table, const char *where, ...) {
  va_list ap;
  char buffer[4096];
  Eina_List *list = NULL, *l = NULL;
  MYSQL_RES *result = NULL;
  struct sql_schema *schema = NULL;
  struct sql_schema_field *field = NULL;
  
  
  memset(buffer, 0, 4096);

  va_start(ap, where);
  vsnprintf(buffer, 4096, where, ap);
  va_end(ap);

  schema = eina_hash_find(_schema, table);
  if (!schema) {
    EINA_LOG_CRIT("Failed to find %s in global sql schema.", table);
    return NULL;
  }
  
  result = sql_query("SELECT * FROM %s WHERE %s", table, buffer);  
  if (result) {
    MYSQL_ROW row;
    unsigned int i = 0;
    unsigned int num_fields = mysql_num_fields(result);;
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    while ((row = mysql_fetch_row(result))) {
      void *data;
      int length = 0;
      data = schema->alloc_cb();
      if (data) {
	for (i = 0; i < num_fields; i++) {
	  EINA_LIST_FOREACH(schema->fields, l, field) {
	    if (!strncmp(fields[i].name, field->field_name, strlen(field->field_name))) {
	      if (field->type == SQL_TYPE_BOOL) {
		SQL_FIELD_ASSIGN_BOOL(data, length, row[i], field->length);
	      } else if (field->type == SQL_TYPE_SHORT) {
		SQL_FIELD_ASSIGN_SHORT(data, length, row[i], field->length);
	      } else if (field->type == SQL_TYPE_INT) {
		SQL_FIELD_ASSIGN_INT(data, length, row[i], field->length);
	      } else if (field->type == SQL_TYPE_LONG) {
		SQL_FIELD_ASSIGN_LONG(data, length, row[i], field->length);
	      } else if (field->type == SQL_TYPE_SERIAL) {
		SQL_FIELD_ASSIGN_LONG(data, length, row[i], field->length);
	      } else if (field->type == SQL_TYPE_STRING) {
		  if (strlen(row[i])) {
		    memcpy(data + length, row[i], strlen(row[i]));
		  }
		  length += field->length;
	      }
	    }
	  }
	}
	// append struct to returning list
	list = eina_list_append(list, data);
      }
    }
    mysql_free_result(result);
  }
  return list;
}

const void *sql_select_one(const char *table, const char *where, ...) {
  va_list ap;
  char buffer[4096];
  Eina_List *list = NULL;
  
  memset(buffer, 0, 4096);

  va_start(ap, where);
  vsnprintf(buffer, 4096, where, ap);
  va_end(ap);

  list = sql_select(table, "%s LIMIT 1", buffer);
  return eina_list_data_get(list);
}

int sql_insert(const char *table, void **data) {
  char sets[4096];
  int length = 0;
  Eina_List *l = NULL;
  struct sql_schema *schema = NULL;
  struct sql_schema_field *field = NULL;

  memset(sets, 0, 4096);
  schema = eina_hash_find(_schema, table);

  EINA_LIST_FOREACH(schema->fields, l, field) {
    if (!strcmp(field->field_name, "created_at")) {
      int t = (int)time(NULL);
      memcpy((void *)(*data + length), &t, sizeof(t));
    }
    
    if (field->type == SQL_TYPE_BOOL) {
      sprintf(sets, "%s%d, ", sets, SQL_FIELD_GET_BOOL(*data, length, field->length));
    } else if (field->type == SQL_TYPE_SHORT) {
      sprintf(sets, "%s%d, ", sets, SQL_FIELD_GET_SHORT(*data, length, field->length));
    } else if (field->type == SQL_TYPE_INT) {
      sprintf(sets, "%s%d, ", sets, SQL_FIELD_GET_INT(*data, length, field->length));
    } else if (field->type == SQL_TYPE_LONG) {
      sprintf(sets, "%s%ld, ", sets, SQL_FIELD_GET_LONG(*data, length, field->length));
    } else if (field->type == SQL_TYPE_SERIAL) {
      sprintf(sets, "%sNULL, ", sets);
    } else if (field->type == SQL_TYPE_STRING) {
      sprintf(sets, "%s'%s', ", sets, (char *)(*data + length));
    }
    length += field->length;
  }

  sets[strlen(sets) - 2] = '\0';
  
  if (sql_query_simple("INSERT INTO %s VALUES(%s)", table, sets)) {
    long long int id = (long long int)mysql_insert_id(_mysql);
    if (id > 0) {
      memcpy((*data), &id, sizeof(id));
      return 1;
    }
  }
  return 0;
}

int sql_update(const char *table, const void *data) {
  char sets[4096];
  int length = 0;
  long int id = 0;
  Eina_List *l = NULL;
  struct sql_schema *schema = NULL;
  struct sql_schema_field *field = NULL;

  memset(sets, 0, 4096);
  schema = eina_hash_find(_schema, table);

  EINA_LIST_FOREACH(schema->fields, l, field) {
    if (!strcmp(field->field_name, "updated_at")) {
      int t = (int)time(NULL);
      memcpy((void *)(data + length), &t, sizeof(t));
    }

    if (field->type == SQL_TYPE_SERIAL) {
      id = SQL_FIELD_GET_LONG(data, length, field->length);
    }
    
    if (field->type == SQL_TYPE_BOOL) {
      sprintf(sets, "%s%s=%d, ", sets, field->field_name, SQL_FIELD_GET_BOOL(data, length, field->length));
    } else if (field->type == SQL_TYPE_SHORT) {
      sprintf(sets, "%s%s=%d, ", sets, field->field_name, SQL_FIELD_GET_SHORT(data, length, field->length));
    } else if (field->type == SQL_TYPE_INT) {
      sprintf(sets, "%s%s=%d, ", sets, field->field_name, SQL_FIELD_GET_INT(data, length, field->length));
    } else if (field->type == SQL_TYPE_LONG) {
      sprintf(sets, "%s%s=%ld, ", sets, field->field_name, SQL_FIELD_GET_LONG(data, length, field->length));
    } else if (field->type == SQL_TYPE_STRING) {
      sprintf(sets, "%s%s='%s', ", sets, field->field_name, (char *)(data + length));
    }
    length += field->length;
  }

  sets[strlen(sets) - 3] = '\0';
  
  if (id > 0) {
    return sql_query_simple("UPDATE %s SET %s WHERE id=%d", table, sets, id);
  }
  return 0;
}
