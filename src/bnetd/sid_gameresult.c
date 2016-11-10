#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2C
 * Packet Name: sid_gameresult
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Game type
 * (DWORD) Number of results - always 8
 * (DWORD) [8] Results
 * (STRING) [8] Game players - always 8
 * (STRING) Map name
 * (STRING) Player score
 * 
 * Remarks : 
 * Contains end-of-game statistics. Player Score is a string containing
 * right-aligned lines separated by 0x0A. The positions in the 'Results' array and
 * the 'Players' array are equivalent.
 * <dl><dt>Possible values for Game type:</dt>
 * <dd>0x00: Normal</dd>
 * <dd>0x01: Ladder</dd>
 * <dd>0x03: Ironman (W2BN only)</dd>
 * <dt>Possible values for Result:</dt>
 * <dd>0x00: No player</dd>
 * <dd>0x01: Win</dd>
 * <dd>0x02: Loss</dd>
 * <dd>0x03: Draw</dd>
 * <dd>0x04: Disconnect</dd></dl>
 *
 */
struct request {
  uint32_t game_type;
  uint32_t number_of_results___always;
  uint32_t results;
  char * game_players___always;
  char * map_name;
  char * player_score;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2C, packet_bnetd_sid_gameresult);
 *
 */
void packet_bnetd_sid_gameresult(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.game_type = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_results___always = packet_get_int(request, ENDIAN_LITTLE);
  packet.results = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_players___always = packet_get_string(request, ENDIAN_LITTLE);
  packet.map_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.player_score = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_gameresult not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
