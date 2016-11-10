#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x32
 * Packet Name: d2gs_npcbuy
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) NPC ID - Unconfirmed
 * (DWORD) Item ID - Unconfirmed
 * (DWORD) Buffer Type - Unconfirmed
 * (DWORD) Cost
 * 
 * 
 * Remarks : 
 * Buys an item from a Non Player Character.
 * 
 * Possible Buffer Types:
 * 0x00 - Regular (ordinary item)
 * 0x02 - Gambled
 * 
 * Please note: This message's official name is not known, and has been invented.
 * 
 *
 */
struct request {
  uint32_t npc_id___unconfirmed;
  uint32_t item_id___unconfirmed;
  uint32_t buffer_type___unconfirmed;
  uint32_t cost;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x32, packet_d2gs_d2gs_npcbuy);
 *
 */
void packet_d2gs_d2gs_npcbuy(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.npc_id___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.item_id___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.buffer_type___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.cost = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x32);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_npcbuy not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
