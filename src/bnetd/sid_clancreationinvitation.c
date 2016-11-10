#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x72
 * Packet Name: sid_clancreationinvitation
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (DWORD) Clan tag
 * (STRING) Inviter name
 * (BYTE) Status
 * 
 * Remarks : 
 * This message is used to reply to an invitation to create a new clan.
 * 
 * You must reply to this packet using one of two statuses:
 * <dl>
 * <dd>0x04: Declined</dd>
 * <dd>0x06: Accepted</dd>
 * </dl>
 * If you do not reply after about 30 seconds (give or take), the server assumes
 * declined.
 *
 */
struct request {
  uint32_t cookie;
  uint32_t clan_tag;
  char * inviter_name;
  uint8_t status;
} __attribute__((packed));


/*
 * Packet ID: 0x72
 * Packet Name: sid_clancreationinvitation
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (DWORD) Clan Tag
 * (STRING) Clan Name
 * (STRING) Inviter's username
 * (BYTE) Number of users being invited
 * (STRING) [] List of users being invited
 * 
 * 
 * Remarks : 
 * Received when a user is inviting you to create a new clan on Battle.net.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x72, packet_bnetd_sid_clancreationinvitation);
 *
 */
void packet_bnetd_sid_clancreationinvitation(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.clan_tag = packet_get_int(request, ENDIAN_LITTLE);
  packet.inviter_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.status = packet_get_byte(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x72);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clancreationinvitation not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
