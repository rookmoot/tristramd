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
 * Packet ID: 0x5B
 * Packet Name: sid_changeemail
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Account Name
 * (STRING) Old Email Address
 * (STRING) New Email Address
 * 
 * Remarks : 
 *
 */
struct request {
  char *username;
  char *old_mail;
  char *new_mail;
} __attribute__((packed));


/*
 * NO RESPONSE NEEDED
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5B, packet_bnetd_sid_changeemail);
 *
 */
void packet_bnetd_sid_changeemail(struct connection *conn, struct packet *request) {
  struct account *account = NULL;
  struct request packet;

  conn = conn;
  
  memset(&packet, 0, sizeof(packet));
  packet.username = packet_get_string(request, ENDIAN_LITTLE);
  packet.old_mail = packet_get_string(request, ENDIAN_LITTLE);
  packet.new_mail = packet_get_string(request, ENDIAN_LITTLE);

  account = (struct account *)sql_select_one("accounts", "username='%s' AND mail='%s'", packet.username, packet.old_mail);
  if (account) {
    strncpy(account->mail, packet.new_mail, strlen(packet.new_mail));
    sql_update("accounts", (void *)account);
  } else {
  }
  
  free(packet.username);
  free(packet.old_mail);
  free(packet.new_mail);
}
