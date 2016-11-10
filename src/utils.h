#ifndef UTILS_H_
#define UTILS_H_

char *utils_string_to_lower(const char *str);
void utils_hex_to_str(char *in, char *out);
int utils_in_array(uint32_t n, uint32_t *arr, unsigned int size);
#endif
