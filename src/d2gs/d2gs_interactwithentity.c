#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x13
 * Packet Name: d2gs_interactwithentity
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Entity Type
 * (DWORD) Entity ID
 * 
 * Entity types
 * --------
 * 00 - Players
 * 01 - Monsters, NPCs, and Mercenaries
 * 02 - Stash, Waypoint, Chests, Portals, others.
 * 03 - Missiles
 * 04 - Items
 * 05 - Entrances
 * 
 * Remarks : 
 * Interacts with the specified Entity.
 * For players and npc's, it will send a request to interact.
 * The interaction depends on the type of the unit or object.
 * For others it will trigger the object, for example using a shrine, looting a
 * corpse you have permission to loot, or opening and closing a door.
 * 
 * This packet is always followed by other's that relate to the type of
 * interaction.
 *
 */
struct request {
  uint32_t entity_type;
  uint32_t entity_id;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x13, packet_d2gs_d2gs_interactwithentity);
 *
 */
void packet_d2gs_d2gs_interactwithentity(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.entity_type = packet_get_int(request, ENDIAN_LITTLE);
  packet.entity_id = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x13);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_interactwithentity not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
