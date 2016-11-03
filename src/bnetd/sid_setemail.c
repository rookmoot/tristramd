#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "account.h"

/*
 * Packet ID: 0x59
 * Packet Name: sid_setemail
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Email Address
 * 
 * Remarks : 
 * Binds an email address to your account.
 * 
 * Sending this message is optional. However, you should only send it when either
 * you receive SID_SETEMAIL from the server or you receive status 0x0E from
 * SID_AUTH_ACCOUNTLOGONPROOF.
 * 
 * This packet used to be named SID_AUTH_RECONNECT, however Blizzard never had it
 * implemented, and so reused the packet ID for their email system.
 *
 */
struct request {
  char * email_address;
} __attribute__((packed));


/*
 * Packet ID: 0x59
 * Packet Name: sid_setemail
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Requests the client to bind an email address to the account.
 * 
 * See SID_SETEMAIL for more information.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x59, packet_bnetd_sid_setemail);
 *
 */
void packet_bnetd_sid_setemail(struct connection *conn, struct packet *request) {
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.email_address = packet_get_string(request, ENDIAN_LITTLE);

  if (conn->account != NULL) {
    if (!strlen(conn->account->mail)) {
      strcpy(conn->account->mail, packet.email_address);
      sql_update("accounts", conn->account);
    }
  }

  free(packet.email_address);
}
