#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x5D
 * Packet Name: sid_reportcrash
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Version? (0x010A0027)
 * (DWORD) Exception code
 * (DWORD) Unknown
 * (DWORD) Unknown
 * 
 * Remarks : 
 * When the game crashes, (usually) a file named Crashdump is created. If this file
 * exists at the next logon, the contents of it are sent to Battle.net in this
 * message.
 * 
 * The first DWORD for Diablo II is a constant value (version?), as seen in
 * Fog.dll.
 * 
 * All calls to Fog_10052 result in the output of Crashdump.
 * 
 * More research is required.
 *
 */
struct request {
  uint32_t version;
  uint32_t exception_code;
  uint32_t unknown;
  uint32_t unknown_1870984070;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5D, packet_bnetd_sid_reportcrash);
 *
 */
void packet_bnetd_sid_reportcrash(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.version = packet_get_int(request, ENDIAN_LITTLE);
  packet.exception_code = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x5D);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_reportcrash not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
