#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "character.h"
#include "account.h"
#include "game.h"

/*
 * Packet ID: 0x68
 * Packet Name: d2gs_gamelogon
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) D2GS Server Hash
 * (WORD) D2GS Server Token
 * (BYTE) Character ID
 * (DWORD) Version byte (Currently 0x0B)
 * (DWORD) Unknown - Suggested Const (0xED5DCC50)
 * (DWORD) Unknown - Suggested Const (0x91A519B6)
 * (BYTE) Unknown - Suggested (0x00)
 * (STRING) Character name
 * (VOID) *See user-comment below
 * 
 * 
 * Remarks : 
 * Possible Character IDs:
 * 0x00 - Amazon
 * 0x01 - Sorceress
 * 0x02 - Necromancer
 * 0x03 - Paladin
 * 0x04 - Barbarian
 * 0x05 - Druid
 * 0x06 - Assassin
 * 
 * The "Character name" field is a buffer of 15 characters with a null-terminator.
 * This string should always be filled with 15 characters (plus terminator), padded
 * with 0's for unused bytes.
 * 
 * Note: If the character ID does not match the type of character you're attempting
 * to logon with, and the character is nealy created (with 0 play time), then that
 * character will automatically be changed or "morphed" into the character ID sent.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t uuid;
  uint16_t dgs_server_token;
  uint8_t character_id;
  uint32_t version_byte;
  uint32_t unknown1;
  uint32_t unknown2;
  uint8_t unknown3;
  char * character_name;
  void * data;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x68, packet_d2gs_d2gs_gamelogon);
 *
 */
void packet_d2gs_d2gs_gamelogon(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct game *game = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.uuid = packet_get_int(request, ENDIAN_LITTLE);
  packet.dgs_server_token = packet_get_short(request, ENDIAN_LITTLE);
  packet.character_id = packet_get_byte(request, ENDIAN_LITTLE);
  packet.version_byte = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown1 = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown2 = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown3 = packet_get_byte(request, ENDIAN_LITTLE);
  packet.character_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.data = (char *)packet_get_string(request, ENDIAN_LITTLE);

  /*
   * Game server acceptance :
   *  - Clone the current user to the old
   *    already connected user.
   *  - Find correct game by token.
   *  - Verify character is the same a cloned one.
   *  - Assign game to user.
   */

  conn = net_connection_clone(conn, packet.uuid);
  if (!conn) {
    EINA_LOG_CRIT("Failed to clone net connection from realm to d2gs.");
    EINA_LOG_CRIT("This is a pretty bad exception. Please fix this.");
    return;
  }
  
  // create the response packet.
  game = game_exists_by_token(packet.dgs_server_token);
  if (!game) {
    EINA_LOG_CRIT("Failed to get game from token %d.", packet.dgs_server_token);
    return;
  }

  if (strncmp((char *)conn->account->character->name, packet.character_name, strlen(packet.character_name)) != 0) {
    EINA_LOG_CRIT("Attempting to log a bad character (%s != %s)", conn->account->character->name, packet.character_name);
    return;
  }

  if (!account_game_set(conn->account, game)) {
    EINA_LOG_CRIT("Failed to assign game to account.");
    return;
  }
  
  free(packet.character_name);
  free(packet.data);

  response = packet_new(PROTOCOL_D2GS_COMPRESSED);
  if (!response) {
    return;
  }
  packet_message_id_set(response, 0x7A);
  packet_set_byte(response, ENDIAN_LITTLE, 0x09);
  packet_set_byte(response, ENDIAN_LITTLE, 0x2e);
  packet_set_byte(response, ENDIAN_LITTLE, 0xef);
  net_connection_response_append(conn, response);

  response = packet_new(PROTOCOL_D2GS_COMPRESSED);
  if (!response) {
    return;
  }
  packet_message_id_set(response, 0x5C);
  net_connection_response_append(conn, response);
}
