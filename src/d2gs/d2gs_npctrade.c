#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x38
 * Packet Name: d2gs_npctrade
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Trade Type - Unconfirmed
 * (DWORD) NPC ID - Unconfirmed
 * (DWORD) Unknown - Unconfirmed - Possible value 0x00
 * 
 * 
 * Remarks : 
 * This packet's use is currently unconfirmed.
 * 
 * Possible Trade Types:
 * 0x01 - Trade
 * 0x02 - Gamble
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t trade_type___unconfirmed;
  uint32_t npc_id___unconfirmed;
  uint32_t unknown___unconfirmed___possible_value_x;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x38, packet_d2gs_d2gs_npctrade);
 *
 */
void packet_d2gs_d2gs_npctrade(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.trade_type___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.npc_id___unconfirmed = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown___unconfirmed___possible_value_x = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x38);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_npctrade not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
