#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x1B
 * Packet Name: sid_gamedataaddress
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Unknown 1
 * (SOCKADDR) Address &amp; Port
 * <dl><dd>SOCKADDR structure is as follows:</dd>
 * <dd>(WORD) Port</dd>
 * <dd>(DWORD) Address</dd></dl>
 * (DWORD) Unknown 2
 * (DWORD) Unknown 3
 * 
 * Remarks : 
 * Specifies host &amp; port that a game creator is using for a game.
 * 
 * The three unknowns come from PvPGN documentation. The first unknown appears to
 * always be 0x02, which is also seen in game listings. More info: <a
 * href="https://pastebin.com/iDtEmhpr">https://pastebin.com/iDtEmhpr</a>
 *
 */
struct request {
  uint16_t unknown;
  uint16_t port;
  uint32_t address;
  uint32_t unknown_251752669;
  uint32_t unknown_81594287;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x1B, packet_bnetd_sid_gamedataaddress);
 *
 */
void packet_bnetd_sid_gamedataaddress(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.unknown = packet_get_short(request, ENDIAN_LITTLE);
  packet.port = packet_get_short(request, ENDIAN_LITTLE);
  packet.address = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x1B);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_gamedataaddress not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
