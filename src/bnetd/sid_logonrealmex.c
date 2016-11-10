#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <Eina.h>

#include "tristram.h"
#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x3E
 * Packet Name: sid_logonrealmex
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Client Token
 * (DWORD) [5] Hashed realm password
 * (STRING) Realm title
 * 
 * Remarks : 
 * Realm password is always "password". The password hash is created the same way
 * the hash is for logging on to an account.
 *
 */
struct request {
  uint32_t client_token;
  uint32_t hashed_realm_password[5];
  char *realm_title;
} __attribute__((packed));


/*
 * Packet ID: 0x3E
 * Packet Name: sid_logonrealmex
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) MCP Cookie
 * (DWORD) MCP Status
 * (DWORD) [2] MCP Chunk 1
 * (DWORD) IP
 * (DWORD) Port
 * (DWORD) [12] MCP Chunk 2
 * (STRING) Battle.net unique name
 * 
 * Remarks : 
 * This packet supplies the data necessary to connect to an MCP server. The cookie
 * value is defined in the first DWORD of SID_LOGONREALMEX and is simply returned
 * by Battle.net. If the length of the message (not including header) is greater
 * than 8, the client should continue to logon to the MCP. Otherwise, the Status
 * field is interpreted as an error code:
 * 
 * 0x80000001: Realm is unavailable
 * 0x80000002: Realm logon failed
 * 
 * Any other value indicates failure.
 * 
 * The first part of the MCP Chunk that must be sent to the MCP server includes the
 * Status and Cookie DWORDS, making MCP Chunk 1 the first 4 DWORDS of the packet.
 * 
 * There used to be a WORD at the end of this packet, which was apparently random.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3E, packet_bnetd_sid_logonrealmex);
 *
 */
void packet_bnetd_sid_logonrealmex(struct connection *conn, struct packet *request) {
  char *name = NULL;
  char *port = NULL;
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_realm_password[0] = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_realm_password[1] = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_realm_password[2] = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_realm_password[3] = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_realm_password[4] = packet_get_int(request, ENDIAN_LITTLE);
  packet.realm_title = packet_get_string(request, ENDIAN_LITTLE);

  CONFIG_GET_STRING("servers.realm.port", &port);
  CONFIG_GET_STRING("servers.realm.name", &name);
  
  if (!strncmp(packet.realm_title, name, strlen(name))) {
    // create the response packet.
    response = packet_new(PROTOCOL_BNETD);
    if (!response) {
      return;
    }
  
    // set response message id.
    packet_message_id_set(response, 0x3E);

    // set response data.
    packet_set_int(response, ENDIAN_LITTLE, packet.client_token);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_BIG, 0x7f000001); // 127.0.0.1
    packet_set_int(response, ENDIAN_LITTLE, net_connection_uuid_get(conn));
    packet_set_int(response, ENDIAN_BIG, 0x7f000001); // 127.0.0.1
    packet_set_int(response, ENDIAN_BIG, 0x17e10000); //0000e117); // 6113

    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x50583244); // PX2D
    packet_set_int(response, ENDIAN_LITTLE, 0x36385849); // 68XI
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x409);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);
    packet_set_int(response, ENDIAN_LITTLE, 0x01);

    
    packet_set_string(response, ENDIAN_LITTLE, "test");
    
    // send back response.
    net_connection_response_append(conn, response);
  }
  
  free(packet.realm_title);
}
