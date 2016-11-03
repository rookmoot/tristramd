#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x16
 * Packet Name: sid_clickad
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Ad ID
 * (DWORD) Request type
 * 
 * Remarks : 
 * The clients send this when an ad is clicked.
 * 
 * Request Type is 0 if the client used SID_QUERYADURL to get the ad's data, 1
 * otherwise. 
 *
 */
struct request {
  uint32_t ad_id;
  uint32_t request_type;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x16, packet_bnetd_sid_clickad);
 *
 */
void packet_bnetd_sid_clickad(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.ad_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.request_type = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x16);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clickad not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
