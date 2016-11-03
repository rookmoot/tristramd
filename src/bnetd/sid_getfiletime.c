#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x33
 * Packet Name: sid_getfiletime
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Request ID
 * (DWORD) Unknown
 * (STRING) Filename
 * 
 * Remarks : 
 * This packet seems to request the current filetime for the specified file.
 * Purpose of first 2 DWORDs is unknown, however, both are echoed back to the
 * client by Battle.net and do not seem to affect the reply. Because of this it is
 * reasonable to assume that first DWORD at least is a request ID of some kind.
 * This is called into question, however, by the fact that the replying packet also
 * contains the requested filename. The game (STAR/SEXP) always sends the same
 * number in DWORD 1 for the file in question. DWORD 2 seems to be null.
 * 
 * <dl><dt>Known codes for DWORD 1:</dt>
 * <dd>0x01: tos_usa.txt
 * 0x03: bnserver-WAR3.ini
 * 0x1A: tos_USA.txt
 * 0x1B: bnserver.ini
 * 0x1D: icons_STAR.bni
 * 0x80000006: (0x4C) IX86ExtraWork.mpq
 * 0x80000005: (0x4A) IX86ExtraWork.mpq
 * 0x80000004: bnserver-D2DV.ini</dd>
 *
 */
struct request {
  uint32_t request_id;
  uint32_t unknown;
  char * filename;
} __attribute__((packed));


/*
 * Packet ID: 0x33
 * Packet Name: sid_getfiletime
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Request ID
 * (DWORD) Unknown
 * (FILETIME) Last update time
 * (STRING) Filename
 * 
 * Remarks : 
 * Contains the latest filetime for the requested file. If the file does not exist,
 * the returned filetime is null.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x33, packet_bnetd_sid_getfiletime);
 *
 */
void packet_bnetd_sid_getfiletime(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.filename = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x33);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_getfiletime not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
