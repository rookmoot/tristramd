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
 * Packet Name: d2gs_unselectobj
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * blank
 * 
 * 
 * Remarks : 
 * Unselects the selected object. Please note: This message's official name is not
 * known, and has been invented. The contents of this packet may be incomplete or
 * incorrect, and your comments and contributions are requested.
 *
 */

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x2D, packet_d2gs_d2gs_unselectobj);
 *
 */
void packet_d2gs_d2gs_unselectobj(struct connection *conn, struct packet *request EINA_UNUSED) {
  struct packet *response = NULL;



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2D);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_unselectobj not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
