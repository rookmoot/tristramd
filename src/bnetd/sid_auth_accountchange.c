#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x55
 * Packet Name: sid_auth_accountchange
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) [32] Client key (A)
 * (STRING) Username
 * 
 * Remarks : 
 * This message is used to change the client's password.
 *
 */
struct request {
  uint8_t client_key;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x55
 * Packet Name: sid_auth_accountchange
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (BYTE) [32] Salt (s)
 * (BYTE) [32] Server key (B)
 * 
 * Remarks : 
 * Reports success or failure on a password change operation.
 * 
 * Status Codes:<dl><dd>0x00: Change accepted, requires proof.</dd>
 * <dd>0x01: Account doesn't exist.</dd>
 * <dd>0x05: Account requires upgrade - see SID_AUTH_ACCOUNTUPGRADE</dd>
 * <dd>Other: Unknown (failure).</dd></dl>
 * 
 * If an error occurs, the salt and server key values are set to zero.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x55, packet_bnetd_sid_auth_accountchange);
 *
 */
void packet_bnetd_sid_auth_accountchange(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.client_key = packet_get_byte(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x55);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountchange not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
