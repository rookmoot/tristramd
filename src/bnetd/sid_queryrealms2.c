#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "tristram.h"

/*
 * Packet ID: 0x40
 * Packet Name: sid_queryrealms2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Requests a realm listing.
 *
 */


/*
 * Packet ID: 0x40
 * Packet Name: sid_queryrealms2
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
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x40, packet_bnetd_sid_queryrealms2);
 *
 */
void packet_bnetd_sid_queryrealms2(struct connection *conn, struct packet *request) {
  char *name;
  char *description;
  struct packet *response = NULL;

  CONFIG_GET_STRING("servers.realm.name", &name);
  CONFIG_GET_STRING("servers.realm.description", &description);

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x40);

  packet_set_int(response, ENDIAN_LITTLE, 0);
  packet_set_int(response, ENDIAN_LITTLE, 1);
  packet_set_int(response, ENDIAN_LITTLE, 1);
  packet_set_string(response, ENDIAN_LITTLE, name);
  packet_set_string(response, ENDIAN_LITTLE, description);

  net_connection_response_append(conn, response);
}
