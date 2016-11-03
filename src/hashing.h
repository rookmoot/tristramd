#ifndef HASHING_H_
#define HASHING_H_

void hash_new(const char *str, uint32_t *hashout);
const char *hash_to_str(uint32_t *hash);

int hashing_check_password(uint32_t *real_password, uint32_t *claimed, uint32_t ctoken, uint32_t stoken);


#endif
