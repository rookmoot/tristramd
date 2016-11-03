#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "account.h"
#include "character.h"

struct ladder_handler {
  uint8_t type;
  char *format;
};

static struct ladder_handler ladder_handlers[] = {
  {0x00, "is_expansion=0 AND is_hardcore=1"},
  {0x01, "is_expansion=0 AND is_hardcore=1 AND class=0"},
  {0x02, "is_expansion=0 AND is_hardcore=1 AND class=1"},
  {0x03, "is_expansion=0 AND is_hardcore=1 AND class=2"},
  {0x04, "is_expansion=0 AND is_hardcore=1 AND class=3"},
  {0x05, "is_expansion=0 AND is_hardcore=1 AND class=4"},
  
  {0x09, "is_expansion=0 AND is_hardcore=0"},
  {0x0A, "is_expansion=0 AND is_hardcore=0 AND class=0"},
  {0x0B, "is_expansion=0 AND is_hardcore=0 AND class=1"},
  {0x0C, "is_expansion=0 AND is_hardcore=0 AND class=2"},
  {0x0D, "is_expansion=0 AND is_hardcore=0 AND class=3"},
  {0x0E, "is_expansion=0 AND is_hardcore=0 AND class=4"},

  {0x13, "is_expansion=1 AND is_hardcore=1"},
  {0x14, "is_expansion=1 AND is_hardcore=1 AND class=0"},
  {0x15, "is_expansion=1 AND is_hardcore=1 AND class=1"},
  {0x16, "is_expansion=1 AND is_hardcore=1 AND class=2"},
  {0x17, "is_expansion=1 AND is_hardcore=1 AND class=3"},
  {0x18, "is_expansion=1 AND is_hardcore=1 AND class=4"},
  {0x19, "is_expansion=1 AND is_hardcore=1 AND class=5"},
  {0x1A, "is_expansion=1 AND is_hardcore=1 AND class=6"},

  {0x1B, "is_expansion=1 AND is_hardcore=0"},
  {0x1C, "is_expansion=1 AND is_hardcore=0 AND class=0"},
  {0x1D, "is_expansion=1 AND is_hardcore=0 AND class=1"},
  {0x1E, "is_expansion=1 AND is_hardcore=0 AND class=2"},
  {0x1F, "is_expansion=1 AND is_hardcore=0 AND class=3"},
  {0x20, "is_expansion=1 AND is_hardcore=0 AND class=4"},
  {0x21, "is_expansion=1 AND is_hardcore=0 AND class=5"},
  {0x22, "is_expansion=1 AND is_hardcore=0 AND class=6"},
};

static inline char *mcp_requestladderdata_get_format(uint8_t type) {
  unsigned int i = 0;
  char *fmt = NULL;
  
  for (i = 0; ladder_handlers[i].format != NULL; ++i) {
    if (ladder_handlers[i].type == type) {
      fmt = ladder_handlers[i].format;
    }
  }
  return fmt;
}

/*
 * Packet ID: 0x11
 * Packet Name: mcp_requestladderdata
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Ladder type
 * (WORD) Starting position
 * 
 * Remarks : 
 * This will request 16 ladder entries, starting at a zero-based location specified
 * in 'Starting position'. For example if this is 0, then ladder entries 1-16 are
 * retrieved. If this is 17, then ladder entries 18-33 are retrieved. Note: The
 * values that Diablo 2 sends for this are always perfectly divisible by 16. This
 * might be a requirement.
 * <dl>
 * <dt>Possible ladder types:</dt>
 * <dd>0x00: standard hardcore overall ladder</dd>
 * <dd>0x01: standard hardcore amazon ladder</dd>
 * <dd>0x02: standard hardcore sorceress ladder</dd>
 * <dd>0x03: standard hardcore necromancer ladder</dd>
 * <dd>0x04: standard hardcore paladin ladder</dd>
 * <dd>0x05: standard hardcore barbarian ladder</dd>

 * <dd>0x09: standard softcore overall ladder</dd>
 * <dd>0x0A: standard softcore amazon ladder</dd>
 * <dd>0x0B: standard softcore sorceress ladder</dd>
 * <dd>0x0C: standard softcore necromancer ladder</dd>
 * <dd>0x0D: standard softcore paladin ladder</dd>
 * <dd>0x0E: standard softcore barbarian ladder</dd>

 * <dd>0x13: expansion hardcore overall ladder</dd>
 * <dd>0x14: expansion hardcore amazon ladder</dd>
 * <dd>0x15: expansion hardcore sorceress ladder</dd>
 * <dd>0x16: expansion hardcore necromancer ladder</dd>
 * <dd>0x17: expansion hardcore paladin ladder</dd>
 * <dd>0x18: expansion hardcore barbarian ladder</dd>
 * <dd>0x19: expansion hardcore druid ladder</dd>
 * <dd>0x1A: expansion hardcore assassin ladder</dd>

 * <dd>0x1B: expansion softcore overall ladder</dd>
 * <dd>0x1C: expansion softcore amazon ladder</dd>
 * <dd>0x1D: expansion softcore sorceress ladder</dd>
 * <dd>0x1E: expansion softcore necromancer ladder</dd>
 * <dd>0x1F: expansion softcore paladin ladder</dd>
 * <dd>0x20: expansion softcore barbarian ladder</dd>
 * <dd>0x21: expansion softcore druid ladder</dd>
 * <dd>0x22: expansion softcore assassin ladder</dd>
 * </dl>
 *
 */
struct request {
  uint8_t ladder_type;
  uint16_t starting_position;
} __attribute__((packed));


/*
 * Packet ID: 0x11
 * Packet Name: mcp_requestladderdata
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Ladder type
 * <dl>
 * <dt>The 10-byte header:</dt>
 * <dd>(WORD) Total response size</dd>
 * <dd>(WORD) Current message size</dd>
 * <dd>(WORD) Total size of unreceived messages</dd>
 * <dd>(WORD) Rank of first entry</dd>
 * <dd>(WORD) Unknown (0)</dd>
 * <dt>Message data:</dt>
 * <dd>(DWORD)Number of entries</dd>
 * <dd>(DWORD)Unknown (0x10)</dd>
 * <dt>For each entry:</dt>
 * <dd>(QWORD) Character experience</dd>
 * <dd>(BYTE) Character Flags</dd>
 * <dd>(BYTE) Character title</dd>
 * <dd>(WORD) Character level</dd>
 * <dd>(BYTE) [16] Character name</dd>
 * 
 * Remarks : 
 * <dl>
 * <dt>Total Response Size:</dt>
 * <dd>The size of the entire batch of SID_REQUESTLADDERDATA messages, excluding
 * their headers and first bytes.</dd>
 * <dt>Current Message Size:</dt>
 * <dd>The size of the current message, excluding its header and first byte.</dd>
 * <dt>Total size of unreceived messages:</dt>
 * <dd>The total size of all the unreceived messages in the batch, excluding their
 * headers and first bytes. In the last packet, this value is 0, since there are no
 * unreceived messages.</dd>
 * <dt>Rank of first entry:</dt>
 * <dd>Always zero, except in the last message. In the last message, this specifies
 * the zero-based rank of the first entry. (For example if this is 17 in the last
 * packet, then ladder entries 18-33 were retrieved.)</dd>
 * <dt>Character Flags (AND 0x07 to get character):</dt>
 * <dd>0x00: Amazon</dd>
 * <dd>0x01: Sorceress</dd>
 * <dd>0x02: Necromancer</dd>
 * <dd>0x03: Paladin</dd>
 * <dd>0x04: Barbarian</dd>
 * <dd>0x05: Druid</dd>
 * <dd>0x06: Assassin</dd>
 * <dd>
 * <dl>
 * <dd>0x08: highlight this (if response to <a
 * href='http://bnetdocs.org/?op=packet&pid=481' target='_blank'>C > S [0x16]
 * MCP_CHARRANK</a></dd>
 * <dd>0x10: dead character (hardcore only)</dd>
 * <dd>0x20: hardcore character</dd>
 * <dd>0x40: expansion character</dd>
 * </dl>
 * </dd>
 * <dt>Character Name:</dt>
 * <dd>The character name is always 16 bytes. If the name is shorter than 16 bytes,
 * the string is padded with nulls. The last byte is always null, since character
 * names are limited to 15 chars.</dd>
 * <dt>General Remarks:</dt>
 * <dd>The server may respond to this packet with one or more of these messages.
 * The client must not handle the data until the last packet in the batch is
 * received.</dd>
 * <dd>The messages' data should be concatenated backwards. For example, if 3
 * packets were received, then the data buffer should contain the data of the 3rd
 * packet, followed by the data of the 2nd packet, followed by the data of the 1st
 * packet. Only after the last packet was received, the data buffer should be
 * parsed.</dd>
 * <dd><b>Important note:</b> If the entry is the last entry in the packet, the
 * character name might be smaller than 16 bytes.In this case, the client MUST add
 * null bytes to the end of the packet, BEFORE adding it to the data buffer.</dd>
 * <dd>Ladder packets are NOT SENT IN ANY ORDER - They are often sent completely
 * out of order and must be placed back into the proper order. You have to infer
 * the sequencing based on the 'how big' fields in the 10 byte header.</dd>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x11, packet_realm_mcp_requestladderdata);
 *
 */
void packet_realm_mcp_requestladderdata(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  char buffer[40000];
  char *fmt = NULL;
  Eina_List *acs = NULL, *l = NULL;
  struct accounts_characters *ac = NULL;
  unsigned int i = 0, total = 0, length = 0;
  uint8_t character_title = 0;

  memset(buffer, 0, 40000);
  
  memset(&packet, 0, sizeof(packet));
  packet.ladder_type = packet_get_byte(request, ENDIAN_LITTLE);
  packet.starting_position = packet_get_short(request, ENDIAN_LITTLE);

  fmt = mcp_requestladderdata_get_format(packet.ladder_type);
  acs = sql_select("accounts_characters", "%s ORDER BY level DESC LIMIT 1000", fmt);
  
  EINA_LIST_FOREACH(acs, l, ac) {
    printf("FOUND: %s %ld\n", ac->name, ac->experience);
    memcpy(buffer + length, (uint64_t *)&(ac->experience), 8);
    memcpy(buffer + length + 8, (uint8_t *)&(ac->class), 1);
    // TODO: need to be changed in the future.
    memcpy(buffer + length + 9, &(character_title), 1);
    memcpy(buffer + length + 10, (uint16_t *)&(ac->level), 2);
    memcpy(buffer + length + 12, ac->name, strlen(ac->name));
    for (i = 0; i < (16 - strlen(ac->name)); i++) {
      buffer[length + 12 + strlen(ac->name) + i] = '\0';
    }
    length += 28;
    total++;
  }

  
  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x11);

  packet_set_byte(response,  ENDIAN_LITTLE, packet.ladder_type);
  packet_set_short(response, ENDIAN_LITTLE, (uint16_t)(length + 10 + 8));
  packet_set_short(response, ENDIAN_LITTLE, (uint16_t)(length + 10 + 8));
  packet_set_short(response, ENDIAN_LITTLE, 0);
  packet_set_short(response, ENDIAN_LITTLE, 0);
  packet_set_short(response, ENDIAN_LITTLE, 0);

  packet_set_int(response, ENDIAN_LITTLE, total);
  packet_set_int(response, ENDIAN_LITTLE, 0x10);
  
  for (i = 0; i < length; i++) {
    packet_set_byte(response, ENDIAN_LITTLE, buffer[i]);
  }

  if (total > 0) {
    connection_response_append(conn, response);
  }
}
