#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x0C
 * Packet Name: d2gs_rightskillonlocation
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) X coordinate
 * (WORD) Y coordinate
 * 
 * 
 * Remarks : 
 * Uses the currently selected skill at the specified location.
 * 
 * This packet is sent when the location is first clicked with the mouse.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint16_t x_coordinate;
  uint16_t y_coordinate;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x0C, packet_d2gs_d2gs_rightskillonlocation);
 *
 */
void packet_d2gs_d2gs_rightskillonlocation(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.x_coordinate = packet_get_short(request, ENDIAN_LITTLE);
  packet.y_coordinate = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x0C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_rightskillonlocation not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
