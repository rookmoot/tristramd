#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "file.h"
#include "ad.h"

/*
 * Packet ID: 0x15
 * Packet Name: sid_checkad
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Platform ID
 * (DWORD) Product ID
 * (DWORD) ID of last displayed banner
 * (DWORD) Current time
 * 
 * Remarks : 
 * Requests ad banner information from Battle.net.
 * 
 * Official clients send this packet every 20 seconds (give or take). You do not
 * have to send this at all.
 * 
 * In addition, Battle.net will not reply unless any of the fields above are
 * updated from the last time you checked. If you have never checked before,
 * Battle.net will always reply back.
 *
 */
struct request {
  uint32_t platform_id;
  uint32_t product_id;
  uint32_t id_of_last_displayed_banner;
  uint32_t current_time;
} __attribute__((packed));


/*
 * Packet ID: 0x15
 * Packet Name: sid_checkad
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Ad ID
 * (DWORD) File extension
 * (FILETIME) Local file time
 * (STRING) Filename
 * (STRING) Link URL
 * 
 * Remarks : 
 * Contains information needed to download and display an ad banner.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x15, packet_bnetd_sid_checkad);
 *
 */
void packet_bnetd_sid_checkad(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;
  
  struct ad *ad = NULL;
  struct file *file = NULL;
  Eina_List *ads = NULL, *l = NULL;

  char *extension = "xcp.";
  uint32_t extension_tag;

  // TODO: remove me to enable
  return;
  
  memcpy(&extension_tag, extension, 4);
  
  memset(&packet, 0, sizeof(packet));
  packet.platform_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.id_of_last_displayed_banner = packet_get_int(request, ENDIAN_LITTLE);
  packet.current_time = packet_get_int(request, ENDIAN_LITTLE);

  ads = sql_select("ads", "filename<>'' ORDER BY id DESC LIMIT 1");
  EINA_LIST_FOREACH(ads, l, ad) {
    file = file_new(ad->filename);
  }

  if (file == NULL) {
    return;
  }
  
  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x15);
  EINA_LIST_FOREACH(ads, l, ad) {
    packet_set_int(response, ENDIAN_LITTLE, (uint32_t)ad->id);
    packet_set_int(response, ENDIAN_LITTLE, extension_tag);
    packet_set_long(response, ENDIAN_LITTLE, file_time_get(file));
    packet_set_string(response, ENDIAN_LITTLE, (const char *)ad->filename);
    packet_set_string(response, ENDIAN_LITTLE, (const char *)ad->url);
  }

  file_free(file);

  net_connection_response_append(conn, response);
}
