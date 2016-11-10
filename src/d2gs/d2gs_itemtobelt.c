#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x23
 * Packet Name: d2gs_itemtobelt
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID
 * (DWORD) Belt Location
 * 
 * 
 * Remarks : 
 * Moves an item into the player's belt.
 * 
 * Extra Info: The status of this information is: Public Belt locations start at
 * 0x00 and go from right to left, starting at the lowest row and moving across,
 * then going up one row. For example, location 0x03 would be at the bottom right
 * hand corner and location 0x0C would be at the top left corner in a 4-slot belt.
 * This can be calculated as (row*4+column).
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t item_id;
  uint32_t belt_location;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x23, packet_d2gs_d2gs_itemtobelt);
 *
 */
void packet_d2gs_d2gs_itemtobelt(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.belt_location = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x23);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_itemtobelt not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
