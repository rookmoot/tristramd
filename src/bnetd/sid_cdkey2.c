#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x36
 * Packet Name: sid_cdkey2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Spawn (0/1)
 * (DWORD) Key Length
 * (DWORD) CDKey Product
 * (DWORD) CDKey Value1
 * (DWORD) Server Token
 * (DWORD) Client Token
 * (DWORD) [5] Hashed Data
 * (STRING) Key owner
 * 
 * Remarks : 
 * This packet is an updated version of SID_CDKEY (0x30), designed to prevent
 * CDKeys from being stolen, by sending them hashed instead of plain-text.
 * 
 * The data that should be hashed is:<ol><li>Client Token</li><li>Server
 * Token</li><li>Key Product (from decoded CD key)</li><li>Key Value1 (from decoded
 * CD key)</li><li>Key Value2 (from decoded CD key)</li></ol>
 *
 */
struct request {
  uint32_t spawn;
  uint32_t key_length;
  uint32_t cdkey_product;
  uint32_t cdkey_value;
  uint32_t server_token;
  uint32_t client_token;
  uint32_t hashed_data;
  char * key_owner;
} __attribute__((packed));


/*
 * Packet ID: 0x36
 * Packet Name: sid_cdkey2
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
 * <dd>0x01: OK</dd>
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
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x36, packet_bnetd_sid_cdkey2);
 *
 */
void packet_bnetd_sid_cdkey2(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.spawn = packet_get_int(request, ENDIAN_LITTLE);
  packet.key_length = packet_get_int(request, ENDIAN_LITTLE);
  packet.cdkey_product = packet_get_int(request, ENDIAN_LITTLE);
  packet.cdkey_value = packet_get_int(request, ENDIAN_LITTLE);
  packet.server_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_data = packet_get_int(request, ENDIAN_LITTLE);
  packet.key_owner = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x36);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_cdkey2 not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
