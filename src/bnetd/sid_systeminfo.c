#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x2B
 * Packet Name: sid_systeminfo
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of processors
 * (DWORD) Processor architecture
 * (DWORD) Processor level
 * (DWORD) Processor timing
 * (DWORD) Total physical memory
 * (DWORD) Total page file
 * (DWORD) Free disk space
 * 
 * Remarks : 
 * Contains system information. This packet was sent during the connection process
 * for STAR/SEXP/DRTL/DSHR clients prior to version 1.07. It is now only used by
 * JSTR and SSHR. For information on how to emulate this system, please see <a
 * href="http://forum.valhallalegends.com/index.php?topic=16255.0">this topic</a>.
 *
 */
struct request {
  uint32_t number_of_processors;
  uint32_t processor_architecture;
  uint32_t processor_level;
  uint32_t processor_timing;
  uint32_t total_physical_memory;
  uint32_t total_page_file;
  uint32_t free_disk_space;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x2B, packet_bnetd_sid_systeminfo);
 *
 */
void packet_bnetd_sid_systeminfo(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.number_of_processors = packet_get_int(request, ENDIAN_LITTLE);
  packet.processor_architecture = packet_get_int(request, ENDIAN_LITTLE);
  packet.processor_level = packet_get_int(request, ENDIAN_LITTLE);
  packet.processor_timing = packet_get_int(request, ENDIAN_LITTLE);
  packet.total_physical_memory = packet_get_int(request, ENDIAN_LITTLE);
  packet.total_page_file = packet_get_int(request, ENDIAN_LITTLE);
  packet.free_disk_space = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x2B);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_systeminfo not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
