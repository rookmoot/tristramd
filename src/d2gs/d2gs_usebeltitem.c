#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x26
 * Packet Name: d2gs_usebeltitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID
 * (DWORD) Unknown - Possibly unused
 * (DWORD) Unknown - Possibly unused
 * 
 * 
 * Remarks : 
 * Uses the specified item in the player's belt.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t item_id;
  uint32_t unknown___possibly_unused;
  uint32_t unknown___possibly_unused_790456933;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x26, packet_d2gs_d2gs_usebeltitem);
 *
 */
void packet_d2gs_d2gs_usebeltitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown___possibly_unused = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown___possibly_unused = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x26);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_usebeltitem not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
