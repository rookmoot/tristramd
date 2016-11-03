#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x01
 * Packet Name: mcp_startup
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) MCP Cookie
 * (DWORD) MCP Status
 * (DWORD) [2] MCP Chunk 1
 * (DWORD) [12] MCP Chunk 2
 * (STRING) Battle.net Unique Name
 * 
 * Remarks : 
 * This packet authenticates the client with the MCP and allows character querying
 * and logon to proceed.
 * All 16 DWORDs (Cookie, Status, Chunk 1, and Chunk 2) are received from the
 * server via SID_LOGONREALMEX.
 * 
 * Not much information is known about the DWORD values, other than that they're
 * received from the server. The following information needs work:
 * <dl>
 * <dd>MCP Cookie: Client Token</dd>
 * <dd>MCP Status: Unknown</dd>
 * <dd>MCP Chunk 1 [01]: Server IP (BNCS)</dd>
 * <dd>MCP Chunk 1 [02]: UDP Value *</dd>
 * <dd>MCP Chunk 2 [01]: Unknown</dd>
 * <dd>MCP Chunk 2 [02]: Unknown</dd>
 * <dd>MCP Chunk 2 [03]: Something to do with the gateway</dd>
 * <dd>MCP Chunk 2 [04]: Product (D2DV/D2XP)</dd>
 * <dd>MCP Chunk 2 [05]: Platform (IX86/PMAC/XMAC)</dd>
 * <dd>MCP Chunk 2 [06]: Unknown</dd>
 * <dd>MCP Chunk 2 [07]: Language ID (1033 [0x409] for enUS)</dd>
 * <dd>MCP Chunk 2 [08]: Unknown</dd>
 * <dd>MCP Chunk 2 [09]: Unknown</dd>
 * <dd>MCP Chunk 2 [10]: Unknown</dd>
 * <dd>MCP Chunk 2 [11]: Unknown</dd>
 * <dd>MCP Chunk 2 [12]: Unknown</dd>
 * </dl>
 * 
 * This is purely speculation, but as there are 5 unknown DWORDs at the end of this
 * chunk, it is possible that it is actually a hash of something.
 * 
 * * UDPValue: No one really knows what this is, however, it is used in 2nd DWORD
 * of the UDP packet PKT_CONNTEST2. The client receives it in SID_AUTH_INFO.
 *
 */
struct request {
  uint32_t mcp_cookie;
  uint32_t mcp_status;
  uint32_t mcp_chunk;
  uint32_t uuid;
  char * battlenet_unique_name;
} __attribute__((packed));


/*
 * Packet ID: 0x01
 * Packet Name: mcp_startup
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * 
 * Remarks : 
 * Contains the return value from MCP Startup.
 * <dl>
 * <dt>Known return values:</dt>
 * <dd>0x02, 0x0A-0x0D: Realm Unavailable: No Battle.net connection detected.</dd>
 * <dd>0x7E: CDKey banned from realm play.</dd>
 * <dd>0x7F: Temporary IP ban "Your connection has been temporarily restricted from
 * this realm. Please try to log in at another time."</dd>
 * <dd>Else: Success.</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x01, packet_realm_mcp_startup);
 *
 */
void packet_realm_mcp_startup(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.mcp_cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.mcp_status = packet_get_int(request, ENDIAN_LITTLE);
  packet.mcp_chunk = packet_get_int(request, ENDIAN_LITTLE);
  packet.uuid = packet_get_int(request, ENDIAN_LITTLE);
  
  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x01);
  
  conn = net_connection_clone(conn, packet.uuid);
  if (!conn->account) {
    EINA_LOG_CRIT("Failed to find account for this client.");
    EINA_LOG_CRIT("This may be a hack attempt.");
    EINA_LOG_CRIT("Closing connection.");

    // sending error packet result.
    packet_set_int(response, ENDIAN_LITTLE, 0x02);
    connection_response_append(conn, response);
    return;
  }
  
  packet_set_int(response, ENDIAN_LITTLE, 0x00);
  connection_response_append(conn, response);
}
