#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"
#include "character.h"

/*
 * Packet ID: 0x02
 * Packet Name: mcp_charcreate
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Character class
 * (WORD) Character flags
 * (STRING) Character name
 * 
 * Remarks : 
 * Creates a character on the Realm.
 * 
 * Character Classes are the same as in D2 users' Statstrings:
 * <dl><dd>0x00: Amazon
 * 0x01: Sorceress
 * 0x02: Necromancer
 * 0x03: Paladin
 * 0x04: Barbarian
 * 0x05: Druid
 * 0x06: Assassin</dd></dl>
 * Flag values should be OR'd together. The only flags that can be set for
 * character creation are classic, hardcore, expansion, and ladder, but the other
 * flags are included here for completeness:
 * <dl><dd>0x00: Classic
 * 0x04: Hardcore
 * 0x08: Dead
 * 0x20: Expansion
 * 0x40: Ladder</dd>
 * Sending 0x05 or 0x06 in character class or 0x20 in character flags while on D2DV
 * will disconnect and temporarily ban you from the realm. Likewise, sending 0x05
 * or 0x06 in character class without setting 0x20 in character flags will result
 * in a disconnect and ban.
 *
 */
struct request {
  uint32_t class;
  uint16_t status;
  char *name;
} __attribute__((packed));


/*
 * Packet ID: 0x02
 * Packet Name: mcp_charcreate
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * The return value for character creation.
 * <dl>
 * <dt>Known values:</dt>
 * <dd>0x00: Success</dd>
 * <dd>0x14: Character already exists, or maximum number of characters (currently
 * 8) reached.</dd>
 * <dd>0x15: Invalid name</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x02, packet_realm_mcp_charcreate);
 *
 */
void packet_realm_mcp_charcreate(struct connection *conn, struct packet *request) {
  struct request packet;
  struct packet *response = NULL;
  struct character *character = NULL;

  if (!conn || !conn->account) {
    EINA_LOG_CRIT("Client must be logged in.");
    return;
  }
  
  memset(&packet, 0, sizeof(packet));
  packet.class = packet_get_int(request, ENDIAN_LITTLE);
  packet.status = packet_get_short(request, ENDIAN_LITTLE);
  packet.name = packet_get_string(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x02);

  character = character_create(packet.name, (enum character_class)packet.class, (character_status)packet.status);
  if (!character) {
    packet_set_int(response, ENDIAN_LITTLE, 0x14); // error creating char.
  }

  if (character_save(conn->account, character)) {
    packet_set_int(response, ENDIAN_LITTLE, 0x00); // success
  } else {
    packet_set_int(response, ENDIAN_LITTLE, 0x14); // error creating char.
  }
  free(packet.name);
  net_connection_response_append(conn, response);
}
