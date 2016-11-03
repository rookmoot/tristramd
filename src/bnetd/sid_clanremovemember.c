#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x78
 * Packet Name: sid_clanremovemember
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) Username
 * 
 * Remarks : 
 * Kick a member out of the clan. Only clan leaders and officers may perform this
 * action successfully. Members can only be removed if they've been in the clan for
 * over one week.
 *
 */
struct request {
  uint32_t cookie;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x78
 * Packet Name: sid_clanremovemember
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Status
 * 
 * 
 * Remarks : 
 * Response when removing a member from your clan.
 * 
 * Status constants:<dl>
 * <dd>0x00: Removed</dd>
 * <dd>0x01: Removal failed</dd>
 * <dd>0x02: Cannot be removed yet</dd>
 * <dd>0x07: Not authorized to remove</dd>
 * <dd>0x08: Not allowed to remove</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x78, packet_bnetd_sid_clanremovemember);
 *
 */
void packet_bnetd_sid_clanremovemember(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x78);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanremovemember not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
