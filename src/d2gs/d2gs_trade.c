#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x4F
 * Packet Name: d2gs_trade
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Request ID
 * (WORD)  Gold Amount
 * 
 * 
 * Remarks : 
 * This message should be used for manipulating the trading window, the Horadric
 * Cube item window, and the Stash window.
 * 
 * Possible Request ID's:
 * [0x02] Cancel trade request
 * [0x03] Accept trade request
 * [0x04] Press Accept button (accept)
 * [0x07] Press Accept button (unaccept) - This should be sent when placing items
 * in the trade window as well.
 * [0x08] Refresh window (See below for examples on usage)
 * [0x12] Close stash window
 * [0x13] Move gold (stash to inventory)
 * [0x14] Move gold (inventory to stash)
 * [0x17] Close Horadric Cube window
 * 
 * Examples of using Refresh ID:
 * Item has been picked up to mouse cursor buffer.
 * Item has been placed in the trade screen.
 * Accepted or closed a trade window.
 * Manual unclick of Accept button.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t request_id;
  uint16_t gold_amount;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x4F, packet_d2gs_d2gs_trade);
 *
 */
void packet_d2gs_d2gs_trade(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.gold_amount = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x4F);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_trade not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
