#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1D
 * Packet Name: d2gs_switchbodyitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID
 * (DWORD) Body Location
 * 
 * 
 * Remarks : 
 * Swaps item in player's cursor buffer with item in the body location.
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
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t item_id;
  uint32_t body_location;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1D, packet_d2gs_d2gs_switchbodyitem);
 *
 */
void packet_d2gs_d2gs_switchbodyitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.body_location = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x1D);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_switchbodyitem not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
