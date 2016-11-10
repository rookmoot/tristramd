#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1E
 * Packet Name: sid_clientid2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Server Version<dl><dt>For server version 1:</dt>
 * <dd>(DWORD) Registration Version</dd>
 * <dd>(DWORD) Registration Authority</dd>
 * <dt>For server version 0:</dt>
 * <dd>(DWORD) Registration Authority</dd>
 * <dd>(DWORD) Registration Version</dd></dl>
 * (DWORD) Account Number
 * (DWORD) Registration Token
 * (STRING) LAN computer name
 * (STRING) LAN username
 * 
 * Remarks : 
 * See related link for more information.
 *
 */
struct request {
  uint32_t server_versionfor_server_version;
  uint32_t registration_version;
  uint32_t registration_authority;
  uint32_t registration_authority_1173140765;
  uint32_t registration_version_1692310628;
  uint32_t account_number;
  uint32_t registration_token;
  char * lan_computer_name;
  char * lan_username;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1E, packet_bnetd_sid_clientid2);
 *
 */
void packet_bnetd_sid_clientid2(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.server_versionfor_server_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_authority = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_authority = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.account_number = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.lan_computer_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.lan_username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x1E);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clientid2 not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
