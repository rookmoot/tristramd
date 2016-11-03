#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x73
 * Packet Name: sid_clandisband
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * 
 * Remarks : 
 * Disbands the clan of which the client is a member. You must be a leader to send
 * this packet.
 *
 */
struct request {
  uint32_t cookie;
} __attribute__((packed));


/*
 * Packet ID: 0x73
 * Packet Name: sid_clandisband
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Result
 * 
 * Remarks : 
 * Result:<dl>
 * <dd>0x00: Successfully disbanded the clan</dd>
 * <dd>0x02: Cannot quit clan, not one week old yet</dd>
 * <dd>0x07: Not authorized to disband the clan</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x73, packet_bnetd_sid_clandisband);
 *
 */
void packet_bnetd_sid_clandisband(struct connection *conn, struct packet *request) {
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
  packet_message_id_set(response, 0x73);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clandisband not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
