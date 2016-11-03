#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "chat.h"

/*
 * Packet ID: 0x10
 * Packet Name: sid_leavechat
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * 
 * Remarks : 
 * Leaves chat mode but does not disconnect. Generally sent when entering a game.
 * This is also sent by D2DV/D2XP when switching characters, and by all products
 * when logging off.
 * 
 * After you send this, normal chat functions to not work. In order to enter a
 * channel when you are not in chat mode, you must send SID_JOINCHANNEL.
 *
 */

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x10, packet_bnetd_sid_leavechat);
 *
 */
void packet_bnetd_sid_leavechat(struct connection *conn, struct packet *request EINA_UNUSED) {
  chat_channel_account_remove(conn->account);
}
