#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "game.h"

/*
 * Packet ID: 0x03
 * Packet Name: mcp_creategame
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request Id *
 * (DWORD) Difficulty
 * (BYTE) Unknown - 1
 * (BYTE) Player difference **
 * (BYTE) Maximum players
 * (STRING) Game name
 * (STRING) Game password
 * (STRING) Game description
 * 
 * Remarks : 
 * <dl>
 * <dt>Difficulty:</dt>
 * <dd>0x0000: Normal</dd>
 * <dd>0x1000: Nightmare</dd>
 * <dd>0x2000: Hell</dd>
 * </dl>
 * 
 * * This value starts at 0x02 at first game creation, and increments by 0x02 each
 * consecutive game creation.
 * 
 * ** A value of 0xFF indicates that the game is not restricted by character
 * difference.
 * 
 * Before sending the game name and password, Diablo II automatically changes their
 * case. For example if the string "aBc DeF" is typed in Diablo II, then the string
 * sent is "Abc Def". This does not apply to the game description.
 *
 */
struct request {
  uint16_t request_id;
  uint32_t difficulty;
  uint8_t unknown;
  uint8_t player_difference;
  uint8_t maximum_players;
  char * game_name;
  char * game_password;
  char * game_description;
} __attribute__((packed));


/*
 * Packet ID: 0x03
 * Packet Name: mcp_creategame
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request Id
 * (WORD) Game token
 * (WORD) Unknown (0)
 * (DWORD) Result
 * 
 * Remarks : 
 * <dl>
 * <dt>Result:</dt>
 * <dd>0x00: Game creation succeeded. This does NOT automatically join the game -
 * the client must also send packet MCP_JOINGAME.</dd>
 * <dd>0x1E: Invalid game name.</dd>
 * <dd>0x1F: Game already exists.</dd>
 * <dd>0x20: Game servers are down.</dd>
 * <dd>0x6E: A dead hardcore character cannot create games.</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x03, packet_realm_mcp_creategame);
 *
 */
void packet_realm_mcp_creategame(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct game *game = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_short(request, ENDIAN_LITTLE);
  packet.difficulty = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_byte(request, ENDIAN_LITTLE);
  packet.player_difference = packet_get_byte(request, ENDIAN_LITTLE);
  packet.maximum_players = packet_get_byte(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_description = packet_get_string(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x03);

  game = game_create(packet.game_name, packet.game_password, packet.game_description,
		     packet.maximum_players, packet.player_difference, packet.difficulty);
  if (game) {
    packet_set_short(response, ENDIAN_LITTLE, packet.request_id);
    packet_set_short(response, ENDIAN_LITTLE, game_token_get(game));
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
    packet_set_int(response, ENDIAN_LITTLE, 0x00);
  } else {
    packet_set_short(response, ENDIAN_LITTLE, packet.request_id);
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
    packet_set_int(response, ENDIAN_LITTLE, 0x20);
  }
  net_connection_response_append(conn, response);
}
