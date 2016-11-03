#ifndef TRISTRAM_H_
#define TRISTRAM_H_

#include <libconfig.h>

extern config_t cfg;

#define CONFIG_GET_STRING(key, value)							\
  if (!config_lookup_string(&cfg, key, (const char **)value)) {				\
    EINA_LOG_CRIT("CONFIG: Your configuration file haven't any %s entry.", key);	\
    exit(-1);										\
  }
#define CONFIG_GET_INT(key, value)							\
  if (!config_lookup_int(&cfg, key, value)) {						\
    EINA_LOG_CRIT("CONFIG: Your configuration file haven't any %s entry.", key);	\
    exit(-1);										\
  }
#define CONFIG_GET_LONG(key, value)							\
  if (!config_lookup_int64(&cfg, key, value)) {						\
    EINA_LOG_CRIT("CONFIG: Your configuration file haven't any %s entry.", key);	\
    exit(-1);										\
  }
#define CONFIG_GET_FLOAT(key, value)							\
  if (!config_lookup_float(&cfg, key, value)) {						\
    EINA_LOG_CRIT("CONFIG: Your configuration file haven't any %s entry.", key);	\
    exit(-1);										\
  }
#define CONFIG_GET_BOOL(key, value)							\
  if (!config_lookup_bool(&cfg, key, value)) {						\
    EINA_LOG_CRIT("CONFIG: Your configuration file haven't any %s entry.", key);	\
    exit(-1);										\
  }

#define CONFIG_FOREACH(key, settings, iter) {						\
  settings = config_lookup(&cfg, key);							\
  for (iter = 0; iter < config_setting_length(settings); ++iter)

int tristram_init(const char *configfile);
void tristram_shutdown(void);

#endif
