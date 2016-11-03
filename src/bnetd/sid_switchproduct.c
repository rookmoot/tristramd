#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x5C
 * Packet Name: sid_switchproduct
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Product ID
 * 
 * Remarks : 
 * When logging onto WAR3, while having W3XP installed on your system; the client
 * sends two CDKey hashes in SID_AUTH_CHECK and uses 'W3XP' as the Product Id in
 * SID_AUTH_INFO. After a successful SID_AUTH_CHECK, the client then sends this
 * packet with the Product ID set to 'WAR3' to make the switch from expansion to
 * non-expansion.
 *
 */
struct request {
  uint32_t product_id;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5C, packet_bnetd_sid_switchproduct);
 *
 */
void packet_bnetd_sid_switchproduct(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x5C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_switchproduct not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
