#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x52
 * Packet Name: sid_auth_accountcreate
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) [32] Salt (s)
 * (BYTE) [32] Verifier (v)
 * (STRING) Username
 * 
 * Remarks : 
 * This message is sent to create an NLS-style account. It contains the client's
 * salt and verifier values, which are saved by the server for use with future
 * logons.
 * 
 * See the [NLS/SRP Protocol] page for more information.
 *
 */
struct request {
  uint8_t salt;
  uint8_t verifier;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x52
 * Packet Name: sid_auth_accountcreate
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * 
 * Remarks : 
 * The message reports the success or failure of an account creation attempt.
 * 
 * Status:<dl><dd>0x00: Successfully created account name.</dd>
 * <dd>0x04: Name already exists.</dd>
 * <dd>0x07: Name is too short/blank.</dd>
 * <dd>0x08: Name contains an illegal character.</dd>
 * <dd>0x09: Name contains an illegal word.</dd>
 * <dd>0x0a: Name contains too few alphanumeric characters.</dd>
 * <dd>0x0b: Name contains adjacent punctuation characters.</dd>
 * <dd>0x0c: Name contains too many punctuation characters.</dd>
 * <dd>Any other: Name already exists.</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x52, packet_bnetd_sid_auth_accountcreate);
 *
 */
void packet_bnetd_sid_auth_accountcreate(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.salt = packet_get_byte(request, ENDIAN_LITTLE);
  packet.verifier = packet_get_byte(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x52);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountcreate not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
