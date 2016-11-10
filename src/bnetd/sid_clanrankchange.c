#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x7A
 * Packet Name: sid_clanrankchange
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) Username
 * (BYTE) New rank
 * 
 * 
 * Remarks : 
 * Used by leaders and officers to change the rank of a clan member.
 * 
 * New rank:
 * <dl>
 * <dd>0x00: Initiate</dd>
 * <dd>0x01: Peon</dd>
 * <dd>0x02: Grunt</dd>
 * <dd>0x03: Shaman</dd>
 * <dd>0x04: Chieftain</dd>
 * </dl>
 *
 */
struct request {
  uint32_t cookie;
  char * username;
  uint8_t new_rank;
} __attribute__((packed));


/*
 * Packet ID: 0x7A
 * Packet Name: sid_clanrankchange
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Status
 * 
 * Remarks : 
 * This message returns the result of the clan rank change request.
 * 
 * Result:<dl>
 * <dd>0x00: Successfully changed rank</dd>
 * <dd>0x01: Failed to change rank</dd>
 * <dd>0x02: Cannot change user's rank yet</dd>
 * <dd>0x07: Not authorized to change user rank *</dd>
 * <dd>0x08: Not allowed to change user rank **</dd>
 * </dl>
 * * This will be received when you are not a shaman/chieftain and you are trying
 * to change the rank of another user.
 * 
 * ** This will be received when you are trying to change rank of someone who is
 * your rank or higher such as a chieftain, or you are trying to change the rank of
 * an initiate during their probation period.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7A, packet_bnetd_sid_clanrankchange);
 *
 */
void packet_bnetd_sid_clanrankchange(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);
  packet.new_rank = packet_get_byte(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x7A);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanrankchange not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
