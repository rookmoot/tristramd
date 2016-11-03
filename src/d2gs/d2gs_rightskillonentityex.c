#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x0E
 * Packet Name: d2gs_rightskillonentityex
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
 * Uses your right skill on the Entity specefied in Entity ID, while holding the
 * hotkey for standing still(shift).
 * 
 * Please note: This message's official name is not known, and has been invented.
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
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0E, packet_d2gs_d2gs_rightskillonentityex);
 *
 */
void packet_d2gs_d2gs_rightskillonentityex(struct connection *conn, struct packet *request) {
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
  packet_message_id_set(response, 0x0E);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_rightskillonentityex not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
