#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x57
 * Packet Name: sid_auth_accountupgrade
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * This message is sent to upgrade an old account to an NLS-style account. It
 * should be sent when SID_AUTH_ACCOUNTLOGON or SID_AUTH_ACCOUNTCHANGE indicates
 * that an account upgrade is required.
 *
 */


/*
 * Packet ID: 0x57
 * Packet Name: sid_auth_accountupgrade
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (DWORD) Server Token
 * 
 * 
 * Remarks : 
 * Status Codes:<dl><dd>0x00: Upgrade request accepted.</dd>
 * <dd>Other: Upgrade request denied.</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x57, packet_bnetd_sid_auth_accountupgrade);
 *
 */
void packet_bnetd_sid_auth_accountupgrade(struct connection *conn, struct packet *request EINA_UNUSED) {
  struct packet *response = NULL;

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x57);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountupgrade not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
