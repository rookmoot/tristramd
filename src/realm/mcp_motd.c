#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "file.h"

/*
 * Packet ID: 0x12
 * Packet Name: mcp_motd
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Requests the realm's MOTD.
 *
 */

/*
 * Packet ID: 0x12
 * Packet Name: mcp_motd
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Unknown
 * (STRING) MOTD
 * 
 * Remarks : 
 * Contains the realm's MOTD. The Byte at the beginning is apparently ignored.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x12, packet_realm_mcp_motd);
 *
 */
void packet_realm_mcp_motd(struct connection *conn, struct packet *request EINA_UNUSED) {
  struct packet *response = NULL;
  struct file *file = NULL;
  size_t size = 0;
  
  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x12);

  // unknow byte
  packet_set_byte(response, ENDIAN_LITTLE, 0x00);

  file = file_new("realm_motd.txt");
  if (file) {
    packet_set_string(response, ENDIAN_LITTLE, (const char *)file_chunk_data_get(file, &size));
    file_free(file);
  }
  net_connection_response_append(conn, response);
}
