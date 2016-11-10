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
 * Packet ID: 0x3D
 * Packet Name: sid_createaccount2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) [5] Password hash
 * (STRING) Username
 * 
 * Remarks : 
 * Creates a Battle.net account. Usernames longer than 15 characters are
 * truncated.
 * 
 * Passwords should be converted to lower case before hashing, and are only hashed
 * once (unlike SID_LOGONRESPONSE).
 *
 */
struct request {
  uint32_t password_hash[5];
  char * username;
} __attribute__((packed));


/*
 * Packet ID: 0x3D
 * Packet Name: sid_createaccount2
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (STRING) Account name suggestion
 * 
 * Remarks : 
 * Account creation result.
 * <dl><dt>Result:</dt>
 * <dd>0x00: Account created</dd>
 * <dd>0x01: Name is too short</dd>
 * <dd>0x02: Name contained invalid characters</dd>
 * <dd>0x03: Name contained a banned word</dd>
 * <dd>0x04: Account already exists</dd>
 * <dd>0x05: Account is still being created</dd>
 * <dd>0x06: Name did not contain enough alphanumeric characters</dd>
 * <dd>0x07: Name contained adjacent punctuation characters</dd>
 * <dd>0x08: Name contained too many punctuation characters</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3D, packet_bnetd_sid_createaccount2);
 *
 */
void packet_bnetd_sid_createaccount2(struct connection *conn, struct packet *request) {
  struct request packet;
  char *password;
  struct account *account = NULL;
  struct packet *response = NULL, *response_setemail = NULL;
  
  memset(&packet, 0, sizeof(packet));
  packet.password_hash[0] = packet_get_int(request, ENDIAN_LITTLE);
  packet.password_hash[1] = packet_get_int(request, ENDIAN_LITTLE);
  packet.password_hash[2] = packet_get_int(request, ENDIAN_LITTLE);
  packet.password_hash[3] = packet_get_int(request, ENDIAN_LITTLE);
  packet.password_hash[4] = packet_get_int(request, ENDIAN_LITTLE);
  packet.username = packet_get_string(request, ENDIAN_LITTLE);

  password = (char *)hash_to_str((uint32_t *)packet.password_hash);
  
  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x3D);

  // set response data.
  account = account_new();
  if (account) {
    strncpy(account->username, packet.username, strlen(packet.username));
    strncpy(account->password, password, strlen(password));
  }
  if (sql_insert("accounts", (void **)&account)) {
    packet_set_int(response, ENDIAN_LITTLE, 0x00); // success
  } else {
    packet_set_int(response, ENDIAN_LITTLE, 0x04); // error
  }
  net_connection_response_append(conn, response);

  free(packet.username);
  free(password);
  
  response_setemail = packet_new(PROTOCOL_BNETD);
  if (response_setemail) {
    packet_message_id_set(response_setemail, 0x59);
    net_connection_response_append(conn, response_setemail);
  }
}
