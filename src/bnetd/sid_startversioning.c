#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x06
 * Packet Name: sid_startversioning
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Platform ID
 * (DWORD) Product ID
 * (DWORD) Version Byte
 * (DWORD) Unknown (0)
 * 
 * Remarks : 
 * This message is sent to the server to start the process of checking the game
 * files. This message is part of the old logon process for products before
 * Starcraft.
 *
 */
struct request {
  uint32_t platform_id;
  uint32_t product_id;
  uint32_t version_byte;
  uint32_t unknown;
} __attribute__((packed));


/*
 * Packet ID: 0x06
 * Packet Name: sid_startversioning
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (FILETIME) MPQ Filetime
 * (STRING) MPQ Filename
 * (STRING) ValueString
 * 
 * 
 * Remarks : 
 * Contains CheckRevision values.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x06, packet_bnetd_sid_startversioning);
 *
 */
void packet_bnetd_sid_startversioning(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.platform_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.version_byte = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x06);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_startversioning not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
