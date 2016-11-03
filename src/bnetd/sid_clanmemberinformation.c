#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x82
 * Packet Name: sid_clanmemberinformation
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (DWORD) User's clan tag
 * (STRING) Username
 * 
 * Remarks : 
 * This packet requests information about a user and their current status within
 * their clan. If the user is in a clan, it'll return what clan they're in, their
 * rank, along with the time they joined it in a FILETIME structure.
 *
 */
struct request {
  uint32_t cookie;
  uint32_t users_clan_tag;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x82
 * Packet Name: sid_clanmemberinformation
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Status code
 * (STRING) Clan name
 * (BYTE) User's rank
 * (FILETIME) Date joined
 * 
 * Remarks : 
 * Status code:
 * This message follows the same status codes as listed on the Clan Message Codes
 * page.
 * <dd>0x00 = success</dd>
 * <dd>0x0C = user not found in that clan</dd>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x82, packet_bnetd_sid_clanmemberinformation);
 *
 */
void packet_bnetd_sid_clanmemberinformation(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.users_clan_tag = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x82);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanmemberinformation not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
