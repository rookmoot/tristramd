#include <stdio.h>
#include <stdlib.h>
#include <Eina.h>

#include "tristram.h"
#include "sql.h"
#include "net.h"
#include "character.h"
#include "account.h"
#include "game.h"

struct sql_schema *account_schema_get(void) {
  struct sql_schema *schema = NULL;

  schema = sql_schema_new("accounts", SQL_ALLOC_CAST_CB(account_new), SQL_FREE_CAST_CB(account_free));
  schema_add_field(schema, "id",            SQL_TYPE_SERIAL,  8,   0, 0);
  schema_add_field(schema, "username",      SQL_TYPE_STRING,  16,  1, "");
  schema_add_field(schema, "password",      SQL_TYPE_STRING,  41,  1, "");
  schema_add_field(schema, "mail",          SQL_TYPE_STRING,  128, 1, "");
  schema_add_field(schema, "is_admin",      SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_field(schema, "is_operator",   SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_field(schema, "is_blocked",    SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_field(schema, "last_login_at", SQL_TYPE_INT,     4,   1, 0);
  schema_add_field(schema, "last_login_ip", SQL_TYPE_STRING,  16,  1, "");
  schema_add_default_fields(schema);
  
  return schema;
}

struct account *account_new(void) {
  struct account *account;

  account = malloc(sizeof(*account));
  if (!account) {
    EINA_LOG_CRIT("Failed to allocate account memory.");
    return NULL;
  }
  memset(account, 0, sizeof(*account));
  return account;
}

void account_free(struct account *account) {
  if (!account) {
    return;
  }
  free(account);
}


void account_connection_set(struct account *account, struct connection *conn) {
  account->connection = conn;
  conn->account = account;
}

struct connection *account_connection_get(struct account *account) {
  return account->connection;
}

unsigned int account_chat_flags_get(struct account *account) {
  account = account;
  return 0x00;
}

char *account_chat_name_get(struct account *account, const int full) {
  char name[1024];

  EINA_SAFETY_ON_NULL_RETURN_VAL(account, "");
  EINA_SAFETY_ON_NULL_RETURN_VAL(account->character, "");
  
  memset(name, 0, 1024);
  if (full) {
    snprintf(name, 1024, "%s*%s", account->character->name, account->username);
  } else {
    strcpy(name, (char *)account->character->name);
  }
  
  return strdup((const char *)name);
}

char *account_chat_statstring_get(struct account *account, const int full) {
  char tmp[1024];
  char statstring[1024];
  const char *realmname;
  
  EINA_SAFETY_ON_NULL_RETURN_VAL(account, "");
  EINA_SAFETY_ON_NULL_RETURN_VAL(account->character, "");

  memset(tmp, 0, 1024);
  memset(statstring, 0, 1024);

  character_get_statstring(account->character, (char *)tmp);

  if (full) {
    CONFIG_GET_STRING("servers.realm.name", &realmname);
    snprintf(statstring, 1024, "PX2D%s,%s,%s", realmname, account->character->name, tmp);
  } else {
    snprintf(statstring, 1024, "%s", tmp);
  }
  
  return strdup((const char *)statstring);
}

int account_game_set(struct account *account, struct game *game) {
  EINA_SAFETY_ON_NULL_RETURN_VAL(account, 0);
  EINA_SAFETY_ON_NULL_RETURN_VAL(game, 0);

  account->game = game;
  return 1;
}

struct game *account_game_get(struct account *account) {
  EINA_SAFETY_ON_NULL_RETURN_VAL(account, NULL);
  return account->game;
}



