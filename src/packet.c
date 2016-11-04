#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <Eina.h>
#include "net.h"
#include "packet.h"
#include "file.h"
#include "protocol.h"

#define HEXDUMP_COLS 16

static inline int _packet_read_header(struct packet *packet);
static inline int _packet_write_header(struct packet *packet);
static inline void packet_protocol_init(struct packet *packet);

struct packet *packet_new(enum protocol protocol) {
  struct packet *packet = NULL;

  packet = malloc(sizeof(*packet));
  if (!packet) {
    EINA_LOG_CRIT("Failed to allocate packet memory.");
    return NULL;
  }
  memset(packet, 0, sizeof(*packet));
  packet->protocol = protocol;
  packet_protocol_init(packet);
  
  return packet;
}

void packet_free(struct packet *packet) {
  if (packet) {
    free(packet);
  }
}

static inline void packet_protocol_init(struct packet *packet) {
  switch (packet->protocol) {
    case PROTOCOL_BNETD:
      packet->rdx = 0;
      packet->wdx = 4;
      break;
    case PROTOCOL_BNFTP:
      packet->rdx = 0;
      packet->wdx = 2;
      packet->header.opcode = 0x01;
      break;
    case PROTOCOL_REALM:
      packet->rdx = 0;
      packet->wdx = 3;
      break;
    case PROTOCOL_D2GS:
      packet->rdx = 0;
      packet->wdx = 1;
      break;
    case PROTOCOL_D2GS_COMPRESSED:
      packet->rdx = 0;
      packet->wdx = 2;
      break;
    default:
      break;
  }
}

void packet_message_id_set(struct packet *packet, uint8_t opcode) {
  packet->header.opcode = opcode;
}

uint8_t packet_message_id_get(struct packet *packet) {
  return packet->header.opcode;
}

void packet_dump(struct packet *packet, struct connection *conn, const char *prepend) {
  unsigned int i, j;
  char tmp[PACKET_DATA_SIZE];
  struct protocol_handler handler;
  
  if (packet->size <= 0) {
    return;
  }
  
  EINA_LOG_DBG(" %s client: %d, protocol: %X, opcode: %X, length: %X", prepend, conn->socket, packet->protocol, packet->header.opcode, packet->header.length);
  if (protocol_handler_find(packet, &handler)) {
    EINA_LOG_DBG(" %s handler: %s", prepend, handler.name);
  }
  
  memset(tmp, 0, PACKET_DATA_SIZE);
  for (i = 0; i < packet->size + ((packet->size % HEXDUMP_COLS) ? (HEXDUMP_COLS - packet->size % HEXDUMP_COLS) : 0); i++) {
    if (i % HEXDUMP_COLS == 0) {
      sprintf(tmp, "%s0x%06x: ", tmp, i);
    }
    
    if (i < packet->size) {
      sprintf(tmp, "%s%02x ", tmp, 0xFF & ((char*)packet->data)[i]);
    } else {
      sprintf(tmp, "%s   ", tmp);
    }
    
    if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1)) {
      for (j = i - (HEXDUMP_COLS - 1); j <= i; j++) {
	if (j >= packet->size) {
	  sprintf(tmp, "%s ", tmp);
	  } else if (isprint(((char *)packet->data)[j])) {
	  sprintf(tmp, "%s%c", tmp, ((char *)packet->data)[j]);
	} else {
	  sprintf(tmp, "%s.", tmp);
	}
      }

      EINA_LOG_DBG("%s %s", prepend, tmp);
      memset(tmp, 0, PACKET_DATA_SIZE);
    }
  }
}

int packet_read_from_connection(struct packet *packet, struct connection *conn) {
  memset(packet->data, 0, PACKET_DATA_SIZE);
  packet->size = (int)recv(conn->socket, packet->data, PACKET_DATA_SIZE, 0);

  if (packet->size == 0) {
    return -1;
  }

  if ((packet->protocol == PROTOCOL_NONE && packet->size == 1) ||
      (packet->protocol == PROTOCOL_NONE && conn->state < CONNECTION_STATE_DIAL)) {
    EINA_LOG_DBG("Found new protocol %d for client %p", packet->data[0], conn);
    packet->protocol = (uint8_t)packet->data[0];
    conn->protocol = (uint8_t)packet->protocol;

    if (packet->size > 1) {
      size_t i = 0;
      for (i = 1; i < packet->size; i++) {
	packet->data[i - 1] = packet->data[i];
      }
      packet->size--;
    }
    packet_protocol_init(packet);
    conn->state = CONNECTION_STATE_DIAL;
    
    if (packet->size == 1) {
      return 0;
    }
  }

  if (!_packet_read_header(packet)) {
    EINA_LOG_ERR("Failed to read packet header.");
    return 0;
  }

  packet_dump(packet, conn, "RECV: ");
  
  return 1;
}

int packet_write_to_connection(struct packet *packet, struct connection *conn) {
  size_t len = 0;

  if (!_packet_write_header(packet)) {
    EINA_LOG_ERR("Failed to write packet header.");
    return 0;
  }

  packet_dump(packet, conn, "SENT: ");

  len = send(conn->socket, packet->data, packet->size, 0);
  if (len < packet->size) {
    EINA_LOG_ERR("Packet lenght sent to connection is less longer than packet bytes.");
    return 0;
  }
  return 1;
}

static inline int _packet_read_header(struct packet *packet) {
  switch (packet->protocol) {
    case PROTOCOL_BNETD:
      packet_get_byte(packet, ENDIAN_LITTLE);
      packet->header.opcode = packet_get_byte(packet, ENDIAN_LITTLE);
      packet->header.length = packet_get_short(packet, ENDIAN_LITTLE);
      packet->wdx = 4;
      break;

    case PROTOCOL_BNFTP:
      packet->header.opcode = 0x01;
      packet->header.length = packet_get_short(packet, ENDIAN_LITTLE);
      packet->wdx = 2;
      break;

    case PROTOCOL_REALM:
      if (packet->data[0] == 0x01) {
	packet_get_byte(packet, ENDIAN_LITTLE);;
      }
      packet->header.length = packet_get_short(packet, ENDIAN_LITTLE);
      packet->header.opcode = packet_get_byte(packet, ENDIAN_LITTLE);
      packet->wdx = 3;
      break;

    case PROTOCOL_D2GS:
      packet->header.opcode = packet_get_byte(packet, ENDIAN_LITTLE);
      packet->wdx = 1;
      break;

    case PROTOCOL_D2GS_COMPRESSED:
      packet->header.length = packet_get_byte(packet, ENDIAN_LITTLE);
      packet->header.opcode = packet_get_byte(packet, ENDIAN_LITTLE);
      packet->wdx = 2;
      break;
      
    default:
      EINA_LOG_CRIT("Packet protocol %d header reader not yet implemented", packet->protocol);
      return 0;
  }
  
  return 1;
}

static inline int _packet_write_header(struct packet *packet) { 
  uint32_t length = 0;
  
  switch (packet->protocol) {
    case PROTOCOL_BNETD:
      length = packet->wdx;
      packet->wdx = 0;
      packet_set_byte(packet, ENDIAN_LITTLE, 0xFF);
      packet_set_byte(packet, ENDIAN_LITTLE, packet->header.opcode);
      packet_set_short(packet, ENDIAN_LITTLE, (uint16_t)length);
      packet->wdx = packet->size > length ? packet->size : length;
      packet->header.length = packet->wdx;
      packet->size = packet->wdx;
      break;

    case PROTOCOL_BNFTP:
      length = packet->wdx - packet->bnftp_data_length;
      packet->wdx = 0;
      packet_set_short(packet, ENDIAN_LITTLE, (uint16_t)length);
      packet->wdx = length + packet->bnftp_data_length;
      packet->header.length = packet->wdx;
      packet->size = packet->wdx;
      break;

    case PROTOCOL_REALM:
      length = packet->wdx;
      packet->wdx = 0;
      packet_set_short(packet, ENDIAN_LITTLE, (uint16_t)length);
      packet_set_byte(packet, ENDIAN_LITTLE, packet->header.opcode);
      packet->wdx = packet->size > length ? packet->size : length;
      packet->header.length = packet->wdx;
      packet->size = packet->wdx;
      break;

    case PROTOCOL_D2GS:
      length = packet->wdx;
      packet->wdx = 0;
      packet_set_byte(packet, ENDIAN_LITTLE, packet->header.opcode);
      packet->wdx = packet->size > length ? packet->size : length;
      packet->header.length = packet->wdx;
      packet->size = packet->wdx;
      break;

    case PROTOCOL_D2GS_COMPRESSED:
      length = packet->wdx;
      packet->wdx = 0;
      packet_set_byte(packet, ENDIAN_LITTLE, (uint8_t)length);
      packet_set_byte(packet, ENDIAN_LITTLE, packet->header.opcode);
      packet->wdx = packet->size > length ? packet->size : length;
      packet->header.length = packet->wdx;
      packet->size = packet->wdx;
      break;

    default:
      EINA_LOG_CRIT("Packet protocol %d header reader not yet implemented", packet->protocol);
      return 0;
  }
  return 1;
}


uint8_t packet_get_byte(struct packet *packet, enum packet_endianness endian) {
  uint8_t value = 0;

  if ((packet->rdx + 1) >= packet->size && packet->protocol < PROTOCOL_D2GS) {
    return 0;
  }
  endian = endian;

  value = packet->data[packet->rdx++];
  return value;
}

uint16_t packet_get_short(struct packet *packet, enum packet_endianness endian) {
  uint16_t value = 0;
  
  if ((packet->rdx + 2) >= packet->size) {
    return 0;
  }

  if (endian == ENDIAN_LITTLE) {
    value  = ((uint16_t)packet->data[packet->rdx + 0]);
    value |= ((uint16_t)packet->data[packet->rdx + 1]) << 8;
  } else {
    value  = ((uint16_t)packet->data[packet->rdx + 1]);
    value |= ((uint16_t)packet->data[packet->rdx + 0]) << 8;
  }
  packet->rdx += 2;

  return value;
}

uint32_t packet_get_int(struct packet *packet, enum packet_endianness endian) {
  uint32_t value = 0;
  
  if ((packet->rdx + 4) >= packet->size) {
    return 0;
  }

  if (endian == ENDIAN_LITTLE) {
    value  = ((uint32_t)packet->data[packet->rdx + 0]);
    value |= ((uint32_t)packet->data[packet->rdx + 1]) << 8;
    value |= ((uint32_t)packet->data[packet->rdx + 2]) << 16;
    value |= ((uint32_t)packet->data[packet->rdx + 3]) << 24;
  } else {
    value  = ((uint32_t)packet->data[packet->rdx + 3]);
    value |= ((uint32_t)packet->data[packet->rdx + 2]) << 8;
    value |= ((uint32_t)packet->data[packet->rdx + 1]) << 16;
    value |= ((uint32_t)packet->data[packet->rdx + 0]) << 24;
  }
  packet->rdx += 4;

  return value;
}

uint64_t packet_get_long(struct packet *packet, enum packet_endianness endian) {
  uint64_t value = 0;

  if ((packet->rdx + 8) >= packet->size) {
    return 0;
  }

  if (endian == ENDIAN_LITTLE) {
    value  = ((uint64_t)packet->data[packet->rdx + 0]);
    value |= ((uint64_t)packet->data[packet->rdx + 1]) << 8;
    value |= ((uint64_t)packet->data[packet->rdx + 2]) << 16;
    value |= ((uint64_t)packet->data[packet->rdx + 3]) << 24;
    value |= ((uint64_t)packet->data[packet->rdx + 4]) << 32;
    value |= ((uint64_t)packet->data[packet->rdx + 5]) << 40;
    value |= ((uint64_t)packet->data[packet->rdx + 6]) << 48;
    value |= ((uint64_t)packet->data[packet->rdx + 7]) << 56;
  } else {
    value  = ((uint64_t)packet->data[packet->rdx + 7]);
    value |= ((uint64_t)packet->data[packet->rdx + 6]) << 8;
    value |= ((uint64_t)packet->data[packet->rdx + 5]) << 16;
    value |= ((uint64_t)packet->data[packet->rdx + 4]) << 24;
    value |= ((uint64_t)packet->data[packet->rdx + 3]) << 32;
    value |= ((uint64_t)packet->data[packet->rdx + 2]) << 40;
    value |= ((uint64_t)packet->data[packet->rdx + 1]) << 48;
    value |= ((uint64_t)packet->data[packet->rdx + 0]) << 56;
  }
  packet->rdx += 8;
  
  return value;
}

char *packet_get_string(struct packet *packet, enum packet_endianness endian) {
  int i = 0;
  char buffer[PACKET_DATA_SIZE];

  endian = endian;
  
  memset(buffer, 0, PACKET_DATA_SIZE);
  for (i = 0; packet->data[packet->rdx + i] != '\0'; i++) {
    buffer[i] = packet->data[packet->rdx + i];
  }
  packet->rdx += (i + 1);
  return (char *)strdup((const char *)buffer);
}

void packet_set_byte(struct packet *packet, enum packet_endianness endian, uint8_t value) {
  endian = endian;

  packet->data[packet->wdx++] = value;
  packet->size = packet->wdx;
}

void packet_set_short(struct packet *packet, enum packet_endianness endian, uint16_t value) {
  if (endian == ENDIAN_LITTLE) {
    packet->data[packet->wdx + 0] = ((value) & 0xff);
    packet->data[packet->wdx + 1] = ((value >> 8));
  } else {
    packet->data[packet->wdx + 0] = ((value >> 8));
    packet->data[packet->wdx + 1] = ((value)& 0xff);
  }
  packet->wdx += 2;
  packet->size = packet->wdx;
}

void packet_set_int(struct packet *packet, enum packet_endianness endian, uint32_t value) {
  if (endian == ENDIAN_LITTLE) {
    packet->data[packet->wdx + 0] = ((value) & 0xff);
    packet->data[packet->wdx + 1] = ((value >> 8) & 0xff);
    packet->data[packet->wdx + 2] = ((value >> 16) & 0xff);
    packet->data[packet->wdx + 3] = ((value >> 24));
  } else {
    packet->data[packet->wdx + 0] = ((value >> 24) & 0xff);
    packet->data[packet->wdx + 1] = ((value >> 16) & 0xff);
    packet->data[packet->wdx + 2] = ((value >> 8) & 0xff);
    packet->data[packet->wdx + 3] = ((value)& 0xff);
  }
  packet->wdx += 4;
  packet->size = packet->wdx;
}

void packet_set_long(struct packet *packet, enum packet_endianness endian, uint64_t value) {
  if (endian == ENDIAN_LITTLE) {
    packet->data[packet->wdx + 0] = ((value) & 0xff);
    packet->data[packet->wdx + 1] = ((value >> 8) & 0xff);
    packet->data[packet->wdx + 2] = ((value >> 16) & 0xff);
    packet->data[packet->wdx + 3] = ((value >> 24) & 0xff);
    packet->data[packet->wdx + 4] = ((value >> 32) & 0xff);
    packet->data[packet->wdx + 5] = ((value >> 40) & 0xff);
    packet->data[packet->wdx + 6] = ((value >> 48) & 0xff);
    packet->data[packet->wdx + 7] = ((value >> 56));
  } else {
    packet->data[packet->wdx + 0] = ((value >> 56));
    packet->data[packet->wdx + 1] = ((value >> 48) & 0xff);
    packet->data[packet->wdx + 2] = ((value >> 40) & 0xff);
    packet->data[packet->wdx + 3] = ((value >> 32) & 0xff);
    packet->data[packet->wdx + 4] = ((value >> 24) & 0xff);
    packet->data[packet->wdx + 5] = ((value >> 16) & 0xff);
    packet->data[packet->wdx + 6] = ((value >> 8) & 0xff);
    packet->data[packet->wdx + 7] = ((value)& 0xff);
  }
  packet->wdx += 8;
  packet->size = packet->wdx;
}

void packet_set_string(struct packet *packet, enum packet_endianness endian, const char *value) {
  unsigned int i = 0;

  endian = endian;
  for (i = 0; i < strlen(value); i++) {
    packet->data[packet->wdx + i] = value[i];
  }
  packet->wdx += (i + 1);
  packet->size = packet->wdx;
}

void packet_set_bnftp_file(struct packet *packet, struct file *file) {
  uint8_t *data = NULL;
  unsigned int i = 0;

  data = (uint8_t *)file_chunk_data_get(file, (size_t *)&(packet->bnftp_data_length));
  for (i = 0; i < (unsigned int)packet->bnftp_data_length; ++i) {
    packet->data[packet->wdx + i] = data[i];
  }
  packet->wdx += i;
  packet->size = packet->wdx;
}
