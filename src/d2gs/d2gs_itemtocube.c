#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2A
 * Packet Name: d2gs_itemtocube
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID
 * (DWORD) Cube ID
 * 
 * 
 * Remarks : 
 * Moves item from player's cursor buffer into Horadric cube.
 *
 */
struct request {
  uint32_t item_id;
  uint32_t cube_id;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x2A, packet_d2gs_d2gs_itemtocube);
 *
 */
void packet_d2gs_d2gs_itemtocube(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.cube_id = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2A);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_itemtocube not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
