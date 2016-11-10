#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x25
 * Packet Name: d2gs_switchbeltitem
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Item ID - Cursor buffer
 * (DWORD) Item ID - Item to be replaced
 * 
 * 
 * Remarks : 
 * Replaces item in belt with item in player's cursor buffer.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t item_id___cursor_buffer;
  uint32_t item_id___item_to_be_replaced;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x25, packet_d2gs_d2gs_switchbeltitem);
 *
 */
void packet_d2gs_d2gs_switchbeltitem(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.item_id___cursor_buffer = packet_get_int(request, ENDIAN_LITTLE);
  packet.item_id___item_to_be_replaced = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x25);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_switchbeltitem not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
