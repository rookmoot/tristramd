#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <Eina.h>

#include "utils.h"


char *utils_string_to_lower(const char *str) {
  char *tmp = (char *)str;
  for ( ; *tmp; ++tmp) {
    *tmp = tolower(*tmp);
  }

  return (char *)str;
}


void utils_hex_to_str(char *in, char *out) {
  while (*in) {
    if ((unsigned char)(*in - 'a') <= 'f' - 'a')
      *out = (*in - 'a' + 10) << 4;
    else if ((unsigned char)(*in - '0') <= '9' - '0')
      *out = (*in - '0') << 4;
    else
      return;
    in++;
    if ((unsigned char)(*in - 'a') <= 'f' - 'a')
      *out |= (*in - 'a' + 10);
    else if ((unsigned char)(*in - '0') <= '9' - '0')
      *out |= (*in - '0');
    else
      return;
    in++;
    out++;
  }
}
