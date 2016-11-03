#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x05
 * Packet Name: mcp_gamelist
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request ID
 * (DWORD) Unknown (0)
 * (STRING) Search String *
 * 
 * Remarks : 
 * Requests a game listing.
 * 
 * * Normally blank. If a non-empty string is sent, games will be returned that
 * include this string in their names. This is not used by the client, but still
 * exists.
 *
 */
struct request {
  uint16_t request_id;
  uint32_t unknown;
  char * search_string;
} __attribute__((packed));


/*
 * Packet ID: 0x05
 * Packet Name: mcp_gamelist
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Request Id
 * (DWORD) Index
 * (BYTE) Number of players in game
 * (DWORD) Status
 * (STRING) Game name
 * (STRING) Game description
 * 
 * Remarks : 
 * Instead of receiving a single response that has a list of all the games, the
 * client will receive this packet once for every game listed by the server.
 * <dl>
 * <dt>Request Id:</dt>
 * <dd>Like a cookie. This value will be whatever you sent the server in
 * MCP_GAMELIST.</dd>
 * <dt>Index:</dt>
 * <dd>The game's index on the server.</dd>
 * <dt>Number of players in game:</dt>
 * <dd>Self explanatory.</dd>
 * <dt>Status:</dt>
 * <dd>0x00300004: Game is available to join</dd>
 * <dd>0xFFFFFFFF: Server is down</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x05, packet_realm_mcp_gamelist);
 *
 */
void packet_realm_mcp_gamelist(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_short(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.search_string = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x05);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_realm_mcp_gamelist not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
