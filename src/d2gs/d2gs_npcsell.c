#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x33
 * Packet Name: d2gs_npcsell
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) NPC ID - Unconfirmed
 * (DWORD) Item ID - Unconfirmed
 * (DWORD) Buffer ID - Unconfirmed - Possible value 0x04
 * (DWORD) Cost
 * 
 * 
 * Remarks : 
 * Sell an item to a Non Player Character.
 * 
 * The Buffer ID refers to the windows in the NPC trade screen, depending on what
 * you are selling the Buffer ID will match the item type.
 * After selling the item it will appear in the given Buffer ID.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t npc_id___unconfirmed;
  uint32_t item_id___unconfirmed;
  uint32_t buffer_id___unconfirmed___possible_value_x;
  uint32_t cost;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x33, packet_d2gs_d2gs_npcsell);
 *
 */
void packet_d2gs_d2gs_npcsell(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.npc_id___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.item_id___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.buffer_id___unconfirmed___possible_value_x = packet_get_int(request, ENDIAN_LITTLE);
  packet.cost = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x33);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_npcsell not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
