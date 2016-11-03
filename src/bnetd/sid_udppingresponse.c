#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x14
 * Packet Name: sid_udppingresponse
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) UDPCode
 * 
 * Remarks : 
 * Enables UDP support.
 * 
 * Server supplies code via UDP packet PKT_SERVERPING (0x05). Usually 'bnet'.
 * 
 * Not responding will give you a UDP Plug in chat (flag 0x10).
 * 
 * You should send this anytime between S>C 0x50 SID_AUTH_INFO (or equivalent) and
 * C>S 0x0A SID_ENTERCHAT. Sending this packet before or after these two packets
 * will get you disconnected from official servers.
 *
 */
struct request {
  uint32_t udpcode;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x14, packet_bnetd_sid_udppingresponse);
 *
 */
void packet_bnetd_sid_udppingresponse(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.udpcode = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x14);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_udppingresponse not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
