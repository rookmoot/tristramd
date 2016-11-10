#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x66
 * Packet Name: sid_friendsupdate
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Index on friends list
 * 
 * Remarks : 
 * This message requests an update on a specific user on the friends list.
 * Battle.net should reply immediately with the same packet (see related).
 * 
 * The indexes are 0-based, so friend #1 on the friends list would have a value of
 * 0 in this message, friend #2 would be 1, and so on.
 * 
 * Works for all clients, however D2DV/D2XP will not receive friend updates
 * automatically. For those clients, you must request the list at your own
 * intervals if you wish to know about any updates.
 *
 */
struct request {
  uint8_t index_on_friends_list;
} __attribute__((packed));


/*
 * Packet ID: 0x66
 * Packet Name: sid_friendsupdate
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Entry number
 * (BYTE) Status
 * (BYTE) Location
 * (DWORD) Product id
 * (STRING) Location
 * 
 * Remarks : 
 * Sent to clients supporting this packet when the friend-listed account's `Status`
 * or `Location` changes. The first friend is always entry `0x00`.
 * 
 * Note that there is a Battle.net server bug in which when you are automatically
 * sent this packet, the `Product id` is your own `Product id` instead of your
 * friend's. So if you were to be using `WAR3`, for example, and a friend signs on
 * using `SEXP`, the `Product id` in this packet will be `WAR3`. To receive the
 * _correct_ `Product id` for the user, you may request another update for this
 * user (by sending `0x66` back to the server with the same `Entry Number`), or
 * request the full list again.
 * 
 * In addition, this packet _is not_ sent to you automatically if the friendship is
 * not mutual.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x66, packet_bnetd_sid_friendsupdate);
 *
 */
void packet_bnetd_sid_friendsupdate(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.index_on_friends_list = packet_get_byte(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x66);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_friendsupdate not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
