#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x06
 * Packet Name: mcp_gameinfo
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request ID
 * (STRING) Game name
 * 
 * Remarks : 
 * Requests information about a game.
 *
 */
struct request {
  uint16_t request_id;
  char * game_name;
} __attribute__((packed));


/*
 * Packet ID: 0x06
 * Packet Name: mcp_gameinfo
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request ID
 * (DWORD) Status *
 * (DWORD) Game Uptime (seconds)
 * (WORD) Unknown
 * (BYTE) Maximum players allowed
 * (BYTE) Number of characters in the game
 * (BYTE) [16] Classes of ingame characters **
 * (BYTE) [16] Levels of ingame characters **
 * (BYTE) Unused (0)
 * (STRING) [16] Character names **
 * 
 * Remarks : 
 * * Usually 0x00300004, but rarely 0. If it is 0, the packet contains no useful
 * information and the server is probably down.
 * 
 * ** Internally, there are 16 character slots, but the last 8 are always empty.
 * This value sometimes includes some empty character slots.
 * Then, some empty strings are added to the end of the packet.
 * To determine the number of characters really in the game:
 * 
 * CharsInGameReal = CharsInGameFake - AmountOfEmptyCharNames;
 * 
 * Byte N here refers to character in slot N, or 0 if the slot is empty.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x06, packet_realm_mcp_gameinfo);
 *
 */
void packet_realm_mcp_gameinfo(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_short(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x06);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_realm_mcp_gameinfo not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
