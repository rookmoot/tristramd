#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x29
 * Packet Name: sid_logonresponse
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Client Token
 * (DWORD) Server Token
 * (DWORD) [5] Password Hash
 * (STRING) Username
 * 
 * Remarks : 
 * Contains Client's username & hashed password.
 * 
 * Battle.net password hashes are hashed twice. First, the password is hashed by
 * itsself, then the following data is hashed again and sent to Battle.net:
 * 
 * Client Token
 * Server Token
 * First password hash (20 bytes)
 * 
 * Passwords should be converted to lower case before hashing.
 *
 */
struct request {
  uint32_t client_token;
  uint32_t server_token;
  uint32_t password_hash;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x29
 * Packet Name: sid_logonresponse
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Reports success/fail on password authentication.
 * <dl><dt>Result:</dt>
 * <dd>0x00: Invalid password</dd>
 * <dd>0x01: Success</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x29, packet_bnetd_sid_logonresponse);
 *
 */
void packet_bnetd_sid_logonresponse(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.server_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.password_hash = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x29);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_logonresponse not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
