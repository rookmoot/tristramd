#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x65
 * Packet Name: sid_friendslist
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * [blank]
 * 
 * Remarks : 
 * Requests a friends list from Battle.net.
 * 
 * Works for all clients, however D2DV/D2XP will not receive friend updates
 * automatically. For those clients, you must request the list at your own
 * intervals if you wish to know about any updates.
 *
 */


/*
 * Packet ID: 0x65
 * Packet Name: sid_friendslist
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Number of entries
 * 
 * For each entry:
 * (STRING) Account name
 * (BYTE) Status
 * (BYTE) Location
 * (DWORD) Product id
 * (STRING) Location name
 * 
 * Remarks : 
 * This packet contains the current user's Friends List. If the friend is offline,
 * their `Product id` is `0x00`. `Location` is only supplied when it is relevant;
 * for example, if `Status` is set to offline (`0x00`), the location string will be
 * empty.
 * 
 * - Possible bitwise values for Status:
 * ```
 * 0x01: Mutual
 * 0x02: DND
 * 0x04: Away
 * ```
 * 
 * - Possible values for Location:
 * ```
 * 0x00: Offline
 * 0x01: Not in chat
 * 0x02: In chat
 * 0x03: In a public game
 * 0x04: In a private game
 * 0x05: In a private game and mutual friend
 * ```
 * 
 * Whether you are a particular user's friend causes different behavior when
 * receiving information about the type of game the user is in (this is true when
 * receiving friend updates, too). When the `Location` field is `0x04`, the user is
 * in a password-protected game, and you are not on that user's friends list. When
 * it is `0x05`, the user is in a password-protected game, and you are on that
 * user's friends list.
 * 
 * For example, let's say `MyndFyre` is logging on and `Arta[vL]` is on his friends
 * list. `Arta[vL]` is also in a password-protected game. `MyndFyre` is not on
 * `Arta[vL]`'s friends list. `MyndFyre` will receive location `0x04` for
 * `Arta[vL]` and the `Location name` parameter will be an empty string. And, if
 * `MyndFyre` were to type in `/f l`, Battle.net would respond with `SID_CHATEVENT`
 * saying `1. Arta[vL] is in a private game.`
 * 
 * On the other hand, if `Arta[vL]` had listed `MyndFyre` as a friend, then on
 * logon, `MyndFyre` will receive location `0x05` for `Arta[vL]` and the name of
 * the password-protected game will come through the `Location name` parameter of
 * the message. If `MyndFyre` were to type in `/f l`, Battle.net would respond with
 * `1. Arta[vL] is in the private game vL.`
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x65, packet_bnetd_sid_friendslist);
 *
 */
void packet_bnetd_sid_friendslist(struct connection *conn, struct packet *request EINA_UNUSED) {
  struct packet *response = NULL;

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x65);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_friendslist not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
