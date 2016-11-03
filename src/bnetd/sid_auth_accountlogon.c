#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x53
 * Packet Name: sid_auth_accountlogon
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) [32] Client Key ('A')
 * (STRING) Username
 * 
 * 
 * Remarks : 
 * This message is sent to the server to initiate a logon. It consists of the
 * client's public key and their UserName.
 * 
 * The client's public key is a value calculated by the client and used for a
 * single logon. For more information, see [NLS/SRP Protocol].
 *
 */
struct request {
  uint8_t client_key;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x53
 * Packet Name: sid_auth_accountlogon
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (BYTE) [32] Salt (s)
 * (BYTE) [32] Server Key (B)
 * 
 * Remarks : 
 * Reports the success or failure of the logon request.
 * 
 * Possible status codes:<dl><dd>0x00: Logon accepted, requires proof.</dd>
 * <dd>0x01: Account doesn't exist.</dd>
 * <dd>0x05: Account requires upgrade.</dd>
 * <dd>Other: Unknown (failure).</dd></dl>
 * 
 * See the [NLS/SRP Protocol] document for more information.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x53, packet_bnetd_sid_auth_accountlogon);
 *
 */
void packet_bnetd_sid_auth_accountlogon(struct connection *conn, struct packet *request) {
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
  packet_message_id_set(response, 0x53);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountlogon not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
