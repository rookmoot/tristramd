#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x08
 * Packet Name: sid_startadvex
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) State
 * (DWORD) Time since creation
 * (WORD) Game Type
 * (WORD) Parameter
 * (DWORD) Unknown (0x00)
 * (DWORD) Unknown (Likely ladder, but will always be 0x00 because there is no SSHR
 * ladder)
 * (STRING) Game name
 * (STRING) Game password
 * (STRING) Game Statstring
 * 
 * Remarks : 
 * Creates a game in a manner similar to SID_STARTADVEX2 and SID_STARTADVEX3. This
 * is only used by Starcraft Shareware.
 * See documentation on SID_STARTADVEX3 for details on the values and their
 * meanings.
 *
 */
struct request {
  uint32_t state;
  uint32_t time_since_creation;
  uint16_t game_type;
  uint16_t parameter;
  uint32_t unknown;
  uint32_t unknown_1836101414;
  char * game_name;
  char * game_password;
  char * game_statstring;
} __attribute__((packed));


/*
 * Packet ID: 0x08
 * Packet Name: sid_startadvex
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * 
 * Remarks : 
 * Status
 * <dl><dd>0x00: Failed</dd>
 * <dd>0x01: Success</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x08, packet_bnetd_sid_startadvex);
 *
 */
void packet_bnetd_sid_startadvex(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.state = packet_get_int(request, ENDIAN_LITTLE);
  packet.time_since_creation = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_type = packet_get_short(request, ENDIAN_LITTLE);
  packet.parameter = packet_get_short(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_statstring = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x08);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_startadvex not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
