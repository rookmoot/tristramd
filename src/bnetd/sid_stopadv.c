#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x02
 * Packet Name: sid_stopadv
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * This message is sent to inform the server that a game should no longer be
 * advertised to other users. It is sent when a game starts, or when a game is
 * aborted (the host leaves).
 * 
 * All Battle.snp clients (DRTL, DSHR, STAR/SEXP, JSTR, SSHR, and W2BN) always send
 * this message when logging off, even if it not in a game.
 *
 */

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x02, packet_bnetd_sid_stopadv);
 *
 */
void packet_bnetd_sid_stopadv(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x02);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_stopadv not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
