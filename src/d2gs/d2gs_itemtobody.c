#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1A
 * Packet Name: d2gs_itemtobody
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
 * Moves item from player's cursor buffer to body location. Body locations are
 * identified by the line number in the data file bodylocs.txt.
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
 * These values have been recorded for mercenary body locations, but aren't
 * confirmed: (Note, each location ID is prefixed with 0x61)
 * Example: 1A 64 00 00 00 61 02 00 00 (Move item 0x64 to Mercenary Right-hand
 * weapon)
 * 
 * 0x01 - Mercenary Helm
 * 0x02 - Mercenary Right-hand Weapon
 * 0x03 - Mercenary Armor
 * 0x04 - Mercenary Left-hand Weapon
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
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x1A, packet_d2gs_d2gs_itemtobody);
 *
 */
void packet_d2gs_d2gs_itemtobody(struct connection *conn, struct packet *request) {
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
  packet_message_id_set(response, 0x1A);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_itemtobody not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
