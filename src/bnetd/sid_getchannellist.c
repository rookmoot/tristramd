#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "account.h"
#include "chat.h"


static Eina_Bool packet_bnetd_sid_getchannellist_foreach_cb(const Eina_Hash *hash EINA_UNUSED, const void *key, void *data EINA_UNUSED, void *fdata);


/*
 * Packet ID: 0x0B
 * Packet Name: sid_getchannellist
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Product ID
 * 
 * Remarks : 
 * Requests a list of channels that the client is permitted to enter.
 * 
 * In the past this packet returned a product list for the specified Product ID,
 * however, the Product ID field is now ignored -- it does not need to be a valid
 * Product ID, and can be set to zero. The list of channels returned will be for
 * the client's product, as specified during the client's logon.
 *
 */
struct request {
  uint32_t product_id;
} __attribute__((packed));


/*
 * Packet ID: 0x0B
 * Packet Name: sid_getchannellist
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) [] Channel names, each terminated by a null string.
 * 
 * Remarks : 
 * Contains a list of available channels.
 * 
 * For some reason, Diablo II adds extra data to the channel list (as can be seen
 * in game), and older clients list blocked channels, such as Warez and Backstage.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0B, packet_bnetd_sid_getchannellist);
 *
 */
void packet_bnetd_sid_getchannellist(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;
  
  memset(&packet, 0, sizeof(packet));
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);


  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x0B);
  eina_hash_foreach(chat_channel_list_get(), packet_bnetd_sid_getchannellist_foreach_cb, response);
  
  net_connection_response_append(conn, response);
}


static Eina_Bool packet_bnetd_sid_getchannellist_foreach_cb(const Eina_Hash *hash EINA_UNUSED, const void *key, void *data EINA_UNUSED, void *fdata) {
  struct packet *response = (struct packet *)fdata;
  if (response) {
    packet_set_string(response, ENDIAN_LITTLE, (const char *)key);
  }
  return EINA_TRUE;
}
