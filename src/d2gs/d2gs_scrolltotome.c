#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x29
 * Packet Name: d2gs_scrolltotome
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID - Scroll
 * (DWORD) Item ID - Tome
 * 
 * 
 * Remarks : 
 * Places a scroll into a tome of scrolls.
 * 
 * Note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t item_id___scroll;
  uint32_t item_id___tome;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x29, packet_d2gs_d2gs_scrolltotome);
 *
 */
void packet_d2gs_d2gs_scrolltotome(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id___scroll = packet_get_int(request, ENDIAN_LITTLE);
  packet.item_id___tome = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x29);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_scrolltotome not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
