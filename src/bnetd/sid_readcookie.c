#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x24
 * Packet Name: sid_readcookie
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) First DWORD from S -> C
 * (DWORD) Second DWORD from S -> C
 * (STRING) Registry key name
 * (STRING) Registry key value
 * 
 * Remarks : 
 * Much like a website cookie, simply stores some arbitrary string to a 'cookie
 * jar' to save preferences et al. which can be retrieved later by the server.
 * 
 * Not used because it was quickly discovered that storing preferences produces
 * less problems and were faster by storing them server-side, associating them with
 * the account. It is somewhat curious that these packet IDs are close to
 * SID_PROFILE/SID_WRITEPROFILE (0x26 & 0x27).
 * 
 * Found at: battle!190216FBh and battle!1901D660h, respectively.
 *
 */
struct request {
  uint32_t first_dword_from_s___c;
  uint32_t second_dword_from_s___c;
  char * registry_key_name;
  char * registry_key_value;
} __attribute__((packed));


/*
 * Packet ID: 0x24
 * Packet Name: sid_readcookie
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Echoed back, Request ID?
 * (DWORD) Echoed back, Timestamp?
 * (STRING) Registry key name
 * 
 * Remarks : 
 * Much like a website cookie, simply stores some arbitrary string to a 'cookie
 * jar' to save preferences et al. which can be retrieved later by the server.
 * 
 * Not used because it was quickly discovered that storing preferences produces
 * less problems and were faster by storing them server-side, associating them with
 * the account. It is somewhat curious that these packet IDs are close to
 * SID_PROFILE/SID_WRITEPROFILE (0x26 & 0x27).
 * 
 * Found at: battle!190216FBh and battle!1901D660h, respectively.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x24, packet_bnetd_sid_readcookie);
 *
 */
void packet_bnetd_sid_readcookie(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.first_dword_from_s___c = packet_get_int(request, ENDIAN_LITTLE);
  packet.second_dword_from_s___c = packet_get_int(request, ENDIAN_LITTLE);
  packet.registry_key_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.registry_key_value = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x24);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_readcookie not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
