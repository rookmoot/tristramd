#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x17
 * Packet Name: sid_readmemory
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Request ID
 * (VOID) Memory
 * 
 * Remarks : 
 * Rudimentary hack detection system. Was never used probably due to terrible
 * implementation with little security.
 * 
 * Yes, it is possible for a PvPGN server to read <strong>EVERYTHING</strong> that
 * is in the process' memory, including sensitive information such as your CDKey.
 * 
 * Found at:
 * battle!1901D470h (as of 1.16.1)
 *
 */
struct request {
  uint32_t request_id;
  void * memory;
} __attribute__((packed));


/*
 * Packet ID: 0x17
 * Packet Name: sid_readmemory
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Request ID
 * (DWORD) Address
 * (DWORD) Length
 * 
 * Remarks : 
 * Rudimentary hack detection system. Was never used probably due to terrible
 * implementation with little security.
 * 
 * Yes, it is possible for a PvPGN server to read <strong>EVERYTHING</strong> that
 * is in the process' memory, including sensitive information such as your CDKey.
 * 
 * Found at:
 * battle!1901D470h (as of 1.16.1)
 * 
 * NOTE: No longer used, as Warden is now used to monitor for hacks. The Battle.net
 * clients still support this though so one can code in support for it if one so
 * desires. Due to it being no longer actively used by Battle.net, this packet is
 * being marked as defunct. 
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x17, packet_bnetd_sid_readmemory);
 *
 */
void packet_bnetd_sid_readmemory(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.request_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.memory = (char *)packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x17);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_readmemory not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
