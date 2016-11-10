#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x54
 * Packet Name: sid_auth_accountlogonproof
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) [20] Client Password Proof (M1)
 * 
 * Remarks : 
 * This message is sent to the server after a successful SID_AUTH_ACCOUNTLOGON. It
 * contains the client's password proof. See [NLS/SRP Protocol] for more
 * information.
 *
 */
struct request {
  uint8_t client_password_proof;
} __attribute__((packed));


/*
 * Packet ID: 0x54
 * Packet Name: sid_auth_accountlogonproof
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (BYTE) [20] Server Password Proof (M2)
 * (STRING) Additional information
 * 
 * Remarks : 
 * Statuses:<dl>
 * <dd>0x00: Logon successful.</dd>
 * <dd>0x02: Incorrect password.</dd>
 * <dd>0x06: Account closed.</dd>
 * <dd>0x0E: An email address should be registered for this account.</dd>
 * <dd>0x0F: Custom error. A string at the end of this message contains the
 * error.</dd>
 * </dl>
 * 
 * This message confirms the validity of the client password proof and supplies the
 * server password proof. See [NLS/SRP Protocol] for more information.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x54, packet_bnetd_sid_auth_accountlogonproof);
 *
 */
void packet_bnetd_sid_auth_accountlogonproof(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.client_password_proof = packet_get_byte(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x54);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountlogonproof not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
