#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include <sys/types.h>
#include <Eina.h>

#include "utils.h"
#include "hashing.h"

#define ROTL(x,n,w) (((x)<<((n)&(w-1))) | ((x)>>(((-(n))&(w-1)))))
#define ROTL32(x,n) ROTL(x,n,32)

void hash_new(const char *str, uint32_t *password) {
  unsigned int i = 0;

  memset(password, 0, 20);
  for (i = 0; i < 5; i++) {
    if (sscanf(&str[i * 8], "%8x", &(password[i])) != 1) {
      EINA_LOG_CRIT("Got bad string for password hash: %s", str);
    }
  }
}

const char *hash_to_str(uint32_t *hash) {
  char buffer[41];
  
  memset(buffer, 0, 41);
  snprintf(buffer, 41, "%08x%08x%08x%08x%08x", hash[0], hash[1], hash[2], hash[3], hash[4]);

  return (const char *)strdup((char *)buffer);
}

static void hash_init(uint32_t *hash) {  
  hash[0] = 0x67452301;
  hash[1] = 0xefcdab89;
  hash[2] = 0x98badcfe;
  hash[3] = 0x10325476;
  hash[4] = 0xc3d2e1f0;
}

static void do_hash(uint32_t *hash, uint32_t *tmp) {
  unsigned int i;
  uint32_t     a, b, c, d, e, g;
  
  for (i = 0; i < 64; i++) {
    tmp[i + 16] = ROTL32(1, tmp[i] ^ tmp[i + 8] ^ tmp[i + 2] ^ tmp[i + 13]);
  }
  
  a = hash[0];
  b = hash[1];
  c = hash[2];
  d = hash[3];
  e = hash[4];
  
  for (i = 0; i < 20 * 1; i++) {
    g = tmp[i] + ROTL32(a, 5) + e + ((b & c) | (~b & d)) + 0x5a827999;
    e = d;
    d = c;
    c = ROTL32(b, 30);
    b = a;
    a = g;
  }
  
  for (; i < 20 * 2; i++) {
    g = (d ^ c ^ b) + e + ROTL32(g, 5) + tmp[i] + 0x6ed9eba1;
    e = d;
    d = c;
    c = ROTL32(b, 30);
    b = a;
    a = g;
  }
  
  for (; i < 20 * 3; i++) {
    g = tmp[i] + ROTL32(g, 5) + e + ((c & b) | (d & c) | (d & b)) - 0x70e44324;
    e = d;
    d = c;
    c = ROTL32(b, 30);
    b = a;
    a = g;
  }
  
  for (; i < 20 * 4; i++) {
    g = (d ^ c ^ b) + e + ROTL32(g, 5) + tmp[i] - 0x359d3e2a;
    e = d;
    d = c;
    c = ROTL32(b, 30);
    b = a;
    a = g;
  }
  
  hash[0] += g;
  hash[1] += b;
  hash[2] += c;
  hash[3] += d;
  hash[4] += e;
}


static void hash_set_16(uint32_t *dst, unsigned char const *src, unsigned int count) {
  unsigned int i;
  unsigned int pos;
  
  for (pos = 0, i = 0; i < 16; i++) {
    dst[i] = 0;
    
    if (pos < count) {
      dst[i] |= ((uint32_t)src[pos]);
    }
    pos++;
    
    if (pos < count) {
      dst[i] |= ((uint32_t)src[pos]) << 8;
    }
    pos++;
    
    if (pos < count) {
      dst[i] |= ((uint32_t)src[pos]) << 16;
    }
    pos++;
    
    if (pos < count) {
      dst[i] |= ((uint32_t)src[pos]) << 24;
    }
    pos++;
  }
}
  
static int hashing_hash(uint32_t *hashout, unsigned int size, void const *datain) {
  uint32_t tmp[64 + 16];
  const unsigned char* data;
  unsigned int inc;

  hash_init(hashout);
  
  data = (const unsigned char*)datain;
  while (size > 0) {
    if (size > 64)
      inc = 64;
    else
      inc = size;

    hash_set_16(tmp, data, inc);
    do_hash(hashout, tmp);
    
    data += inc;
    size -= inc;
  }
  return 1;
}

static int hashing_eq(uint32_t *h1, uint32_t *h2) {
  unsigned int i;

  for (i = 0; i < 5; i++) {
    if (h1[i] != h2[i]) {
      return 0;
    }
  }
  return 1;
}

int hashing_check_password(uint32_t *real_password, uint32_t *claimed, uint32_t ctoken, uint32_t stoken) {
  uint8_t tmp[28];
  uint32_t result[5];
  
  *(uint32_t *)tmp       = ctoken;
  *(uint32_t *)(tmp + 4) = stoken;
  memcpy(tmp + 8, real_password, 20);
  
  hashing_hash(result, sizeof(tmp), tmp);
  /*
  char buf[41];
  memset(buf, 0, 41);
  snprintf(buf, 41, "%08x%08x%08x%08x%08x", result[0], result[1], result[2], result[3], result[4]);
  printf("RESULT: %s\n", buf);

  memset(buf, 0, 41);
  snprintf(buf, 41, "%08x%08x%08x%08x%08x", real_password[0], real_password[1], real_password[2], real_password[3], real_password[4]);
  printf("REAL: %s\n", buf);

  memset(buf, 0, 41);
  snprintf(buf, 41, "%08x%08x%08x%08x%08x", claimed[0], claimed[1], claimed[2], claimed[3], claimed[4]);
  printf("CLAIMED: %s\n", buf);
  */
  return hashing_eq(claimed, result);
}
