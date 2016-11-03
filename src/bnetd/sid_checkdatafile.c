#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x32
 * Packet Name: sid_checkdatafile
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) [5] File checksum
 * (STRING) File name
 * 
 * Remarks : 
 * This message was used to check a digest of a game file. This message is no
 * longer used; developers should use the SID_CHECKDATAFILE2 message.
 * 
 * The digest is created by using the broken SHA-1 hash on the first 64-bytes of
 * the (filesize % 64) times. This due to a bug in the client. 
 *
 */
struct request {
  uint32_t file_checksum;
  char * file_name;
} __attribute__((packed));


/*
 * Packet ID: 0x32
 * Packet Name: sid_checkdatafile
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Status
 * 
 * Remarks : 
 * This message is no longer used; developers should use the SID_CHECKDATAFILE2
 * message.
 * <dl><dt>Status:</dt>
 * <dd>0x00: Rejected</dd>
 * <dd>0x01: Approved</dd>
 * <dd>0x02: Ladder approved</dd></dl>
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x32, packet_bnetd_sid_checkdatafile);
 *
 */
void packet_bnetd_sid_checkdatafile(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.file_checksum = packet_get_int(request, ENDIAN_LITTLE);
  packet.file_name = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x32);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_checkdatafile not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
