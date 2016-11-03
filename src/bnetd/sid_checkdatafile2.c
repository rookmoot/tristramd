#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x3C
 * Packet Name: sid_checkdatafile2
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) File size in bytes
 * (DWORD) File hash [5]
 * (STRING) Filename
 * 
 * Remarks : 
 * Verifies that a file is authentic, by producing a hash of that file and sending
 * it to the server for comparison to the original.
 * 
 * The hash is a standard SHA1 hash.
 *
 */
struct request {
  uint32_t file_size_in_bytes;
  uint32_t file_hash;
  char * filename;
} __attribute__((packed));


/*
 * Packet ID: 0x3C
 * Packet Name: sid_checkdatafile2
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * 
 * Remarks : 
 * Result of file integrity query.
 * <dl><dt>Result:</dt>
 * <dd>0x00: Not approved</dd>
 * <dd>0x01: Blizzard approved</dd>
 * <dd>0x02: Approved for ladder</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x3C, packet_bnetd_sid_checkdatafile2);
 *
 */
void packet_bnetd_sid_checkdatafile2(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.file_size_in_bytes = packet_get_int(request, ENDIAN_LITTLE);
  packet.file_hash = packet_get_int(request, ENDIAN_LITTLE);
  packet.filename = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x3C);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_checkdatafile2 not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
