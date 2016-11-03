#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x6D
 * Packet Name: d2gs_ping
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Tick Count
 * (DWORD) Null
 * (DWORD) Null
 * 
 * 
 * Remarks : 
 * This packet should be sent every five to seven seconds to avoid timeout.
 * 
 * Please note: This message's official name is not known, and has been invented.
 *
 */
struct request {
  uint32_t tick_count;
  uint32_t null;
  uint32_t null2;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_D2GS, 0x6D, packet_d2gs_d2gs_ping);
 *
 */
void packet_d2gs_d2gs_ping(struct connection *conn EINA_UNUSED, struct packet *request) {
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.tick_count = packet_get_int(request, ENDIAN_LITTLE);
  packet.null = packet_get_int(request, ENDIAN_LITTLE);
  packet.null2 = packet_get_int(request, ENDIAN_LITTLE);
}
