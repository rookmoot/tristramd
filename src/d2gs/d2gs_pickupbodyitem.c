#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1C
 * Packet Name: d2gs_pickupbodyitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Body Location
 * 
 * Remarks : 
 * Pickup an item from a Body Location to you're cursor buffer.
 * 
 * Possible Body Locations:
 * 0x01 - Helm
 * 0x02 - Amulet
 * 0x03 - Armor
 * 0x04 - Left-hand weapon
 * 0x05 - Right-hand weapon
 * 0x06 - Left-hand ring
 * 0x07 - Right-hand ring
 * 0x08 - Belt
 * 0x09 - Boots
 * 0x0A - Gloves
 *
 */
struct request {
  uint16_t body_location;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1C, packet_d2gs_d2gs_pickupbodyitem);
 *
 */
void packet_d2gs_d2gs_pickupbodyitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.body_location = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x1C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_pickupbodyitem not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
