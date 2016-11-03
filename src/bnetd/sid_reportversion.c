#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x07
 * Packet Name: sid_reportversion
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Platform ID
 * (DWORD) Product ID
 * (DWORD) Version Byte
 * (DWORD) EXE Version
 * (DWORD) EXE Hash
 * (STRING) EXE Information
 * 
 * Remarks : 
 * Contains CheckRevision response, version & EXE info.
 *
 */
struct request {
  uint32_t platform_id;
  uint32_t product_id;
  uint32_t version_byte;
  uint32_t exe_version;
  uint32_t exe_hash;
  char * exe_information;
} __attribute__((packed));


/*
 * Packet ID: 0x07
 * Packet Name: sid_reportversion
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * (STRING) Patch path
 * 
 * 
 * Remarks : 
 * Reports success/failure on challenge.
 * 
 * Result:
 * <dl><dd>0: Failed version check</dd>
 * <dd>1: Old game version</dd>
 * <dd>2: Success</dd>
 * <dd>3: Reinstall required</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x07, packet_bnetd_sid_reportversion);
 *
 */
void packet_bnetd_sid_reportversion(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.platform_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.version_byte = packet_get_int(request, ENDIAN_LITTLE);
  packet.exe_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.exe_hash = packet_get_int(request, ENDIAN_LITTLE);
  packet.exe_information = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x07);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_reportversion not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
