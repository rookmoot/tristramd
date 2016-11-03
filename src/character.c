#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <Eina.h>

#include "character.h"

#define CHARACTER_CHECKSUM_OFFSET 0x0C

//static inline struct character *character_default_get(void);
static inline struct character *character_load_from_file(const char *path);
static inline int character_path_get(const char *name, char *path, unsigned int size);
static inline int character_checksum(unsigned char const * data, unsigned int len, unsigned int offset);

struct character *character_create(const char *name, enum character_class class, character_status status) {
  //  int fd = -1;
  char filepath[1024];
  struct character *character = NULL;
  
  character_path_get(name, (char *)&filepath, 1024);
  EINA_LOG_DBG("Character file : %s", filepath);

  character = character_load_from_file("cache/newbie.save");
  if (!character) {
    return NULL;
  }

  character->level = (uint8_t)0x01;
  character->version = (uint8_t)0x60;
  character->timestamp = (uint32_t)time(NULL);
  character->status = (uint8_t)status;
  character->class = (uint8_t)class;
  memset(character->name, '\0', CHARACTER_MAX_NAME_LEN);
  strncpy((char *)character->name, name, CHARACTER_MAX_NAME_LEN);
  character->name[CHARACTER_MAX_NAME_LEN - 1] = '\0';
  
  return character;
}

int character_delete(struct account *account, const char *name) {
  char filepath[1024];

  character_path_get(name, (char *)&filepath, 1024);
  EINA_LOG_DBG("Character file : %s", filepath);

  if (sql_query_simple("DELETE FROM accounts_characters WHERE account_id=%d and name='%s'", account->id, name)) {
    return (unlink(filepath) == 0) ? 1 : 0;
    return 1;
  }
  
  return 0;
}

struct character *character_load(struct account *account, const char *name) {
  char filepath[1024];
  struct accounts_characters *ac = NULL;

  if (!account || !account->id || !strlen(name)) {
    return NULL;
  }

  ac = (struct accounts_characters *)sql_select_one("accounts_characters", "account_id=%d and name='%s'", account->id, name);
  if (!ac) {
    EINA_LOG_CRIT("Failed to get a character from sql query while loading one.");
    return NULL;
  }

  character_path_get(ac->name, (char *)&filepath, 1024);  
  return character_load_from_file(filepath);
}

int character_save(struct account *account, struct character *character) {
  char path[1024];
  FILE *file = NULL;
  struct accounts_characters *ac = NULL;
  
  character->checksum = character_checksum(
    (unsigned char const *)character,
    sizeof(*character),
    CHARACTER_CHECKSUM_OFFSET
  );
  character->size = sizeof(*character);
  
  character_path_get((char *)character->name, path, 1024);
  file = fopen(path, "w+");
  if (!file) {
    EINA_LOG_CRIT("Failed to open file %s", path);
    return 0;
  }

  fwrite((void *)character, 1, (long)sizeof(*character), file);
  fclose(file);

  if (!(ac = account_character_new())) {
    return 0;
  }
  
  ac->account_id = account->id;
  strncpy(ac->name, (const char *)character->name, strlen((const char *)character->name));
  ac->class = character->class;
  ac->level = character->level;
  ac->is_hardcore  = CHARACTER_STATUS_IS_HARDCORE(character->status);
  ac->is_dead      = CHARACTER_STATUS_IS_DEAD(character->status);
  ac->is_expansion = CHARACTER_STATUS_IS_EXPANSION(character->status);
  ac->is_ladder    = CHARACTER_STATUS_IS_LADDER(character->status);

  if (!sql_insert("accounts_characters", (void **)&ac)) {
    EINA_LOG_CRIT("Failed to insert newly created account character");
    account_character_free(ac);
    return 0;
  }

  account->character = character;
  
  return 1;
}

void character_free(struct character *character) {
  if (!character) {
    return;
  }
  free(character);
}

int character_get_statstring(struct character *character, char *statstring) {
  if (!character) {
    return 0;
  }

  memset(statstring, 0, 33);

  statstring[0]  = 0x84;
  statstring[1]  = 0x80;
  statstring[2]  = 0xFF; // Equipment: Head
  statstring[3]  = 0xFF; // Equipment: Torso
  statstring[4]  = 0xFF; // Equipment: Legs
  statstring[5]  = 0xFF; // Equipment: Right Arm
  statstring[6]  = 0xFF; // Equipment: Left Arm
  statstring[7]  = 0xFF; // Equipment: Right Weapon
  statstring[8]  = 0xFF; // Equipment: Left Weapon
  statstring[9]  = 0xFF; // Equipment: Left Shield
  statstring[10] = 0xFF; // Equipment: Right Shoulder Pad
  statstring[11] = 0xFF; // Equipment: Left Shoulder Pad
  statstring[12] = 0xFF; // Left Item (Heads)
  statstring[13] = (char)character->class + 1;
  statstring[14] = 0xFF; // Color: Head
  statstring[15] = 0xFF; // Color: Torso
  statstring[16] = 0xFF; // Color: Legs
  statstring[17] = 0xFF; // Color: Right Arm
  statstring[18] = 0x06; // Color: Left Arm
  statstring[19] = 0xFF; // Color: Right Weapon
  statstring[20] = 0xFF; // Color: Left Weapon
  statstring[21] = 0xFF; // Color: Shield
  statstring[22] = 0xFF; // Color: Right Shoulder Pad
  statstring[23] = 0xFF; // Color: Left Shoulder Pad
  statstring[24] = 0xFF; // Color: Left Item (Heads)
  statstring[25] = (char)character->level;
  statstring[26] = (char)character->status;
  statstring[27] = 0x80;
  statstring[28] = 0x80;
  statstring[29] = 0x80;
  statstring[30] = (char)(CHARACTER_STATUS_IS_LADDER(character->status) ? 0x01 : 0xFF);
  statstring[31] = 0xFF;
  statstring[32] = 0xFF;
  
  return 1;
}


static inline struct character *character_load_from_file(const char *path) {
  FILE *file = NULL;
  struct character *newbie = NULL;
  
  newbie = malloc(sizeof(*newbie));
  if (!newbie) {
    EINA_LOG_CRIT("Failed to allocate newbie character memory.");
    return NULL;
  }
  memset(newbie, 0, sizeof(*newbie));
  
  file = fopen(path, "rb");
  if (!file) {
    EINA_LOG_CRIT("Failed to open file %s", path);
    return NULL;
  }

  fread((void *)newbie, 1, (long)sizeof(*newbie), file);
  fclose(file);
  return newbie;
}

static inline int character_path_get(const char *name, char *path, unsigned int size) {
  memset(path, 0, size);
  snprintf(path, size, "cache/save/%s.d2s", name);
  return 1;
}

static inline int character_checksum(unsigned char const * data, unsigned int len, unsigned int offset) {
  int checksum;
  unsigned int i;
  unsigned int ch;

  if (!data) {
    return 0;
  }
  
  checksum = 0;
  for (i = 0; i < len; i++) {
    if (i >= offset && i < offset + sizeof(int)) {
      ch = 0;
    }
    else {
      ch = *data;
    }
    ch += (checksum < 0);
    checksum = 2 * checksum + ch;
    data++;
  }
  return checksum;
}


/*
struct accounts_characters {
  long int id;
  long int account_id;

  char name[CHARACTER_MAX_NAME_LEN];
  uint8_t class;
  uint16_t level;

  uint8_t is_hardcore;
  uint8_t is_dead;
  uint8_t is_expansion;
  uint8_t is_ladder;

  SQL_DEFAULT_FIELD;
};
*/

struct sql_schema *account_character_schema_get(void) {
  struct sql_schema *schema = NULL;

  schema = sql_schema_new("accounts_characters", SQL_ALLOC_CAST_CB(account_character_new), SQL_FREE_CAST_CB(account_character_free));
  schema_add_field(schema, "id",            SQL_TYPE_SERIAL,  8,   0, 0);
  schema_add_field(schema, "account_id",    SQL_TYPE_LONG,    8,   0, 0);
  schema_add_field(schema, "name",          SQL_TYPE_STRING,  CHARACTER_MAX_NAME_LEN,  1, "");
  schema_add_field(schema, "class",         SQL_TYPE_INT,     4,   1, 0);
  schema_add_field(schema, "level",         SQL_TYPE_INT,     4,   1, 0);
  schema_add_field(schema, "experience",    SQL_TYPE_LONG,    8,   1, 0);
  schema_add_field(schema, "is_hardcore",   SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_field(schema, "is_dead",       SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_field(schema, "is_expansion",  SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_field(schema, "is_ladder",     SQL_TYPE_BOOL,    1,   1, 0);
  schema_add_default_fields(schema);

  return schema;
}

struct accounts_characters *account_character_new(void) {
  struct accounts_characters *ac = NULL;

  ac = malloc(sizeof(*ac));
  if (!ac) {
    EINA_LOG_CRIT("Failed to allocate account characters memory.");
    return NULL;
  }
  memset(ac, 0, sizeof(*ac));
  return ac;
}

void account_character_free(struct accounts_characters *ac) {
  free(ac);
}
