#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x17
 * Packet Name: mcp_charlist
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of characters to list
 * 
 * Remarks : 
 * Requests a character list.
 *
 */
struct request {
  uint32_t number_of_characters_to_list;
} __attribute__((packed));


/*
 * Packet ID: 0x17
 * Packet Name: mcp_charlist
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Number of characters requested
 * (DWORD) Number of characters that exist on this account
 * (WORD) Number of characters returned
 * <dl>
 * <dt>For each character:</dt>
 * <dd>(STRING) Character name</dd>
 * <dd>(STRING) Character statstring</dd>
 * </dl>
 * 
 * Remarks : 
 * Lists characters.
 * 
 * The statstrings in this packet  do not contain the product or character name.
 * Everything else is the same as a character statstring you would receive from a
 * character entering the channel.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x17, packet_realm_mcp_charlist);
 *
 */
void packet_realm_mcp_charlist(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.number_of_characters_to_list = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x17);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_realm_mcp_charlist not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
