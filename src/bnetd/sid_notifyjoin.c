#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x22
 * Packet Name: sid_notifyjoin
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Product ID *
 * (DWORD) Product version
 * (STRING) Game Name
 * (STRING) Game Password
 * 
 * Remarks : 
 * Notifies Battle.net that the client has joined a game. This is what causes you
 * to receive "Your friend _ entered a _ game called _." from Battle.net if you are
 * mutual friends with this client.
 * 
 * SID_LEAVECHAT (0x10) does not need to be sent after this, since this does what
 * LEAVECHAT does but with an added notification.
 * 
 * * This can be any valid Product ID, even if you are not connected with that ID.
 *
 */
struct request {
  uint32_t product_id;
  uint32_t product_version;
  char * game_name;
  char * game_password;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x22, packet_bnetd_sid_notifyjoin);
 *
 */
void packet_bnetd_sid_notifyjoin(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x22);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_notifyjoin not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
