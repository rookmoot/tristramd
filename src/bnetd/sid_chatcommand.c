#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x0E
 * Packet Name: sid_chatcommand
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Text *
 * 
 * Remarks : 
 * Send text or a command to Battle.net using this packet.
 * 
 * For STAR/SEXP/SSHR/JSTR, Text is UTF-8 encoded (WIDESTRING).
 * 
 * It is generally accepted as unwise to send any character below a space (0x20):
 * this includes line feeds, carriage returns & control characters. The maximum
 * number of characters is 224 per message including the null-terminator (so really
 * only 223 characters), any longer and it becomes trimmed by Battle.net.
 * 
 * If you send a line feed and/or a carriage return, Battle.net disconnects you and
 * IP bans you for 5 minutes.
 *
 */
struct request {
  char * text;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0E, packet_bnetd_sid_chatcommand);
 *
 */
void packet_bnetd_sid_chatcommand(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.text = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x0E);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_chatcommand not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
