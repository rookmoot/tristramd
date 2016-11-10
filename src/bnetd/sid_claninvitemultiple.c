#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x71
 * Packet Name: sid_claninvitemultiple
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) Clan name
 * (DWORD) Clan tag
 * (BYTE) Number of users to invite
 * (STRING) [] Usernames to invite
 * 
 * 
 * Remarks : 
 * This message is used to invite the initial 9 required members to a new clan. The
 * users specified in this packet will receive 0x72.
 *
 */
struct request {
  uint32_t cookie;
  char * clan_name;
  uint32_t clan_tag;
  uint8_t number_of_users_to_invite;
  char * usernames_to_invite;
} __attribute__((packed));


/*
 * Packet ID: 0x71
 * Packet Name: sid_claninvitemultiple
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Result
 * (STRING) [] Failed account names
 * 
 * Remarks : 
 * Multiple invitation response.
 * 
 * Result:<dl>
 * <dd>0x00: Success (everyone accepted)</dd>
 * <dd>0x04: Declined</dd>
 * <dd>0x05: Not available (not in channel or already in a clan)</dd>
 * <dd>0x0B: Clan name contains bad word</dd></dl>
 * Failed account names:<dl><dd>A list of accountnames which failed to accept the
 * invitation successfully. On complete success, this is null.</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x71, packet_bnetd_sid_claninvitemultiple);
 *
 */
void packet_bnetd_sid_claninvitemultiple(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.clan_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.clan_tag = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_users_to_invite = packet_get_byte(request, ENDIAN_LITTLE);
  packet.usernames_to_invite = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x71);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_claninvitemultiple not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
