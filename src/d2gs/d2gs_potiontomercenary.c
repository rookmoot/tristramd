#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x61
 * Packet Name: d2gs_potiontomercenary
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Unknown - 0x00
 * 
 * 
 * Remarks : 
 * Takes the potion your cursor holds and gives it to the mercenary. Please note:
 * This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint16_t unknown___x;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x61, packet_d2gs_d2gs_potiontomercenary);
 *
 */
void packet_d2gs_d2gs_potiontomercenary(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.unknown___x = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x61);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_potiontomercenary not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
