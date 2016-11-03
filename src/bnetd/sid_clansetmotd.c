#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x7B
 * Packet Name: sid_clansetmotd
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) MOTD
 * 
 * Remarks : 
 * Sets your clan's Message of the Day.
 *
 */
struct request {
  uint32_t cookie;
  char * motd;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x7B, packet_bnetd_sid_clansetmotd);
 *
 */
void packet_bnetd_sid_clansetmotd(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.motd = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x7B);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clansetmotd not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
