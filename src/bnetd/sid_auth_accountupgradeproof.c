#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x58
 * Packet Name: sid_auth_accountupgradeproof
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Client Token
 * (DWORD) [5] Old Password Hash
 * (BYTE) [32] New Password Salt
 * (BYTE) [32] New Password Verifier
 * 
 * Remarks : 
 * <dl>
 * <dt>Old Password Hash:</dt>
 *  <dd>Broken SHA-1 Double password hash as seen in SID_LOGONRESPONSE(2)
 * OLS.</dd>
 * <dt>New Password Salt & Verifier:</dt>
 * <dd>Salt and Verifier values as seen in SID_AUTH_ACCOUNTCREATE.</dd>
 * </dl>
 * Old Password is the account's current password. The New Password can be the same
 * as the Old Password, but it can be used to change the password as well.
 * 
 * Basically this packet would convert the stored password hash to a new one, and
 * thus become NLS. However, this packet is no longer responded to, and upgrading
 * accounts is therefore impossible.(Note: If you have an account in need of
 * upgrading [created with SID_CREATEACCOUNT(2)], you can let the account expire
 * and recreate it with SID_AUTH_ACCOUNTCREATE.)
 *
 */
struct request {
  uint32_t client_token;
  uint32_t old_password_hash;
  uint8_t new_password_salt;
  uint8_t new_password_verifier;
} __attribute__((packed));


/*
 * Packet ID: 0x58
 * Packet Name: sid_auth_accountupgradeproof
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * (DWORD) [5] Password proof
 * 
 * Remarks : 
 * Status codes:<dl><dd>0x00: Password changed</dd>
 * <dd>0x02: Incorrect old password</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x58, packet_bnetd_sid_auth_accountupgradeproof);
 *
 */
void packet_bnetd_sid_auth_accountupgradeproof(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.old_password_hash = packet_get_int(request, ENDIAN_LITTLE);
  packet.new_password_salt = packet_get_byte(request, ENDIAN_LITTLE);
  packet.new_password_verifier = packet_get_byte(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x58);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountupgradeproof not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
