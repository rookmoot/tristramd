#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x3F
 * Packet Name: d2gs_characterphrase
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Phrase ID
 * 
 * 
 * Remarks : 
 * All phrases sent to the server will be heard by all players in your vicinity.
 * 
 * Phrase IDs: (Note: The deciding of which of the two phrases are used is random)
 * [0x19] "Help!" / "Help me!"
 * [0x1A] "Follow me!" / "Come on!"
 * [0x1B] "This is yours." / "This is for you."
 * [0x1C] "Thanks!" / "Thank you."
 * [0x1D] "Uh, oops." / "Forgive me."
 * [0x1E] "Bye!" / "Good Bye!"
 * [0x1F] "Die!" / "Time to die!"
 * [0x20] "Run!" / "Run away!" Please note: This message's official name is not
 * known, and has been invented.
 *
 */
struct request {
  uint16_t phrase_id;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x3F, packet_d2gs_d2gs_characterphrase);
 *
 */
void packet_d2gs_d2gs_characterphrase(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.phrase_id = packet_get_short(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_D2GS);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x3F);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_d2gs_d2gs_characterphrase not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
