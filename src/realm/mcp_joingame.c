#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "account.h"
#include "game.h"

/*
 * Packet ID: 0x04
 * Packet Name: mcp_joingame
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request ID
 * (STRING) Game name
 * (STRING) Game Password
 * 
 * Remarks : 
 * This must be sent after a successful game creation.
 *
 */
struct request {
  uint16_t request_id;
  char *game_name;
  char *game_password;
} __attribute__((packed));


/*
 * Packet ID: 0x04
 * Packet Name: mcp_joingame
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request ID
 * (WORD) Game token
 * (WORD) Unknown (0)
 * (DWORD) IP of D2GS Server
 * (DWORD) Game hash
 * (DWORD) Result
 * 
 * Remarks : 
 * <dl>
 * <dt>Possible values for result:</dt>
 * <dd>0x00: Game joining succeeded. In this case, Diablo 2 terminates the
 * connection with the MCP and initiates the connection with the D2GS.</dd>
 * <dd>0x29: Password incorrect.</dd>
 * <dd>0x2A: Game does not exist.</dd>
 * <dd>0x2B: Game is full.</dd>
 * <dd>0x2C: You do not meet the level requirements for this game.</dd>
 * <dd>0x6E: A dead hardcore character cannot join a game.</dd>
 * <dd>0x71: A non-hardcore character cannot join a game created by a Hardcore
 * character.</dd>
 * <dd>0x73: Unable to join a Nightmare game.</dd>
 * <dd>0x74: Unable to join a Hell game.</dd>
 * <dd>0x78: A non-expansion character cannot join a game created by an Expansion
 * character.</dd>
 * <dd>0x79: A Expansion character cannot join a game created by a non-expansion
 * character.</dd>
 * <dd>0x7D: A non-ladder character cannot join a game created by a Ladder
 * character.</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x04, packet_realm_mcp_joingame);
 *
 */
void packet_realm_mcp_joingame(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct game *game = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_short(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x04);

  game = game_exists((const char *)packet.game_name, (const char *)packet.game_password);
  if (game == NULL) {
    packet_set_short(response, ENDIAN_LITTLE, packet.request_id);
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
    packet_set_int(response, ENDIAN_BIG, 0x7f000001);
    packet_set_int(response, ENDIAN_LITTLE, 0x00);
    packet_set_int(response, ENDIAN_LITTLE, 0x2A); // game does not exists
  } else {
    packet_set_short(response, ENDIAN_LITTLE, packet.request_id);
    packet_set_short(response, ENDIAN_LITTLE, game_token_get(game));
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
    packet_set_int(response, ENDIAN_BIG, 0x7f000001);
    packet_set_int(response, ENDIAN_LITTLE, (uint32_t)net_connection_uuid_get(conn));
    packet_set_int(response, ENDIAN_LITTLE, (uint32_t)game_account_join(game, conn->account));
  }
  
  connection_response_append(conn, response);
}
