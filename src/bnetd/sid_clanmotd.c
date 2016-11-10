#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x7C
 * Packet Name: sid_clanmotd
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * 
 * Remarks : 
 * Requests the clan's MOTD.
 *
 */
struct request {
  uint32_t cookie;
} __attribute__((packed));


/*
 * Packet ID: 0x7C
 * Packet Name: sid_clanmotd
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (DWORD) Unknown (0)
 * (STRING) MOTD
 * 
 * Remarks : 
 * This message contains the clan's Message of the Day.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7C, packet_bnetd_sid_clanmotd);
 *
 */
void packet_bnetd_sid_clanmotd(struct connection *conn, struct packet *request) {
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
  packet_message_id_set(response, 0x7C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clanmotd not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
