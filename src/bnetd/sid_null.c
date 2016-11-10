#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x00
 * Packet Name: sid_null
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Keeps the connection alive. This message should be sent to the server every 8
 * minutes (approximately).
 * 
 * The server will send this to you automatically, you do not have to reply to it.
 * You should send this on your own never-ending timer for at least as often as
 * Battle.net does (give or take a few seconds).
 * 
 * This packet is used to detect if your TCP connection has gone dead, to the point
 * where you will never receive data from the server ever again until you reconnect
 * your connection. A situation such as this can be created by unplugging your
 * internet connection for a few minutes, or if your internet is dropped for
 * whatever reason.
 *
 */


/*
 * Packet ID: 0x00
 * Packet Name: sid_null
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Keeps the connection alive. Clients do not need to respond to this message.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x00, packet_bnetd_sid_null);
 *
 */
void packet_bnetd_sid_null(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x00);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_null not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
