#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1C
 * Packet Name: sid_startadvex3
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Game State
 * (DWORD) Game Uptime in seconds
 * (WORD) Game Type
 * (WORD) Sub Game Type
 * (DWORD) Provider Version Constant (0xFF)
 * (DWORD) Ladder Type
 * (STRING) Game Name
 * (STRING) Game Password
 * (STRING) Game Statstring
 * 
 * Remarks : 
 * Used by clients to inform the server that a game has been created, or that the
 * state of a created game has changed.
 * <dl><dt>Bitwise flags for State:</dt>
 * <dd>0x01: Game is private</dd>
 * <dd>0x02: Game is full</dd>
 * <dd>0x04: Game contains players (other than creator)</dd>
 * <dd>0x08: Game is in progress</dd>
 * <dd>0x80: Game is a replay</dd></dl>
 * <dl><dt>Possible values for Game Type (Sub Game Type):</dt>
 * <dd>0x02: Melee</dd>
 * <dd>0x03: Free for All</dd>
 * <dd>0x04: 1 vs 1</dd>
 * <dd>0x05: Capture The Flag</dd>
 * <dd>0x06: Greed (Resources, 0x01: 2500, 0x02: 500, 0x03: 7500, 0x04:
 * 10000)</dd>
 * <dd>0x07: Slaughter (Minutes, 0x01: 15, 0x02: 30, 0x03: 45, 0x04: 60)</dd>
 * <dd>0x08: Sudden Death</dd>
 * <dd>0x09: Ladder (Disconnects, 0x00: Not a loss, 0x01: Counts as a loss)</dd>
 * <dd>0x0A: Use Map Settings</dd>
 * <dd>0x0B: Team Melee (Number Of Teams, 0x01: 2 Teams, 0x02: 3 Teams, etc.)</dd>
 * <dd>0x0C: Team Free For All (Number Of Teams, 0x01: 2 Teams, 0x02: 3 Teams,
 * etc.)</dd>
 * <dd>0x0D: Team Capture The Flag (Number Of Teams, 0x01: 2 Teams, 0x02: 3 Teams,
 * etc.)</dd>
 * <dd>0x0F: Top vs. Bottom (Number Of Teams, 1-7 specifies the ratio of players
 * belonging to both teams)</dd>
 * <dd>0x10: Iron Man Ladder (W2BN only)</dd>
 * <dd>0x20: PGL</dd></dl>
 * 
 * Provider Version Constant: The Current version of the game (1.16.1) uses 0xFF,
 * previous versions have used 0x1F, but this may vary depending on game version.
 * 
 * <dl><dt>Possible values for Ladder:</dt>
 * <dd>0x00: Game is Normal (Non-Ladder)</dd>
 * <dd>0x01: Game is Ladder</dd>
 * <dd>0x03: Game is Iron Man Ladder (W2BN only)</dd></dl>
 * It could be that the ladder is bitwise as well, and that 0x02 means Iron Man and
 * 0x03 just means Iron Man + Ladder.<br>
 * Sub Game Type appears to be 0x01 for all game types except Top vs Bottom, where
 * it seems to depend on the size of each team. More research will be needed to
 * confirm.
 *
 */
struct request {
  uint32_t game_state;
  uint32_t game_uptime_in_seconds;
  uint16_t game_type;
  uint16_t sub_game_type;
  uint32_t provider_version_constant;
  uint32_t ladder_type;
  char * game_name;
  char * game_password;
  char * game_statstring;
} __attribute__((packed));


/*
 * Packet ID: 0x1C
 * Packet Name: sid_startadvex3
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * 
 * Remarks : 
 * <dl><dt>Possible values for Status:</dt>
 * <dd>0x00: Ok</dd>
 * <dd>0x01: Failed</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1C, packet_bnetd_sid_startadvex3);
 *
 */
void packet_bnetd_sid_startadvex3(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.game_state = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_uptime_in_seconds = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_type = packet_get_short(request, ENDIAN_LITTLE);
  packet.sub_game_type = packet_get_short(request, ENDIAN_LITTLE);
  packet.provider_version_constant = packet_get_int(request, ENDIAN_LITTLE);
  packet.ladder_type = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_statstring = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x1C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_startadvex3 not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
