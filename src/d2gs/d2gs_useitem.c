#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x20
 * Packet Name: d2gs_useitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID
 * (DWORD) X coordinate
 * (DWORD) Y coordinate
 * 
 * 
 * Remarks : 
 * Uses the specified item (such as a potion, town portal scroll, etc.).
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t item_id;
  uint32_t x_coordinate;
  uint32_t y_coordinate;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x20, packet_d2gs_d2gs_useitem);
 *
 */
void packet_d2gs_d2gs_useitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.x_coordinate = packet_get_int(request, ENDIAN_LITTLE);
  packet.y_coordinate = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x20);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_useitem not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
