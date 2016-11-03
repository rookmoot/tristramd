#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x30
 * Packet Name: sid_cdkey
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Spawn (0/1)
 * (STRING) CDKey
 * (STRING) Key Owner
 * 
 * Remarks : 
 * Contains unhashed CD key information.
 *
 */
struct request {
  uint32_t spawn;
  char * cdkey;
  char * key_owner;
} __attribute__((packed));


/*
 * Packet ID: 0x30
 * Packet Name: sid_cdkey
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * (STRING) Key owner
 * 
 * Remarks : 
 * This packet is identical to SID_CDKEY (0x30)
 * 
 * <dl><dt>Possible values for Result:</dt>
 * <dd>0x01: Ok
 * <dd>0x02: Invalid key</dd>
 * <dd>0x03: Bad product</dd>
 * <dd>0x04: Banned</dd>
 * <dd>0x05: In use</dd></dl>
 * 
 * In addition, the Key Owner field has two special values:
 * <ul><li>'TOO MANY SPAWNS': Exceeded spawn limit for this CD-Key</li>
 * <li>'NO SPAWNING': Spawns are not supported for this CD-Key</li></ul>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x30, packet_bnetd_sid_cdkey);
 *
 */
void packet_bnetd_sid_cdkey(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.spawn = packet_get_int(request, ENDIAN_LITTLE);
  packet.cdkey = packet_get_string(request, ENDIAN_LITTLE);
  packet.key_owner = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x30);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_cdkey not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
