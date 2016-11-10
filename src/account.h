#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include "sql.h"
#include "net.h"

struct account {
  long int id;
  char username[16];
  char password[41];
  char mail[128];

  uint8_t is_admin;
  uint8_t is_operator;
  uint8_t is_blocked;

  uint32_t last_login_at;
  char last_login_ip[16];

  SQL_DEFAULT_FIELD;

  struct connection *connection;
  struct character *character;
  struct game *game;
  char channel[128];
};

struct sql_schema *account_schema_get(void);

struct account *account_new(void);
void account_free(struct account *account);

void account_connection_set(struct account *account, struct connection *conn);
struct connection *account_connection_get(struct account *account);

void account_set_connected(struct account *account);

unsigned int account_chat_flags_get(struct account *account);
char *account_chat_name_get(struct account *account, const int full);
char *account_chat_statstring_get(struct account *account, const int full);

int account_game_set(struct account *account, struct game *game);
struct game *account_game_get(struct account *account);

#endif
