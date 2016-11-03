#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <inttypes.h>
#include "sql.h"
#include "account.h"

#define CHARACTER_MAX_NAME_LEN 16

struct accounts_characters {
  long int id;
  long int account_id;
  
  char name[CHARACTER_MAX_NAME_LEN];
  uint32_t class;
  uint32_t level;
  uint64_t experience;

  uint8_t is_hardcore;
  uint8_t is_dead;
  uint8_t is_expansion;
  uint8_t is_ladder;

  SQL_DEFAULT_FIELD;
};


enum character_class {
  CLASS_AMAZON      = 0,
  CLASS_SORCERESS   = 1,
  CLASS_NECROMANCER = 2,
  CLASS_PALADIN     = 3,
  CLASS_BARBARIAN   = 4,
  CLASS_DRUID       = 5,
  CLASS_ASSASSIN    = 6,
};

typedef unsigned char character_status;
#define STATUS_CLASSIC   0x00
#define STATUS_HARDCORE  0x04
#define STATUS_DEAD      0x08
#define STATUS_EXPANSION 0x20
#define STATUS_LADDER    0x40

#define CHARACTER_STATUS_TEST(x, mask) ((x & mask) != 0 ? 1 : 0)
#define CHARACTER_STATUS_IS_CLASSIC(x) CHARACTER_STATUS_TEST(x, STATUS_CLASSIC)
#define CHARACTER_STATUS_IS_HARDCORE(x) CHARACTER_STATUS_TEST(x, STATUS_HARDCORE)
#define CHARACTER_STATUS_IS_DEAD(x) CHARACTER_STATUS_TEST(x, STATUS_DEAD)
#define CHARACTER_STATUS_IS_EXPANSION(x) CHARACTER_STATUS_TEST(x, STATUS_EXPANSION)
#define CHARACTER_STATUS_IS_LADDER(x) CHARACTER_STATUS_TEST(x, STATUS_LADDER)


#pragma pack(push, character_h, 1)
struct character {
  uint32_t signature;
  uint32_t version;
  uint32_t size;
  uint32_t checksum;
  uint32_t active_arms;
  uint8_t  name[CHARACTER_MAX_NAME_LEN];
  uint8_t  status;
  uint8_t  progression;
  uint16_t unk0;
  uint8_t  class;
  uint16_t  unk1;
  uint8_t  level;
  uint16_t unk2;
  uint32_t timestamp;
  uint32_t unk3;
  uint32_t hotkeys[16];
  uint32_t mouse[2];
  uint32_t mouse_action[2];

  uint8_t  unk4[32];
  uint8_t  difficulty[3];
  uint32_t map_id;
  uint16_t unk5;
  uint16_t merc_dead;
  uint32_t merc_id;
  uint16_t merc_index;
  uint16_t merc_attr;
  uint32_t merc_experience;
  
  uint8_t  unk6[144];
  uint8_t  quests[298];
  uint8_t  waypoints[81];

  uint8_t  padding[192];
};
#pragma pack(pop, character_h)

struct character *character_create(const char *name, enum character_class class, character_status status);
int character_delete(struct account *account, const char *name);
void character_free(struct character *character);

struct character *character_load(struct account *account, const char *name);
int character_save(struct account *account, struct character *character);

int character_get_statstring(struct character *character, char *statstring);

struct sql_schema *account_character_schema_get(void);
struct accounts_characters *account_character_new(void);
void account_character_free(struct accounts_characters *ac);
  
#endif
