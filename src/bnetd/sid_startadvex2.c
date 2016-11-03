#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1A
 * Packet Name: sid_startadvex2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Password Protected
 * (DWORD) Unknown
 * (DWORD) Unknown
 * (DWORD) Unknown
 * (DWORD) Unknown
 * (DWORD) Port
 * (STRING) Game name
 * (STRING) Game password
 * (STRING) Unknown
 * (STRING) Game stats - Flags, Creator, Statstring
 * 
 * Remarks : 
 * This message is used by Diablo to create a game. It was also used by Starcraft
 * 1.03.
 *
 */
struct request {
  uint32_t password_protected;
  uint32_t unknown[4];
  uint32_t port;
  char * game_name;
  char * game_password;
  char * unknown2;
  char * game_stats;
} __attribute__((packed));


/*
 * Packet ID: 0x1A
 * Packet Name: sid_startadvex2
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * 
 * Remarks : 
 * This message is used by Diablo to create a game. It was also used by Starcraft
 * 1.03.
 * 
 * <dl><dt>Possible values for Status:</dt>
 * <dd>0x00000000: Failed</dd>
 * <dd>0x00000001: Ok</dd></dl>
 * 
 * Note that Status 0x00000000 and 0x00000001 are in reverse of SID_STARTADVEX3's
 * Status 0x00000000 and 0x00000001.
 * 
 * More info: <a
 * href="https://pastebin.com/WhKsmdnD">https://pastebin.com/WhKsmdnD</a>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1A, packet_bnetd_sid_startadvex2);
 *
 */
void packet_bnetd_sid_startadvex2(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.password_protected = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown[0] = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown[1] = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown[2] = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown[3] = packet_get_int(request, ENDIAN_LITTLE);
  packet.port = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);
  packet.unknown2 = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_stats = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x1A);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_startadvex2 not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
