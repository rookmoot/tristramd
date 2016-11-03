#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x28
 * Packet Name: d2gs_insertsocketitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID - Item to place in socket
 * (DWORD) Item ID - Socketed item
 * 
 * 
 * Remarks : 
 * Inserts the specified item into a socketed item.
 *
 */
struct request {
  uint32_t item_id___item_to_place_in_socket;
  uint32_t item_id___socketed_item;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x28, packet_d2gs_d2gs_insertsocketitem);
 *
 */
void packet_d2gs_d2gs_insertsocketitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id___item_to_place_in_socket = packet_get_int(request, ENDIAN_LITTLE);
  packet.item_id___socketed_item = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x28);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_insertsocketitem not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
