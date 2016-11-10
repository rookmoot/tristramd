#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x37
 * Packet Name: sid_unknown_37
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Count of open characters
 * (VOID) [] Character structure
 * 
 * For each character structure:
 * (STRING) Realm Name (if closed) and Character Name
 * (BYTE) Unknown B1
 * (BYTE) Unknown B2
 * (BYTE) Helm GFX
 * (BYTE) Body GFX
 * (BYTE) Leg GFX
 * (BYTE) Left Hand Weapon
 * (BYTE) Left Hand GFX
 * (BYTE) Right Hand Weapon
 * (BYTE) Right Hand GFX
 * (BYTE) Unknown B3
 * (BYTE) Unknown B4
 * (BYTE) Unknown B5
 * (BYTE) Unknown B6
 * (BYTE) Unknown B7
 * (BYTE) Unknown B8
 * (BYTE) Unknown B9
 * (BYTE) Unknown B10
 * (BYTE) Unknown B11
 * (BYTE) Class
 * (DWORD) Unknown DW1
 * (DWORD) Unknown DW2
 * (DWORD) Unknown DW3
 * (DWORD) Unknown DW4
 * (BYTE) Level
 * (BYTE) Status
 * (BYTE) Title
 * (BYTE) Unknown B13
 * (BYTE) Emblem BGC
 * (BYTE) Emblem FGC
 * (BYTE) Emblem Number
 * (BYTE) Unknown B14
 * (STRING) Guild Tag (max length: 3)
 * 
 * Remarks : 
 * Not much is known about this packet. The structure of this packet and its
 * documentation was taken from PvPGN. The collected information can be found <a
 * href="https://pastebin.com/Dfz8Dzua">here</a>, which includes possible values
 * for the packet format.
 *
 */
struct request {
  uint32_t count_of_open_characters;
  void * character_structure;
  char * realm_name;
  uint8_t unknown_b;
  uint8_t unknown_b_917447586;
  uint8_t helm_gfx;
  uint8_t body_gfx;
  uint8_t leg_gfx;
  uint8_t left_hand_weapon;
  uint8_t left_hand_gfx;
  uint8_t right_hand_weapon;
  uint8_t right_hand_gfx;
  uint8_t unknown_b_1475822624;
  uint8_t unknown_b_1299296698;
  uint8_t unknown_b_2106842;
  uint8_t unknown_b_795043095;
  uint8_t unknown_b_1854088679;
  uint8_t unknown_b_368926248;
  uint8_t unknown_b_1656574372;
  uint8_t unknown_b_1881270426;
  uint8_t unknown_b_1050245687;
  uint8_t class;
  uint32_t unknown_dw;
  uint32_t unknown_dw_2060660592;
  uint32_t unknown_dw_444763127;
  uint32_t unknown_dw_1864542615;
  uint8_t level;
  uint8_t status;
  uint8_t title;
  uint8_t unknown_b_434988861;
  uint8_t emblem_bgc;
  uint8_t emblem_fgc;
  uint8_t emblem_number;
  uint8_t unknown_b_1092551628;
  char * guild_tag;
} __attribute__((packed));


/*
 * Packet ID: 0x37
 * Packet Name: sid_unknown_37
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Unknown 1
 * (DWORD) Unknown 2
 * (DWORD) Count of characters
 * 
 * For each character:
 * (see related C>S packet for struct)
 * 
 * Remarks : 
 * Not much is known about this packet. The structure of this packet and its
 * documentation was taken from PvPGN. The collected information can be found <a
 * href="https://pastebin.com/Dfz8Dzua">here</a>, which includes possible values
 * for the packet format.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x37, packet_bnetd_sid_unknown_37);
 *
 */
void packet_bnetd_sid_unknown_37(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.count_of_open_characters = packet_get_int(request, ENDIAN_LITTLE);
  packet.character_structure = (char *)packet_get_string(request, ENDIAN_LITTLE);
  packet.realm_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.helm_gfx = packet_get_byte(request, ENDIAN_LITTLE);
  packet.body_gfx = packet_get_byte(request, ENDIAN_LITTLE);
  packet.leg_gfx = packet_get_byte(request, ENDIAN_LITTLE);
  packet.left_hand_weapon = packet_get_byte(request, ENDIAN_LITTLE);
  packet.left_hand_gfx = packet_get_byte(request, ENDIAN_LITTLE);
  packet.right_hand_weapon = packet_get_byte(request, ENDIAN_LITTLE);
  packet.right_hand_gfx = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.class = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_dw = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown_dw = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown_dw = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown_dw = packet_get_int(request, ENDIAN_LITTLE);
  packet.level = packet_get_byte(request, ENDIAN_LITTLE);
  packet.status = packet_get_byte(request, ENDIAN_LITTLE);
  packet.title = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.emblem_bgc = packet_get_byte(request, ENDIAN_LITTLE);
  packet.emblem_fgc = packet_get_byte(request, ENDIAN_LITTLE);
  packet.emblem_number = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown_b = packet_get_byte(request, ENDIAN_LITTLE);
  packet.guild_tag = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x37);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_unknown_37 not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
