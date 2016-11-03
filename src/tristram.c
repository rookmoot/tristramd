#include <stdio.h>
#include <stdlib.h>
#include <Eina.h>

#include "sql.h"
#include "net.h"
#include "protocol.h"
#include "chat.h"
#include "game.h"
#include "tristram.h"

config_t cfg;


int tristram_init(const char *configfile) {
  eina_init();

  config_init(&cfg);
  if (!config_read_file(&cfg, configfile)) {
    EINA_LOG_ERR("%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return 0;
  }

  if (!sql_init()) {
    return -1;
  }

  chat_init();
  protocol_init();
  game_init();
  
  return 1;
}

void tristram_shutdown(void) {
  config_destroy(&cfg);
  protocol_shutdown();
  game_shutdown();
  chat_shutdown();
  sql_shutdown();
  eina_shutdown();
}
