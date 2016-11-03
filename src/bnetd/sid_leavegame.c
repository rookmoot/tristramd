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
 * Packet ID: 0x1F
 * Packet Name: sid_leavegame
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Notifies Battle.net that you have left a game.
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
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1F, packet_bnetd_sid_leavegame);
 *
 */
void packet_bnetd_sid_leavegame(struct connection *conn, struct packet *request EINA_UNUSED) {
  account_game_set(conn->account, NULL);
}
