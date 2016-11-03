#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x42
 * Packet Name: sid_cdkey3
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Unknown 1
 * (DWORD) Unknown 2
 * (DWORD) Unknown 3
 * (DWORD) Unknown 4
 * (DWORD) Unknown 5
 * (DWORD) Unknown 6
 * (DWORD) Unknown 7
 * (DWORD) [5] CD-Key Hash
 * (STRING) CD-Key Owner
 * 
 * Remarks : 
 * Not much is known about this packet. The structure of this packet and its
 * documentation was taken from PvPGN. The collected information can be found <a
 * href="http://pastebin.com/CyDV2pC2">here</a>.
 *
 */
struct request {
  uint32_t unknown;
  uint32_t unknown_1115793027;
  uint32_t unknown_1738049240;
  uint32_t unknown_1005546844;
  uint32_t unknown_1216292607;
  uint32_t unknown_1332136175;
  uint32_t unknown_96483087;
  uint32_t cd_key_hash;
  char * cd_key_owner;
} __attribute__((packed));


/*
 * Packet ID: 0x42
 * Packet Name: sid_cdkey3
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (STRING) CD-Key Owner or Error Message
 * 
 * Remarks : 
 * Not much is known about this packet. The structure of this packet and its
 * documentation was taken from PvPGN. The collected information can be found <a
 * href="http://pastebin.com/CyDV2pC2">here</a>.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x42, packet_bnetd_sid_cdkey3);
 *
 */
void packet_bnetd_sid_cdkey3(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.cd_key_hash = packet_get_int(request, ENDIAN_LITTLE);
  packet.cd_key_owner = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x42);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_cdkey3 not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
