#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2A
 * Packet Name: sid_createaccount
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) [5] Hashed password
 * (STRING) Username
 * 
 * Remarks : 
 * Creates an account.
 * 
 * Usernames longer than 15 characters are truncated, and the password is only
 * hashed once (unlike SID_LOGONRESPONSE).
 * 
 * This packet is identical to SID_CREATEACCOUNT2, but the response is limited to
 * success/fail. Developers who wish to provide a reason for account creation
 * failure should use SID_CREATEACCOUNT2.
 * 
 * Currently, SID_CREATEACCOUNT2 may be used with any product, but the
 * protocol-correct packet to use depends on the product you are emulating.
 *
 */
struct request {
  uint32_t hashed_password;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x2A
 * Packet Name: sid_createaccount
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Results:
 * <dl><dd>0x00: Fail</dd>
 * <dd>0x01: Success</dd></dl>
 * Reports success/fail on account creation.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2A, packet_bnetd_sid_createaccount);
 *
 */
void packet_bnetd_sid_createaccount(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.hashed_password = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2A);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_createaccount not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
