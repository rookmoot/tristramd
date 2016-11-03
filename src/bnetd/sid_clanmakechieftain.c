#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x74
 * Packet Name: sid_clanmakechieftain
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) New Cheiftain
 * 
 * Remarks : 
 * Changes the clan's chieftain.
 *
 */
struct request {
  uint32_t cookie;
  char * new_cheiftain;
} __attribute__((packed));


/*
 * Packet ID: 0x74
 * Packet Name: sid_clanmakechieftain
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Status
 * 
 * Remarks : 
 * Notifies the sending client of the success/failure of its request.
 * Status:<dl>
 * <dd>0x00: Success</dd>
 * <dd>0x02: Cannot change until clan is a week old</dd>
 * <dd>0x04: Declined</dd>
 * <dd>0x05: Failed</dd>
 * <dd>0x07: Not Authorized</dd>
 * <dd>0x08: Not Allowed</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x74, packet_bnetd_sid_clanmakechieftain);
 *
 */
void packet_bnetd_sid_clanmakechieftain(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_cheiftain = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x74);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanmakechieftain not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
