#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2D
 * Packet Name: sid_geticondata
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * 
 * Remarks : 
 * Requests the filename & time of the default icons file for the current game.
 * This message must not be sent after receiving SID_ENTERCHAT or Battle.net will
 * terminate the connection.
 *
 */


/*
 * Packet ID: 0x2D
 * Packet Name: sid_geticondata
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (FILETIME) Filetime
 * (STRING) Filename
 * 
 * Remarks : 
 * Returns filename and filetime of default icons file.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2D, packet_bnetd_sid_geticondata);
 *
 */
void packet_bnetd_sid_geticondata(struct connection *conn, struct packet *request EINA_UNUSED) {
  struct packet *response = NULL;

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2D);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_geticondata not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
