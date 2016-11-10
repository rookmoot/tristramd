#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"
#include "hashing.h"
#include "account.h"

/*
 * Packet ID: 0x31
 * Packet Name: sid_changepassword
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Client Token
 * (DWORD) Server Token
 * (DWORD) [5] Old password hash
 * (DWORD) [5] New password hash
 * (STRING) Account name
 * 
 * Remarks : 
 * Changes Battle.net account password. This message must be sent before you
 * logon.
 * 
 * Passwords should be converted to lower case before hashing.
 *
 */
struct request {
  uint32_t client_token;
  uint32_t server_token;
  uint32_t old_password_hash[5];
  uint32_t new_password_hash[5];
  char *username;
} __attribute__((packed));


/*
 * Packet ID: 0x31
 * Packet Name: sid_changepassword
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Reports whether the password was changed or not. This is used for the old logon
 * system.
 * 
 * <dl>
 * <dd>0x00: Success</dd>
 * <dd>0x01: Failure</dd>
 * </dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x31, packet_bnetd_sid_changepassword);
 *
 */
void packet_bnetd_sid_changepassword(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct account *account = NULL;
  struct request packet;
  char *new = NULL;
  
  memset(&packet, 0, sizeof(packet));
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.server_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.old_password_hash[0] = packet_get_int(request, ENDIAN_LITTLE);
  packet.old_password_hash[1] = packet_get_int(request, ENDIAN_LITTLE);
  packet.old_password_hash[2] = packet_get_int(request, ENDIAN_LITTLE);
  packet.old_password_hash[3] = packet_get_int(request, ENDIAN_LITTLE);
  packet.old_password_hash[4] = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_password_hash[0] = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_password_hash[1] = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_password_hash[2] = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_password_hash[3] = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_password_hash[4] = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }
  // set response message id.
  packet_message_id_set(response, 0x31);

  account = (struct account *)sql_select_one("accounts", "username='%s'", packet.username);
  if (account) {
    uint32_t password[5];
    hash_new(account->password, (uint32_t *)password);

    // before settings new password.
    // we simulate the user login with the old password hash.
    // if the login in is succesful. then we change the password.
    if (hashing_check_password(password, packet.old_password_hash, packet.client_token, packet.server_token) == 1) {
      // success
      new = (char *)hash_to_str((uint32_t *)packet.new_password_hash);
      if (new) {
	strncpy(account->password, new, strlen(new));
	if (sql_update("accounts", (void *)account)) {
	  packet_set_int(response, ENDIAN_LITTLE, 0x00); // success
	} else {
	  packet_set_int(response, ENDIAN_LITTLE, 0x01); // error
	}
	free(new);
      } else {
	packet_set_int(response, ENDIAN_LITTLE, 0x01); // error
      }
    } else {
      packet_set_int(response, ENDIAN_LITTLE, 0x01); // error
    }
  } else {
    packet_set_int(response, ENDIAN_LITTLE, 0x01); // error
  }
  
  free(packet.username);
  net_connection_response_append(conn, response);
}
