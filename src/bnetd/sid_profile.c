#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x35
 * Packet Name: sid_profile
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (STRING) Username
 * 
 * Remarks : 
 * This requests a profile for a user. This message's official name is not known,
 * and has been invented.
 * 
 * It is safe to look up any username you want. The username can have illegal
 * characters, can be of any length, and can have all the other "illy" username
 * characteristics. This message can also be used across all products without a
 * problem, not just WAR3/W3XP.
 * 
 * Battle.net will translate the realm in the username for you, so you can look up
 * @Azeroth as well as @USEast, if you want. You cannot perform cross-realm look
 * ups, however; that is to say you cannot look up @USWest if you're on @USEast and
 * likewise for the other realms.
 * 
 * If you look up a user that is not on their Warcraft III user table, then the
 * clan tag in the replying message will always be null. But it is safe to do this
 * as well. The profile description and location strings will match that of the
 * user you look up, regardless of which internal user table the user resides on.
 *
 */
struct request {
  uint32_t cookie;
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x35
 * Packet Name: sid_profile
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Cookie
 * (BYTE) Success
 * (STRING) Profile\Description value
 * (STRING) Profile\Location value
 * (DWORD) Clan Tag
 * 
 * Remarks : 
 * If the status code is 0, the request was successful; otherwise, the lookup
 * failed and the message length will be five bytes long (not counting the four
 * byte header).
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x35, packet_bnetd_sid_profile);
 *
 */
void packet_bnetd_sid_profile(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.cookie = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x35);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_profile not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
