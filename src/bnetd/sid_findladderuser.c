#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2F
 * Packet Name: sid_findladderuser
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Product ID
 * (DWORD) League
 * (DWORD) Sort method
 * (STRING) Username
 * 
 * Remarks : 
 * Requests a user's status on ladder.
 * 
 * League is 1 for Ladder, 3 for Iron Man.
 * 
 * <dl><dt>Sort methods:</dt>
 * <dd>0x00: Highest Rating</dd>
 * <dd>0x01: Fastest Climbers (Unused)</dd>
 * <dd>0x02: Most Wins</dd>
 * <dd>0x03: Most Games Played</dd></dl>
 *
 */
struct request {
  uint32_t product_id;
  uint32_t league;
  uint32_t sort_method;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x2F
 * Packet Name: sid_findladderuser
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Rank.
 * 
 * Remarks : 
 * Contains the rank of the user specified by the request.
 * 
 * Rank is 0-based. A rank of 0xFFFFFFFF is no rank.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2F, packet_bnetd_sid_findladderuser);
 *
 */
void packet_bnetd_sid_findladderuser(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.league = packet_get_int(request, ENDIAN_LITTLE);
  packet.sort_method = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2F);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_findladderuser not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
