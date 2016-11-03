#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x14
 * Packet Name: d2gs_overheadmessage
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Unknown - 0x00, 0x00
 * (STRING) Message
 * (BYTE) Unused - 0x00
 * (WORD) Unknown - 0x00, 0x00
 * 
 * 
 * Remarks : 
 * This message is used when you'd like to put a message above a character's head
 * as used in the client as "![message]".
 * 
 * Restrictions: Total size of packet cannot be greater than 275 bytes. Message
 * cannot be greater than 255 bytes. 
 * 
 * Please note: This message's official name is not known, and has been invented.
 * 
 * Extra Info: The status of this information is: Public Colors can be created by
 * adding hex FF 63 and a character 30 to 3C. 
 * 
 * Example:
 * To make red, use FF 63 31.
 *
 */
struct request {
  uint16_t unknown;
  char * message;
  uint8_t unused___x;
  uint16_t unknown_2043893920;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x14, packet_d2gs_d2gs_overheadmessage);
 *
 */
void packet_d2gs_d2gs_overheadmessage(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.unknown = packet_get_short(request, ENDIAN_LITTLE);
  packet.message = packet_get_string(request, ENDIAN_LITTLE);
  packet.unused___x = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x14);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_overheadmessage not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
