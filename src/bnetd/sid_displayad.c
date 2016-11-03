#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x21
 * Packet Name: sid_displayad
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Platform ID
 * (DWORD) Product ID
 * (DWORD) Ad ID
 * (STRING) Filename
 * (STRING) URL
 * 
 * Remarks : 
 * Sent when an ad is displayed. Perhaps for statistics?
 * 
 * Null strings are now sent in place of Filename and URL, since the need to
 * truncate long strings to 128 characters was causing inaccuracies.
 *
 */
struct request {
  uint32_t platform_id;
  uint32_t product_id;
  uint32_t ad_id;
  char * filename;
  char * url;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x21, packet_bnetd_sid_displayad);
 *
 */
void packet_bnetd_sid_displayad(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.platform_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.ad_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.filename = packet_get_string(request, ENDIAN_LITTLE);
  packet.url = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x21);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_displayad not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
