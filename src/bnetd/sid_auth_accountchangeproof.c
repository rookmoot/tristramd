#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x56
 * Packet Name: sid_auth_accountchangeproof
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) [20] Old password proof
 * (BYTE) [32] New password's salt (s)
 * (BYTE) [32] New password's verifier (v)
 * 
 * 
 * Remarks : 
 * This message is sent after receiving a successful SID_AUTH_ACCOUNTCHANGE
 * message, and contains the proof for the client's new password.
 * 
 * See [NLS/SRP Protocol] for more information.
 *
 */
struct request {
  uint8_t old_password_proof;
  uint8_t new_passwords_salt;
  uint8_t new_passwords_verifier;
} __attribute__((packed));


/*
 * Packet ID: 0x56
 * Packet Name: sid_auth_accountchangeproof
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status code
 * (BYTE) [20] Server password proof for old password (M2)
 * 
 * 
 * Remarks : 
 * This message reports success or failure for a password change operation.
 * 
 * Status codes:<dl><dd>0x00: Password changed</dd>
 * <dd>0x02: Incorrect old password</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x56, packet_bnetd_sid_auth_accountchangeproof);
 *
 */
void packet_bnetd_sid_auth_accountchangeproof(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.old_password_proof = packet_get_byte(request, ENDIAN_LITTLE);
  packet.new_passwords_salt = packet_get_byte(request, ENDIAN_LITTLE);
  packet.new_passwords_verifier = packet_get_byte(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x56);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_auth_accountchangeproof not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
