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
 * Packet ID: 0x5A
 * Packet Name: sid_resetpassword
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Account Name
 * (STRING) Email Address
 * 
 * Remarks : 
 *
 */
struct request {
  char *username;
  char *mail;
} __attribute__((packed));


/*
 * NO RESPONSE NEEDED
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5A, packet_bnetd_sid_resetpassword);
 *
 */
void packet_bnetd_sid_resetpassword(struct connection *conn, struct packet *request) {
  struct account *account = NULL;
  struct request packet;

  conn = conn;
  
  memset(&packet, 0, sizeof(packet));
  packet.username = packet_get_string(request, ENDIAN_LITTLE);
  packet.mail = packet_get_string(request, ENDIAN_LITTLE);

  account = (struct account *)sql_select_one("accounts", "username='%s' AND mail='%s'", packet.username, packet.mail);
  if (account) {
    // here we go.
    // We received a password reset packet and associated with it
    // we got an account's username and the associated email for it.
    // We check if an account exists according to username and email
    // provided.
    // If this account exists, we need to store in database on a table
    // named scheduler for example, all the stuff to do outside the
    // main program.
    // i.e. A website with a crontab can come and check those rows, then
    // execute all that crapy stuff outside the main program loop.
    // In this particular case, we are willing to send a fancy email
    // to the user with a password reset link for example.
  } else {
  }
  
  free(packet.username);
  free(packet.mail);
}
