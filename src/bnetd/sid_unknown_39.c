#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x39
 * Packet Name: sid_unknown_39
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Character Name
 * 
 * Remarks : 
 * Not much is known about this packet. The structure of this packet and its
 * documentation was taken from PvPGN. The collected information can be found <a
 * href="https://pastebin.com/eGgRBcLV">here</a>.
 *
 */
struct request {
  char * character_name;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x39, packet_bnetd_sid_unknown_39);
 *
 */
void packet_bnetd_sid_unknown_39(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.character_name = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x39);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_unknown_39 not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
