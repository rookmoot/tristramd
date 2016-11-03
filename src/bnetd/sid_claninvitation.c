#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x77
 * Packet Name: sid_claninvitation
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) Target User
 * 
 * Remarks : 
 * This message is used when a leader or officer invites a user to join their clan.
 *
 */
struct request {
  uint32_t cookie;
  char * target_user;
} __attribute__((packed));


/*
 * Packet ID: 0x77
 * Packet Name: sid_claninvitation
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Result
 * 
 * Remarks : 
 * This packet is sent to inform the client of the reply to an invitation.
 * 
 * Result:<dl>
 * <dd>0x00: Invitation accepted</dd>
 * <dd>0x04: Invitation declined</dd>
 * <dd>0x05: Failed to invite user</d>
 * <dd>0x09: Clan is full</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x77, packet_bnetd_sid_claninvitation);
 *
 */
void packet_bnetd_sid_claninvitation(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.target_user = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x77);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_claninvitation not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
