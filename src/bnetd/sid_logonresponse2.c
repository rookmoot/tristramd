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
 * Packet ID: 0x3A
 * Packet Name: sid_logonresponse2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Client Token
 * (DWORD) Server Token
 * (DWORD) [5] Password Hash
 * (STRING) Username
 * 
 * Remarks : 
 * This packet is the same as SID_LOGONRESPONSE, but has additional response codes.
 *
 */
struct request {
  uint32_t client_token;
  uint32_t server_token;
  uint32_t password_hash[5];
  char * username;
} __attribute__((packed));

/*
int packet_bnetd_sid_logonresponse2_check_password(const uint32_t *pwhash, const void *claimhash, uint32_t ctoken, uint32_t stoken)  {
  char tmpbuf[28], result[20];
  char pw1[41];
  char pw2[41];
  char pw3[41];

  uint32_t *hash = (uint32_t *)claimhash;
  
  memset(tmpbuf, 0, 28);
  memset(result, 0, 20);

  memset(pw1, 0, 41);
  memset(pw2, 0, 41);
  memset(pw3, 0, 41);
  
  *(uint32_t *)tmpbuf       = ctoken;
  *(uint32_t *)(tmpbuf + 4) = stoken;
  memcpy(tmpbuf + 8, pwhash, 20);
  BSHA1(tmpbuf, 28, (uint32_t *)result);

  snprintf(pw1, 41, "%08x%08x%08x%08x%08x", (uint32_t)pwhash[0], (uint32_t)pwhash[1], (uint32_t)pwhash[2], (uint32_t)pwhash[3], (uint32_t)pwhash[4]);
  snprintf(pw2, 41, "%08x%08x%08x%08x%08x", (uint32_t)result[0], (uint32_t)result[1], (uint32_t)result[2], (uint32_t)result[3], (uint32_t)result[4]);
  snprintf(pw3, 41, "%08x%08x%08x%08x%08x", hash[0], hash[1], hash[2], hash[3], hash[4]);  
  printf("CMP:  real:%s claimed:%s result:%s\n", pw1, pw3, pw2);
  
  return memcmp(result, claimhash, 40);
}
*/
/*
 * Packet ID: 0x3A
 * Packet Name: sid_logonresponse2
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (STRING) Additional information
 * 
 * Remarks : 
 * <dl><dt>Status:</dt>
 * <dd>0x00: Success.</dd>
 * <dd>0x01: Account does not exist.</dd>
 * <dd>0x02: Invalid password.</dd>
 * <dd>0x06: Account closed.</dd></dl>
 * 
 * The string containing the reason is only present when your account is closed. If
 * not, the entire field is omitted including the null-terminator.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3A, packet_bnetd_sid_logonresponse2);
 *
 */
void packet_bnetd_sid_logonresponse2(struct connection *conn, struct packet *request) {
  int i = 0;
  int need_set_email = 0;
  struct request packet;
  struct packet *response = NULL, *response_setemail = NULL;
  struct account *account = NULL;
  
  memset(&packet, 0, sizeof(packet));
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.server_token = packet_get_int(request, ENDIAN_LITTLE);
  for (i = 0; i < 5; i++) {
    packet.password_hash[i] = packet_get_int(request, ENDIAN_LITTLE);
  }
  packet.username = packet_get_string(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x3A);  
  
  // login user
  account = (struct account *)sql_select_one("accounts", "username='%s'", packet.username);
  if (account) {
    account_connection_set(account, conn);
    if (!strlen(account->mail)) {
      need_set_email = 1;
    }
    
    EINA_LOG_INFO("Trying to login with account %s (id: %ld, blocked : %d)", account->username, account->id, account->is_blocked);
    
    uint32_t password[5];
    hash_new(account->password, (uint32_t *)password);

    // check if account is blocked.
    if (account->is_blocked == 1) {
      packet_set_int(response, ENDIAN_LITTLE, 0x06);
      packet_set_string(response, ENDIAN_LITTLE, "Your account has been blocked by an admin.");
      free(packet.username);
      connection_response_append(conn, response);
      return;
    }

    // check user password.
    if (hashing_check_password(password, packet.password_hash, packet.client_token, packet.server_token) == 1) {
      conn->account->last_login_at = (int)time(NULL);
      strcpy(conn->account->last_login_ip, (char *)conn->info.ip);
      sql_update("accounts", conn->account);
      
      packet_set_int(response, ENDIAN_LITTLE, 0x00); // success
    } else {
      packet_set_int(response, ENDIAN_LITTLE, 0x02); // invalid password
    }
  } else {
    packet_set_int(response, ENDIAN_LITTLE, 0x01); // account does not exist
  }
  
  free(packet.username);
  connection_response_append(conn, response);

  if (need_set_email == 1) {
    response_setemail = packet_new(PROTOCOL_BNETD);
    if (response_setemail) {
      packet_message_id_set(response_setemail, 0x59);
      connection_response_append(conn, response_setemail);
    }
  }
}
