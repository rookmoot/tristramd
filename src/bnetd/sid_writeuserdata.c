#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x27
 * Packet Name: sid_writeuserdata
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of accounts
 * (DWORD) Number of keys
 * (STRING) [] Accounts to update
 * (STRING) [] Keys to update
 * (STRING) [] New values
 * 
 * Remarks : 
 * Updates the Client's profile.
 * Currently, the allowed keys for this are Sex, Location, and Description. The
 * maximum length for the values is 512, including the null terminator.
 * 
 * See SID_READUSERDATA for more information.
 *
 */
struct request {
  uint32_t number_of_accounts;
  uint32_t number_of_keys;
  char * accounts_to_update;
  char * keys_to_update;
  char * new_values;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x27, packet_bnetd_sid_writeuserdata);
 *
 */
void packet_bnetd_sid_writeuserdata(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.number_of_accounts = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_keys = packet_get_int(request, ENDIAN_LITTLE);
  packet.accounts_to_update = packet_get_string(request, ENDIAN_LITTLE);
  packet.keys_to_update = packet_get_string(request, ENDIAN_LITTLE);
  packet.new_values = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x27);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_writeuserdata not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
