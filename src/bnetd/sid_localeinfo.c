#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x12
 * Packet Name: sid_localeinfo
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (FILETIME) System time
 * (FILETIME) Local time
 * (DWORD) Timezone bias
 * (DWORD) SystemDefaultLCID
 * (DWORD) UserDefaultLCID
 * (DWORD) UserDefaultLangID
 * (STRING) Abbreviated language name
 * (STRING) Country name
 * (STRING) Abbreviated country name
 * (STRING) Country (English)
 * 
 * Remarks : 
 * Informs the server of the client's locale information. Much of this
 * functionality has been incorporated into SID_AUTH_INFO, and more in-depth
 * remarks can be found there. 
 *
 */
struct request {
  uint64_t system_time;
  uint64_t local_time;
  uint32_t timezone_bias;
  uint32_t systemdefaultlcid;
  uint32_t userdefaultlcid;
  uint32_t userdefaultlangid;
  char * abbreviated_language_name;
  char * country_name;
  char * abbreviated_country_name;
  char * country;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x12, packet_bnetd_sid_localeinfo);
 *
 */
void packet_bnetd_sid_localeinfo(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.system_time = packet_get_long(request, ENDIAN_LITTLE);
  packet.local_time = packet_get_long(request, ENDIAN_LITTLE);
  packet.timezone_bias = packet_get_int(request, ENDIAN_LITTLE);
  packet.systemdefaultlcid = packet_get_int(request, ENDIAN_LITTLE);
  packet.userdefaultlcid = packet_get_int(request, ENDIAN_LITTLE);
  packet.userdefaultlangid = packet_get_int(request, ENDIAN_LITTLE);
  packet.abbreviated_language_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.country_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.abbreviated_country_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.country = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x12);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_localeinfo not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
