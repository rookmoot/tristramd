#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x34
 * Packet Name: sid_queryrealms
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Unused (0)
 * (DWORD) Unused (0)
 * (STRING) Unknown (empty)
 * 
 * Remarks : 
 * Requests a realm listing.
 * 
 * This packet is no longer used. SID_QUERYREALMS2 is used instead.
 *
 */
struct request {
  uint32_t unused;
  uint32_t unused_1098826796;
  char * unknown;
} __attribute__((packed));


/*
 * Packet ID: 0x34
 * Packet Name: sid_queryrealms
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Unknown
 * (DWORD) Count
 * <dl><dt>For Each Realm:</dt>
 * <dd>(DWORD) Unknown</dd>
 * <dd>(STRING) Realm title</dd>
 * <dd>(STRING) Realm description</dd></dl>
 * 
 * Remarks : 
 * Realm list.
 * 
 * The first Unknown is usually 0. The second [and subsequent] Unknown is usually
 * 1.
 * 
 * This packet is no longer used. SID_QUERYREALMS2 is used instead.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x34, packet_bnetd_sid_queryrealms);
 *
 */
void packet_bnetd_sid_queryrealms(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.unused = packet_get_int(request, ENDIAN_LITTLE);
  packet.unused = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x34);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_queryrealms not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
