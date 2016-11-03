#ifndef GAME_H_
#define GAME_H_

/*
 * (WORD) Request Id *
 * (DWORD) Difficulty
 * (BYTE) Unknown - 1
 * (BYTE) Player difference **
 * (BYTE) Maximum players
 * (STRING) Game name
 * (STRING) Game password
 * (STRING) Game description
 *
 * Remarks :
 * <dl>
 * <dt>Difficulty:</dt>
 * <dd>0x0000: Normal</dd>
 * <dd>0x1000: Nightmare</dd>
 * <dd>0x2000: Hell</dd>
 * </dl>                       
 */

#define DIFFICULTY_NORMAL     0x0000
#define DIFFICULTY_NIGHTMARE  0x1000
#define DIFFICULTY_HELL       0x2000

struct game {
  const char *name;
  const char *password;
  const char *description;

  uint16_t token;
  uint32_t started_at;

  unsigned int level_difference;
  unsigned int difficulty;

  Eina_List *players;
  unsigned int maxplayers;
};

int game_init(void);
void game_shutdown(void);

struct game *game_exists(const char *name, const char *password);
struct game *game_exists_by_token(uint16_t token);

struct game *game_create(const char *name, const char *password, const char *description,
			 unsigned int maxplayers, unsigned int level_difference,
			 unsigned int difficulty);
void game_free(struct game *game);

uint16_t game_token_get(struct game *game);

int game_account_join(struct game *game, struct account *account);
int game_account_leave(struct game *game, struct account *account);

#endif
