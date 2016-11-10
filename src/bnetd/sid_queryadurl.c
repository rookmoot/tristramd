#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x41
 * Packet Name: sid_queryadurl
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Ad ID
 * 
 * Remarks : 
 * Requests the URL for an ad if none is given.
 *
 */
struct request {
  uint32_t ad_id;
} __attribute__((packed));


/*
 * Packet ID: 0x41
 * Packet Name: sid_queryadurl
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Ad ID
 * (STRING) Ad URL
 * 
 * Remarks : 
 * Reports the Adbanner's URL. This message will only be received if the client
 * sent it to the server. 
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x41, packet_bnetd_sid_queryadurl);
 *
 */
void packet_bnetd_sid_queryadurl(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.ad_id = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x41);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_queryadurl not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
