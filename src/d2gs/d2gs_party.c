#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x5E
 * Packet Name: d2gs_party
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD)  Action ID
 * (DWORD) Player ID
 * 
 * 
 * Remarks : 
 * Possible Action IDs:
 * 
 * 0x06 - Invite player to party with you
 * 0x07 - Cancel invite to player
 * 0x08 - Accept invite from player
 * 0x09 - Leave party
 *
 */
struct request {
  uint16_t action_id;
  uint32_t player_id;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x5E, packet_d2gs_d2gs_party);
 *
 */
void packet_d2gs_d2gs_party(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.action_id = packet_get_short(request, ENDIAN_LITTLE);
  packet.player_id = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x5E);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_party not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
