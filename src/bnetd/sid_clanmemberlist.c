#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x7D
 * Packet Name: sid_clanmemberlist
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * 
 * Remarks : 
 * Requests a clan memberlist.
 *
 */
struct request {
  uint32_t cookie;
} __attribute__((packed));


/*
 * Packet ID: 0x7D
 * Packet Name: sid_clanmemberlist
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Number of Members
 * For each member:<dl>
 * <dd>(STRING) Username</dd>
 * <dd>(BYTE) Rank</dd>
 * <dd>(BYTE) Online Status</dd>
 * <dd>(STRING) Location</dd></dl>
 * 
 * Remarks : 
 * Contains the members of a clan.
 * 
 * Online Status:<dl><dd>0x00: Offline
 * 0x01: Online</dd></dl>
 * Rank:<dl><dd>0x00: Initiate that has been in the clan for less than one
 * week</dd>
 * <dd>0x01: Initiate that has been in the clan for over one week</dd>
 * <dd>0x02: Member</dd>
 * <dd>0x03: Officer</dd>
 * <dd>0x04: Leader</dd></dl>
 * Location:<dl><dd>Where the user is, i.e., game name, channel name, or this may
 * be null if the user is not online.</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7D, packet_bnetd_sid_clanmemberlist);
 *
 */
void packet_bnetd_sid_clanmemberlist(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x7D);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanmemberlist not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
