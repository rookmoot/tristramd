#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x18
 * Packet Name: mcp_charupgrade
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Character Name
 * 
 * Remarks : 
 * Converts a non-expansion character to expansion.
 *
 */
struct request {
  char * character_name;
} __attribute__((packed));


/*
 * Packet ID: 0x18
 * Packet Name: mcp_charupgrade
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Converts a non-expansion character to expansion.
 * <dl>
 * <dt>Known return values:</dt>
 * <dd>0x00: Success</dd>
 * <dd>0x46: Character not found</dd>
 * <dd>0x7A: Upgrade failed</dd>
 * <dd>0x7B: Character is expired</dd>
 * <dd>0x7C: Already expansion character</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x18, packet_realm_mcp_charupgrade);
 *
 */
void packet_realm_mcp_charupgrade(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.character_name = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x18);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_realm_mcp_charupgrade not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
