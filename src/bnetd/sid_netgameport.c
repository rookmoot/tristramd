#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x45
 * Packet Name: sid_netgameport
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Port
 * 
 * Remarks : 
 * Sets the port used by the client for hosting WAR3/W3XP games. This value is
 * retreived from HKCU\Software\Blizzard Entertainment\Warcraft
 * III\Gameplay\netgameport, and is sent after the user logs on.
 *
 */
struct request {
  uint16_t port;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x45, packet_bnetd_sid_netgameport);
 *
 */
void packet_bnetd_sid_netgameport(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.port = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x45);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_netgameport not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
