#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x18
 * Packet Name: sid_registry
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) Key Value
 * 
 * 
 * Remarks : 
 * Returns the requested registry value
 *
 */
struct request {
  uint32_t cookie;
  char * key_value;
} __attribute__((packed));


/*
 * Packet ID: 0x18
 * Packet Name: sid_registry
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (DWORD) HKEY
 * (STRING) Registry path
 * (STRING) Registry key
 * 
 * 
 * Remarks : 
 * <dl><dt>HKEY is the registry key root.</dt><dd><dl><dt>The values are
 * standard:</dt>
 * <dd>0x80000000: HKEY_CLASSES_ROOT</dd>
 * <dd>0x80000001: HKEY_CURRENT_USER</dd>
 * <dd>0x80000002: HKEY_LOCAL_MACHINE</dd>
 * <dd>0x80000003: HKEY_USERS</dd>
 * <dd>0x80000004: HKEY_PERFORMANCE_DATA</dd>
 * <dd>0x80000005: HKEY_CURRENT_CONFIG</dd>
 * <dd>0x80000006: HKEY_DYN_DATA</dd></dl></dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x18, packet_bnetd_sid_registry);
 *
 */
void packet_bnetd_sid_registry(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.key_value = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x18);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_registry not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
