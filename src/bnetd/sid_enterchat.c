#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "character.h"
#include "account.h"

/*
 * Packet ID: 0x0A
 * Packet Name: sid_enterchat
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Username *
 * (STRING) Statstring **
 * 
 * 
 * Remarks : 
 * Joins Chat.
 * 
 * * Null on WAR3/W3XP.
 * 
 * ** Null on CDKey Products, except for D2DV and D2XP when on realm characters..
 *
 */
struct request {
  char *username;
  char *statstring;
} __attribute__((packed));


/*
 * Packet ID: 0x0A
 * Packet Name: sid_enterchat
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (STRING) Unique name
 * (STRING) Statstring
 * (STRING) Account name
 * 
 * 
 * Remarks : 
 * Contains Client product, realm, statstring, and is sent as the response when the
 * client sends SID_ENTERCHAT. Unique name is the users unique name in chat (Which
 * may be Arta, Arta#2, Arta#3, etc).  Account name is the users account name
 * (Which in all 3 previous examples would be Arta).
 * 
 * Once you receive this packet, you are not in a channel, but can join/host games
 * and join channels. Because you are not in a channel, you cannot send text
 * messages yet (but you will not be disconnected if you do). See SID_JOINCHANNEL.
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0A, packet_bnetd_sid_enterchat);
 *
 */
void packet_bnetd_sid_enterchat(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;
  struct character *character = NULL;
  unsigned int i = 0;
  char *realmname = NULL;
  char statstring[33];
  char tmp[1024];
  
  memset(&packet, 0, sizeof(packet));
  packet.username = packet_get_string(request, ENDIAN_LITTLE);
  packet.statstring = packet_get_string(request, ENDIAN_LITTLE);

  memset(tmp, 0, 1024);
  realmname = strtok(packet.statstring, ",");
  
  character = conn->account->character;
  
  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }
  
  // set response message id.
  packet_message_id_set(response, 0x0A);

  if (!strncmp((const char *)character->name, (const char *)packet.username, strlen((char *)packet.username))) {
    packet_set_string(response, ENDIAN_LITTLE, (const char *)character->name);

    character_get_statstring(character, (char *)statstring);
    snprintf(tmp, 1024, "PX2D%s,%s,", realmname, character->name);
    packet_set_string(response, ENDIAN_LITTLE, tmp);
    for (i = 0; i < 33; i++) {
      packet_set_byte(response, ENDIAN_LITTLE, statstring[i]);
    }
    
    
    packet_set_string(response, ENDIAN_LITTLE, (const char *)character->name);
  }
  
  free(packet.username);
  free(packet.statstring);
  
  net_connection_response_append(conn, response);
}
