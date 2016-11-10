#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x60
 * Packet Name: sid_gameplayersearch
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * This message requests a list of players for an arranged team game.
 *
 */


/*
 * Packet ID: 0x60
 * Packet Name: sid_gameplayersearch
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Number of players
 * (STRING) [] Player names
 * 
 * 
 * Remarks : 
 * Returns a list of available players for an arranged team ladder game. Players
 * who are considered available: have no away message on, are mutual friends in the
 * chat environment, or non-friend listed users in the same channel as you.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x60, packet_bnetd_sid_gameplayersearch);
 *
 */
void packet_bnetd_sid_gameplayersearch(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x60);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_gameplayersearch not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
