#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "utils.h"
#include "character.h"
#include "account.h"
#include "game.h"

static Eina_List *_games = NULL;

int game_init(void) {
  return 1;
}

void game_shutdown(void) {
  struct game *game = NULL;

  EINA_LIST_FREE(_games, game) {
    game_free(game);
  }
}

struct game *game_exists(const char *name, const char *password) {
  Eina_List *l = NULL;
  struct game *game = NULL;

  EINA_LIST_FOREACH(_games, l, game) {
    if (!strcmp(game->name, utils_string_to_lower(name))) {
      if (strlen(password)) {
	if (!strcmp(password, utils_string_to_lower(password))) {
	  return game;
	} else {
	  continue;
	}
      } else {
	return game;
      }
    }
  }
  return NULL;
}

struct game *game_exists_by_token(uint16_t token) {
  Eina_List *l = NULL;
  struct game *game = NULL;

  EINA_LIST_FOREACH(_games, l, game) {
    if (game->token == token) {
      return game;
    }
  }
  return NULL;
}
  

struct game *game_create(const char *name, const char *password, const char *description,
			 unsigned int maxplayers, unsigned int level_difference,
			 unsigned int difficulty) {
  struct game *game = NULL;

  game = malloc(sizeof(*game));
  if (!game) {
    EINA_LOG_CRIT("Failed to allocate game memory.");
    return NULL;
  }

  memset(game, 0, sizeof(*game));

  game->name = (const char *)strdup((char *)utils_string_to_lower(name));
  game->password = (const char *)strdup((char *)utils_string_to_lower(password));
  game->description = (const char *)strdup(description);

  game->maxplayers = maxplayers;
  game->level_difference = level_difference;
  game->difficulty = difficulty;

  game->token = (uint16_t)rand();
  game->started_at = (uint32_t)time(NULL);

  EINA_LOG_INFO("Creating new game with token : %d", game->token);

  _games = eina_list_append(_games, game);
  
  return game;
}

int game_remove_by_token(uint16_t token) {
  struct game *game = NULL;

  game = game_exists_by_token(token);
  if (game) {
    game_free(game);
    return 1;
  }
  return 0;
}

void game_free(struct game *game) {
  free((char *)game->name);
  free((char *)game->password);
  free((char *)game->description);
  eina_list_free(game->players);
  free(game);
}

uint16_t game_token_get(struct game *game) {
  return game->token;
}

int game_account_join(struct game *game, struct account *account) {
  game = game; account = account;
  return 0;
}

int game_account_leave(struct game *game, struct account *account) {
  game = game; account = account;
  return 0;
}
