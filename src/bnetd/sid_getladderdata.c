#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2E
 * Packet Name: sid_getladderdata
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Product ID
 * (DWORD) League
 * (DWORD) Sort method
 * (DWORD) Starting rank
 * (DWORD) Number of ranks to list
 * 
 * Remarks : 
 * Requests ladder listing.
 * 
 * League is 1 for Ladder, 3 for Iron Man.
 * 
 * <dl><dt>Sort methods:</dt>
 * <dd>0x00: Highest Rating</dd>
 * <dd>0x01: Fastest Climbers (Unused)</dd>
 * <dd>0x02: Most Wins</dd>
 * <dd>0x03: Most Games Played</dd></dl>
 * 
 * Rank is 0-based.
 * 
 * Maximum number of ranks to list is 20.
 *
 */
struct request {
  uint32_t product_id;
  uint32_t league;
  uint32_t sort_method;
  uint32_t starting_rank;
  uint32_t number_of_ranks_to_list;
} __attribute__((packed));


/*
 * Packet ID: 0x2E
 * Packet Name: sid_getladderdata
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Product ID
 * (DWORD) League
 * (DWORD) Sort method
 * (DWORD) Starting rank
 * (DWORD) Number of ranks listed (Count of items in list)
 * <dl><dt>For each list item:</dt>
 * <dd>(DWORD) Wins</dd>
 * <dd>(DWORD) Losses</dd>
 * <dd>(DWORD) Disconnects</dd>
 * <dd>(DWORD) Rating</dd>
 * <dd>(DWORD) Rank</dd>
 * <dd>(DWORD) Official wins</dd>
 * <dd>(DWORD) Official losses</dd>
 * <dd>(DWORD) Official disconnects</dd>
 * <dd>(DWORD) Official rating</dd>
 * <dd>(DWORD) Unknown</dd>
 * <dd>(DWORD) Official rank</dd>
 * <dd>(DWORD) Unknown</dd>
 * <dd>(DWORD) Unknown</dd>
 * <dd>(DWORD) Highest rating</dd>
 * <dd>(DWORD) Unknown</dd>
 * <dd>(DWORD) Season</dd>
 * <dd>(FILETIME) Last game time</dd>
 * <dd>(FILETIME) Official last game time</dd>
 * <dd>(STRING) Name</dd></dl>
 * 
 * Remarks : 
 * Contains the requested ladder data.
 * 
 * League is 1 for Ladder, 3 for Iron Man.
 * 
 * <dl><dt>Sort methods:</dt>
 * <dd>0x00: Highest Rating</dd>
 * <dd>0x01: Fastest Climbers (Unused)</dd>
 * <dd>0x02: Most Wins</dd>
 * <dd>0x03: Most Games Played</dd></dl>
 * 
 * All unknowns are 0.
 * Rank is 0-based. A rank of 0xFFFFFFFF is no rank.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2E, packet_bnetd_sid_getladderdata);
 *
 */
void packet_bnetd_sid_getladderdata(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.league = packet_get_int(request, ENDIAN_LITTLE);
  packet.sort_method = packet_get_int(request, ENDIAN_LITTLE);
  packet.starting_rank = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_ranks_to_list = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2E);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_getladderdata not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
