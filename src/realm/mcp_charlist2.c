#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "character.h"
#include "account.h"

/*
 * Packet ID: 0x19
 * Packet Name: mcp_charlist2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of characters to list.
 * 
 * Remarks : 
 * Requests a character list.
 * 
 * Up to a maximum of 8 characters can be requested.
 *
 */
struct request {
  uint32_t number;
} __attribute__((packed));


/*
 * Packet ID: 0x19
 * Packet Name: mcp_charlist2
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Number of characters requested
 * (DWORD) Number of characters that exist on this account
 * (WORD) Number of characters returned
 * 
 * <dl>
 * <dt>For each character:</dt>
 * <dd>(DWORD) Expiration Date</dd>
 * <dd>(STRING) Character name</dd>
 * <dd>(STRING) Character statstring</dd>
 * </dl>
 * 
 * Remarks : 
 * Lists characters.
 * 
 * The expiration date is a second count. To determine when the character will
 * expire, add this time to  January 1 00:00:00 UTC 1970 and determine the
 * difference between that value and now (all in seconds).
 * 
 * The statstrings in this packet  do not contain the product or character name.
 * Everything else is the same as a character statstring you would receive from a
 * character entering the channel.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_REALM, 0x19, packet_realm_mcp_charlist2);
 *
 */
void packet_realm_mcp_charlist2(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  struct character *character = NULL;
  struct accounts_characters *ac = NULL;
  Eina_List *characters = NULL, *l = NULL;
  unsigned int count = 0;
  unsigned int i = 0;
  char statstring[33];
  
  memset(&packet, 0, sizeof(packet));
  packet.number = packet_get_short(request, ENDIAN_BIG);
  packet.number = 8;

  // create the response packet.
  response = packet_new(PROTOCOL_REALM);
  if (!response) {
    return;
  }
  // set response message id.
  packet_message_id_set(response, 0x19);

  
  characters = sql_select("accounts_characters", "account_id=%d ORDER BY created_at ASC", conn->account->id);
  count = eina_list_count(characters);
  if (count) {
    packet_set_short(response, ENDIAN_LITTLE, packet.number);
    packet_set_int(response, ENDIAN_LITTLE, count);
    packet_set_short(response, ENDIAN_LITTLE, count);
    EINA_LIST_FOREACH(characters, l, ac) {
      character = character_load(conn->account, ac->name);
      if (character) {
	character_get_statstring(character, (char *)statstring);
	packet_set_int(response, ENDIAN_LITTLE, time(NULL) + 10000000);
	packet_set_string(response, ENDIAN_LITTLE, (const char *)character->name);
	for (i = 0; i < 33; i++) {
	  packet_set_byte(response, ENDIAN_LITTLE, statstring[i]);
	}
	packet_set_byte(response, ENDIAN_LITTLE, 0x00);
      }
    }
  } else {
    packet_set_short(response, ENDIAN_LITTLE, packet.number);
    packet_set_int(response, ENDIAN_LITTLE, 0x00);
    packet_set_short(response, ENDIAN_LITTLE, 0x00);
  }
  
  connection_response_append(conn, response);
}
