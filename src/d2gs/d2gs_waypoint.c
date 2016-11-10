#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x49
 * Packet Name: d2gs_waypoint
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Waypoint ID
 * (BYTE) Unknown - Appears to be random
 * (WORD) Unknown - 0x00
 * (BYTE) Level number
 * (WORD) Unknown - 0x00
 * 
 * 
 * Remarks : 
 * Requests to go to a waypoint if it was already activated.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint8_t waypoint_id;
  uint8_t unknown___appears_to_be_random;
  uint16_t unknown___x;
  uint8_t level_number;
  uint16_t unknown___x_1527604591;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x49, packet_d2gs_d2gs_waypoint);
 *
 */
void packet_d2gs_d2gs_waypoint(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.waypoint_id = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown___appears_to_be_random = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown___x = packet_get_short(request, ENDIAN_LITTLE);
  packet.level_number = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown___x = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x49);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_waypoint not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
