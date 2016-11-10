#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x16
 * Packet Name: d2gs_pickupitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Unit Type
 * (DWORD) Unit ID
 * (DWORD) Action ID
 * 
 * 
 * Remarks : 
 * Pick up a ground item to cursor buffer/inventory.
 * 
 * Possible action IDs:
 * 0x00 - Move item to inventory
 * 0x01 - Move item to cursor buffer
 * 
 * Please note: This message's official name is not known, and has been invented.
 * 
 *
 */
struct request {
  uint32_t unit_type;
  uint32_t unit_id;
  uint32_t action_id;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x16, packet_d2gs_d2gs_pickupitem);
 *
 */
void packet_d2gs_d2gs_pickupitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.unit_type = packet_get_int(request, ENDIAN_LITTLE);
  packet.unit_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.action_id = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x16);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_pickupitem not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
