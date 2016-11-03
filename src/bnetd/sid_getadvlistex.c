#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x09
 * Packet Name: sid_getadvlistex
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Game Type
 * (WORD) Sub Game Type
 * (DWORD) Viewing Filter
 * (DWORD) Reserved (0)
 * (DWORD) Number of Games
 * (STRING) Game Name
 * (STRING) Game Password
 * (STRING) Game Statstring
 * 
 * Remarks : 
 * Retrieves a list of games.
 * 
 * <b>Game Type:</b>
 * <dl><dd>For STAR/SEXP/SSHR/JSTR and W2BN, a value of 0 indicates that any type
 * is acceptable.
 * <dl>
 * <dt>Possible game types:</dt>
 * <dd>0x00: All</dd>
 * <dd>0x02: Melee</dd>
 * <dd>0x03: Free for all</dd>
 * <dd>0x04: one vs one</dd>
 * <dd>0x05: CTF</dd>
 * <dd>0x06: Greed</dd>
 * <dd>0x07: Slaughter</dd>
 * <dd>0x08: Sudden Death</dd>
 * <dd>0x09: Ladder</dd>
 * <dd>0x10: Iron man ladder</dd>
 * <dd>0x0A: Use Map Settings</dd>
 * <dd>0x0B: Team Melee</dd>
 * <dd>0x0C: Team FFA</dd>
 * <dd>0x0D: Team CTF</dd>
 * <dd>0x0F: Top vs. Bottom</dd>
 * </dl>
 * 
 * For DRTL/DSHR, Game Type is used to specify a 'level range'. This ensures that
 * clients receive a list of games containing players whose experience is similar
 * to their own.
 * <dl>
 * <dt>Possible ranges:</dt>
 * <dd>0x00: Level 1</dd>
 * <dd>0x01: 2 - 3</dd>
 * <dd>0x02: 4 - 5</dd>
 * <dd>0x03: 6 - 7</dd>
 * <dd>0x04: 8 - 9</dd>
 * <dd>0x05: 10 - 12</dd>
 * <dd>0x06: 13 - 16</dd>
 * <dd>0x07: 17 - 19</dd>
 * <dd>0x08: 20 - 24</dd>
 * <dd>0x09: 25 - 29</dd>
 * <dd>0x0A: 30 - 34</dd>
 * <dd>0x0B: 35 - 39</dd>
 * <dd>0x0C: 40 - 47</dd>
 * <dd>0x0D: 48 - 50</dd>
 * </dl>
 * For all other games, Game Type can be set to 0x00.</dd></dl>
 * <b>Sub Game Type:</b>
 * <dl>
 * <dd>For Greed, this is resources in multiples of 2500 (0x01 = 2500, 0x02 = 5000,
 * etc).</dd>
 * <dd>For Slaughter, this is minutes in multiples of 15 (0x01 = 15, 0x02 = 30,
 * etc).</dd>
 * <dd>For Ladder, this indicates if disconnects count as a loss (0x01 = Yes, 0x00
 * = No).</dd>
 * <dd>For Team Melee, this is the number of teams (0x01 = 2, 0x02 = 3, etc).</dd>
 * <dd>For Team Free For All, this is the number of teams (0x01 = 2, 0x02 = 3,
 * etc).</dd>
 * <dd>For Team Capture The Flag, this is the number of teams (0x01 = 2, 0x02 = 3,
 * etc).</dd>
 * <dd>For Top vs. Bottom, this is the players to team ratio (1-7).</dd>
 * <dd>For any other game type, it is 0x01 which indicates no sub game type.</dd>
 * </dl>
 * <b>Viewing Filter:</b>
 * <dl>
 * <dd>0xFFFF is used to use the combination of values in this packet.</dd>
 * <dd>0xFF80 is used to show all games.</dd>
 * <dd>For STAR/SEXP/SSHR/JSTR, viewing filter is set to 0x30.</dd>
 * <dd>For DRTL/DSHR, viewing filter is set to 0xFFFF by the game, but setting it
 * to 0x00 will disable any viewing limitations, letting you view all games.</dd>
 * </dl>
 * <b>Reserved (0):</b>
 * <dl><dd>This value is hardcoded to 0x00 by all games.</dd></dl>
 * <b>Number of Games:</b>
 * <dl><dd>This is the number of games to list. For a full listing, it's safe to
 * use 0xFF. By default, DRTL/DSHR sets this to 0x19.</dd></dl>
 *
 */
struct request {
  uint16_t game_type;
  uint16_t sub_game_type;
  uint32_t viewing_filter;
  uint32_t reserved;
  uint32_t number_of_games;
  char * game_name;
  char * game_password;
  char * game_statstring;
} __attribute__((packed));


/*
 * Packet ID: 0x09
 * Packet Name: sid_getadvlistex
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of games
 * 
 * If count is 0:
 * (DWORD) Status
 * 
 * Otherwise, games are listed thus:
 * For each list item:
 * WarCraft III:
 * (DWORD)  Game Type
 * (DWORD)  Language ID
 * (WORD)   Address Family (Always AF_INET)
 * (WORD)   Port
 * (DWORD)  Host's IP
 * (DWORD)  sin_zero (0)
 * (DWORD)  sin_zero (0)
 * (DWORD)  Unknown (Public = 0x10, Private = 0x11)
 * (DWORD)  Elapsed time (in seconds)
 * (STRING) Game name *
 * (STRING) Game password (always empty)
 * (BYTE) Free slots (character representing hexadecimal number)
 *       (BYTE)[8] Host counter (characters representing reversed hexadecimal
 * number, ex: '20000000' for second time this host has hosted during his session)
 * (STRING) Encoded map information
 * All Other Clients:
 * (WORD)   Game Type
 * (WORD)   Sub Game Type
 * (DWORD)  Language ID
 * (WORD)   Address Family (Always AF_INET)
 * (WORD)   Port
 * (DWORD)  Host's IP
 * (DWORD)  sin_zero (0)
 * (DWORD)  sin_zero (0)
 * (DWORD)  Game Status
 * (DWORD)  Elapsed time (in seconds)
 * (STRING) Game name *
 * (STRING) Game password
 * (STRING) Game statstring
 * 
 * Remarks : 
 * Returns a list of available games and their information. Varies depending on
 * product.
 * 
 * 
 * * This field is UTF-8 encoded.
 * 
 * For WarCraft III, map information is encoded using the function found <a
 * href="http://forum.valhallalegends.com/index.php?topic=15419.5">here</a>:
 * <dl><dt>Encoded data:</dt>
 * <dd>(DWORD) Map Flags</dd>
 * <dd>(BYTE) Map Null 1</dd>
 * <dd>(BYTE) Map Width (playable area)</dd>
 * <dd>(BYTE) Map Null 2</dd>
 * <dd>(BYTE) Map Height (playable area)</dd>
 * <dd>(BYTE) Map Null 3</dd>
 * <dd>(DWORD) Map CRC</dd>
 * <dd>(STRING) Map Path</dd>
 * <dd>(STRING) Game Host</dd>
 * <dd>(BYTE) Map Unknown (possibly a STRING with just the null terminator)</dd>
 * <dd>(BYTE)[20] Unknown (probably a SHA1 hash)</dd></dl>
 * <dl><dt>Possible flags (combine the below settings) for Map Flags:</dt>
 * <dd><dl><dt>Game Speed (mask 0x00000003, unique)</dt>
 * <dd>0x00000000: Slow</dd>
 * <dd>0x00000001: Normal</dd>
 * <dd>0x00000002: Fast</dd></dl></dd>
 * <dd><dl><dt>Visibility setting (mask 0x00000F00, unique)</dt>
 * <dd>0x00000100: Hide Terrain</dd>
 * <dd>0x00000200: Map Explored</dd>
 * <dd>0x00000400: Always Visible</dd>
 * <dd>0x00000800: Default</dd></dl></dd>
 * <dd><dl><dt>Observers setting (mask 0x40003000, unique)</dt>
 * <dd>0x00000000: No Observers</dd>
 * <dd>0x00002000: Observers on Defeat</dd>
 * <dd>0x00003000: Full Observers</dd>
 * <dd>0x40000000: Referees</dd></dl></dd>
 * <dd><dl><dt>Other advanced host settings (mask 0x07064000, combinable)</dt>
 * <dd>0x00004000: Teams Together (team members are placed at neighbored starting
 * locations)</dd>
 * <dd>0x00060000: Lock Teams</dd>
 * <dd>0x01000000: Full Shared Unit Control</dd>
 * <dd>0x02000000: Random Hero</dd>
 * <dd>0x04000000: Random Races</dd></dl></dd></dl>
 * Non-WarCraft III Game Statstring Field:
 * For other clients, game statring follows the format found <a
 * href="?op=doc&did=13">here</a>.
 * 
 * WarCraft III Game Type Field (combine the below settings):
 * <dl><dt>Game Type (mask 0x000000FF, unique)</dt>
 * <dd>0x00000001: Custom</dd>
 * <dd>0x00000009: Ladder</dd></dl>
 * <dl><dt>Map Author (mask 0x00006000, combinable)</dt>
 * <dd>0x00002000: Blizzard</dd>
 * <dd>0x00004000: Custom</dd></dl>
 * <dl><dt>Battle/Scenario (mask 0x00018000, unique)</dt>
 * <dd>0x00000000: Battle</dd>
 * <dd>0x00010000: Scenario</dd></dl>
 * <dl><dt>Map Size (mask 0x000E0000, combinable)</dt>
 * <dd>0x00020000: Small</dd>
 * <dd>0x00040000: Medium</dd>
 * <dd>0x00080000: Huge</dd></dl>
 * <dl><dt>Observers (mask 0x00700000, unique)</dt>
 * <dd>0x00100000: Allowed observers ("Full Observers" and "Referees"
 * options)</dd>
 * <dd>0x00200000: Observers on defeat</dd>
 * <dd>0x00400000: No observers</dd></dl>
 * <dl><dt>Public/Private (mask 0x00000800, unique)</dt>
 * <dd>0x00000000: Public game</dd>
 * <dd>0x00000800: Private game</dd></dl>
 * STAR/SEXP/SSHR/JSTR/W2BN Game Type Field:
 * <dl><dt>Game Type (unique)</dt>
 * <dd>0x01: Custom (WAR3/W3XP)</dd>
 * <dd>0x02: Melee</dd>
 * <dd>0x03: Free For All</dd>
 * <dd>0x04: One vs One</dd>
 * <dd>0x05: Capture the Flag</dd>
 * <dd>0x06: Greed</dd>
 * <dd>0x07: Slaughter</dd>
 * <dd>0x08: Sudden Death</dd>
 * <dd>0x09: Ladder</dd>
 * <dd>0x10: Iron man ladder (W2BN)</dd>
 * <dd>0x0A: Use Map Settings</dd>
 * <dd>0x0B: Team Melee</dd>
 * <dd>0x0C: Team FFA</dd>
 * <dd>0x0D: Team CTF</dd>
 * <dd>0x0F: Top vs. Bottom</dd></dl>
 * STAR/SEXP/SSHR/JSTR/W2BN Sub Game Type Field:
 * <dl><dt>For Greed:</dt>
 * <dd>0x01: 2500 resources</dd>
 * <dd>0x02: 5000 resources</dd>
 * <dd>0x03: 7500 resources</dd>
 * <dd>0x04: 10000 resources</dd></dl>
 * <dl><dt>For Slaughter:</dt>
 * <dd>0x01: 15 minutes</dd>
 * <dd>0x02: 30 minutes</dd>
 * <dd>0x03: 45 minutes</dd>
 * <dd>0x04: 60 minutes</dd></dl>
 * <dl><dt>For Team Melee, Team Free For All, and Team Capture the Flag:</dt>
 * <dd>0x01: 2 teams</dd>
 * <dd>0x02: 3 teams</dd>
 * <dd>0x03: 4 teams</dd></dl>
 * <dl><dt>For Top vs. Bottom:</dt>
 * <dd>0x01: 1 v 7</dd>
 * <dd>0x02: 2 v 6</dd>
 * <dd>0x03: 3 v 5</dd>
 * <dd>0x04: 4 v 4</dd>
 * <dd>0x05: 5 v 3</dd>
 * <dd>0x06: 6 v 2</dd>
 * <dd>0x07: 7 v 1</dd></dl>
 * <dl><dt>For Ladder:</dt>
 * <dd>0x00: Disconnects do not count as loss</dd>
 * <dd>0x01: Disconnects count as loss</dd></dl>
 * <dl><dt>For Other Game Types:</dt>
 * <dd>0x01: No sub-game type</dd></dl>
 * DRTL/DSHR Game Type Field:
 * <dl><dt>Game Type is used to specify a 'level range'. This ensures that clients
 * receive a list of games containing players whose experience is similar to their
 * own.</dt>
 * <dd>0x00: Level 1</dd>
 * <dd>0x01: Level 2 or 3</dd>
 * <dd>0x02: Level 4 or 5</dd>
 * <dd>0x03: Level 6 or 7</dd>
 * <dd>0x04: Level 8 or 9</dd>
 * <dd>0x05: Level 10 - 12</dd>
 * <dd>0x06: Level 13 - 16</dd>
 * <dd>0x07: Level 17 - 19</dd>
 * <dd>0x08: Level 20 - 24</dd>
 * <dd>0x09: Level 25 - 29</dd>
 * <dd>0x0A: Level 30 - 34</dd>
 * <dd>0x0B: Level 35 - 39</dd>
 * <dd>0x0C: Level 40 - 47</dd>
 * <dd>0x0D: Level 48 - 50</dd></dl>
 * 
 * Note that fields from Address Family to sin_zero form a sockaddr_in structure.
 * 
 * Valid status codes:
 * <dl><dt>This field is the 'Game Status' for non-WarCraft III and the 'Status'
 * when no games are listed for both WarCraft III and other clients.</dt>
 * <dd>0x00: OK</dd>
 * <dd>0x01: Game doesn't exist</dd>
 * <dd>0x02: Incorrect password</dd>
 * <dd>0x03: Game full</dd>
 * <dd>0x04: Game already started</dd>
 * <dd>0x05: Spawned CD-Key not allowed</dd>
 * <dd>0x06: Too many server requests</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x09, packet_bnetd_sid_getadvlistex);
 *
 */
void packet_bnetd_sid_getadvlistex(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.game_type = packet_get_short(request, ENDIAN_LITTLE);
  packet.sub_game_type = packet_get_short(request, ENDIAN_LITTLE);
  packet.viewing_filter = packet_get_int(request, ENDIAN_LITTLE);
  packet.reserved = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_games = packet_get_int(request, ENDIAN_LITTLE);
  packet.game_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_password = packet_get_string(request, ENDIAN_LITTLE);
  packet.game_statstring = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x09);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_getadvlistex not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
