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
 * Packet ID: 0x6A
 * Packet Name: d2gs_entergameenvironment
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * blank
 * 
 * 
 * Remarks : 
 * This byte should be sent in order to start receiving in-game messages and to
 * interact with the world itself. 
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x6A, packet_d2gs_d2gs_entergameenvironment);
 *
 */
void packet_d2gs_d2gs_entergameenvironment(struct connection *conn, struct packet *request EINA_UNUSED) {
  EINA_LOG_INFO("Account %ld connected on D2GS and entering environnement.", conn->account->id);
}
