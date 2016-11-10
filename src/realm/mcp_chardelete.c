#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "character.h"
#include "account.h"

/*
 * Packet ID: 0x0A
 * Packet Name: mcp_chardelete
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Unknown (0)
 * (STRING) Character name
 * 
 * Remarks : 
 * Deletes a character.
 *
 */
struct request {
  uint16_t unknown;
  char * character_name;
} __attribute__((packed));


/*
 * Packet ID: 0x0A
 * Packet Name: mcp_chardelete
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Deletion response.
 * <dl>
 * <dt>Known values:</dt>
 * <dd>
 * <dl>
 * <dt>(Diablo II v1.10 or later)</dt>
 * <dd>0x00: Success</dd>
 * <dd>0x49: Character doesn't exist</dd>
 * </dl>
 * <dl>
 * <dt>(Diablo II v1.09 or earlier)</dt>
 * <dd>0x00: Character doesn't exist</dd>
 * <dd>0x49: Success</dd>
 * </dl>
 * </dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x0A, packet_realm_mcp_chardelete);
 *
 */
void packet_realm_mcp_chardelete(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;
  struct accounts_characters *ac = NULL;
  
  memset(&packet, 0, sizeof(packet));
  packet.unknown = packet_get_short(request, ENDIAN_LITTLE);
  packet.character_name = packet_get_string(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x0A);
  
  ac = (struct accounts_characters *)sql_select(
    "accounts_characters", "name='%s' AND account_id=%d",
    packet.character_name,
    conn->account->id
  );

  if (ac && ac->id) {
    character_delete(conn->account, packet.character_name);
    packet_set_int(response, ENDIAN_LITTLE, 0x00);
  } else {
    packet_set_int(response, ENDIAN_LITTLE, 0x49);
  }

  free(packet.character_name);
  net_connection_response_append(conn, response);
}
