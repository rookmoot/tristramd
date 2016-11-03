#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>
#include "file.h"

#define PACKET_DATA_SIZE 8192

enum packet_endianness {
  ENDIAN_LITTLE = 0,
  ENDIAN_BIG = 1,
};

struct packet {
  enum protocol protocol;

  struct header {
    uint8_t opcode;
    uint16_t length;
  } header;

  uint32_t wdx;
  uint32_t rdx;
  
  size_t size;
  uint8_t data[PACKET_DATA_SIZE];
  uint32_t bnftp_data_length;  
};

struct packet *packet_new(enum protocol protocol);
void packet_free(struct packet *packet);

void packet_message_id_set(struct packet *packet, uint8_t opcode);
uint8_t packet_message_id_get(struct packet *packet);

void packet_dump(struct packet *packet, struct connection *conn, const char *prepend);

int packet_read_from_connection(struct packet *packet, struct connection *conn);
int packet_write_to_connection(struct packet *packet, struct connection *conn);

uint8_t packet_get_byte(struct packet *packet, enum packet_endianness endian);
uint16_t packet_get_short(struct packet *packet, enum packet_endianness endian);
uint32_t packet_get_int(struct packet *packet, enum packet_endianness endian);
uint64_t packet_get_long(struct packet *packet, enum packet_endianness endian);
char *packet_get_string(struct packet *packet, enum packet_endianness endian);

void packet_set_byte(struct packet *packet, enum packet_endianness endian, uint8_t value);
void packet_set_short(struct packet *packet, enum packet_endianness endian, uint16_t value);
void packet_set_int(struct packet *packet, enum packet_endianness endian, uint32_t value);
void packet_set_long(struct packet *packet, enum packet_endianness endian, uint64_t value);
void packet_set_string(struct packet *packet, enum packet_endianness endian, const char *value);

void packet_set_bnftp_file(struct packet *packet, struct file *file);

#endif
