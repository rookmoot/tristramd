#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x50
 * Packet Name: d2gs_dropgold
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) PlayerID
 * (DWORD) GoldAmount
 * 
 * 
 * Remarks : 
 * Drops a pile of gold to the ground.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t playerid;
  uint32_t goldamount;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x50, packet_d2gs_d2gs_dropgold);
 *
 */
void packet_d2gs_d2gs_dropgold(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.playerid = packet_get_int(request, ENDIAN_LITTLE);
  packet.goldamount = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x50);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_dropgold not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
