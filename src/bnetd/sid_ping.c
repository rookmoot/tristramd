#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x25
 * Packet Name: sid_ping
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Ping Value
 * 
 * Remarks : 
 * Ping response. Ping Value is the DWORD received in the server's initial ping
 * message.
 * 
 * The ping displayed when in chat can be artificially inflated by delaying before
 * sending this packet, or deflated by responding before requested.
 * Ping can be set to -1 (Strictly speaking, 0xFFFFFFFF, since ping is unsigned) by
 * not responding to this packet at all.
 * 
 * The DWORD received from the server is not what determines your ping, but it is
 * actually a cookie for the Battle.net server. You should never change the DWORD.
 *
 */
struct request {
  uint32_t ping_value;
} __attribute__((packed));


/*
 * Packet ID: 0x25
 * Packet Name: sid_ping
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Ping Value
 * 
 * Remarks : 
 * Used to calculate Client's ping. The received DWORD should be sent directly back
 * to Battle.net.
 * 
 * The ping displayed when in chat can be artificially inflated by delaying before
 * sending this packet, or deflated by responding before requested.
 * Ping can be set to -1 (Strictly speaking, 0xFFFFFFFF, since ping is unsigned) by
 * not responding to this packet at all.
 * 
 * The received DWORD is not what determines your ping, but it is actually a cookie
 * for the Battle.net server. You should never change the DWORD.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x25, packet_bnetd_sid_ping);
 *
 */
void packet_bnetd_sid_ping(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.ping_value = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x25);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_ping not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
