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
 * Packet ID: 0x07
 * Packet Name: mcp_charlogon
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Character name
 * 
 * Remarks : 
 * Logs onto the realm.
 * 
 * Note that attempting to log on using an expansion character on D2DV will result
 * in an IPBan by both Battle.net and the Realm.
 *
 */
struct request {
  char *character_name;
} __attribute__((packed));


/*
 * Packet ID: 0x07
 * Packet Name: mcp_charlogon
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Logon response.
 * <dl>
 * <dt>Known values:</dt>
 * <dd>0x00: Success</dd>
 * <dd>0x46: Player not found</dd>
 * <dd>0x7A: Logon failed</dd>
 * <dd>0x7B: Character expired</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x07, packet_realm_mcp_charlogon);
 *
 */
void packet_realm_mcp_charlogon(struct connection *conn, struct packet *request) {
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
  packet_message_id_set(response, 0x07);

  struct character *character = character_load(conn->account, packet.character_name);  
  if (character) {
    conn->account->character = character;
    packet_set_int(response, ENDIAN_LITTLE, 0x00);
  } else {
    packet_set_int(response, ENDIAN_LITTLE, 0x7A);
  }

  free(packet.character_name);
  net_connection_response_append(conn, response);
}
