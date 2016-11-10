#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x70
 * Packet Name: sid_clanfindcandidates
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (DWORD) Clan Tag
 * 
 * Remarks : 
 * This message is sent to the server to check for viable candidates in the channel
 * and friend list, and also to check the availability of the specified clan tag.
 * If 10 or more candidates are found, the official client would prompt with a
 * selection of users that he wants to invite to start a clan.
 *
 */
struct request {
  uint32_t cookie;
  uint32_t clan_tag;
} __attribute__((packed));


/*
 * Packet ID: 0x70
 * Packet Name: sid_clanfindcandidates
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Status
 * (BYTE) Number of potential candidates
 * (STRING) [] Usernames
 * 
 * Remarks : 
 * Contains the list of potential candidates.
 * 
 * Valid Status codes:<dl>
 * <dd>0x00: Successfully found candidate(s)</dd>
 * <dd>0x01: Clan tag already taken</dd>
 * <dd>0x08: Already in clan</dd>
 * <dd>0x0A: Invalid clan tag specified</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x70, packet_bnetd_sid_clanfindcandidates);
 *
 */
void packet_bnetd_sid_clanfindcandidates(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.clan_tag = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x70);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanfindcandidates not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
