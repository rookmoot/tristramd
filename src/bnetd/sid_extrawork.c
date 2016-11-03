#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x4B
 * Packet Name: sid_extrawork
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (WORD) Game type
 * (WORD) Length
 * (STRING) Work returned data
 * 
 * 
 * Remarks : 
 * Response for both SID_OPTIONALWORK and SID_REQUIREDWORK.
 * 
 * Game type:
 * <dl><dd>0x01: Diablo II</dd>
 * <dd>0x02: Warcraft III</dd>
 * <dd>0x03: Starcraft</dd>
 * <dd>0x04: World of Warcraft (Reason for this is not known, but most recent
 * libraries have included it)</dd></dl>
 * 
 * Length:<dl><dd>The length is returned from the call to ExtraWork in the
 * ExtraWork DLL. Traditionally, the library responsible for all ExtraWork requests
 * has been IX86ExtraWork.dll.</dd></dl>
 * Work returned data:<dl><dd>This data is based on a 1024-byte buffer. The call to
 * ExtraWork takes in a structure and returns the length and buffer based on the
 * game type.</dd></dl>
 *
 */
struct request {
  uint16_t game_type;
  uint16_t length;
  char * work_returned_data;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x4B, packet_bnetd_sid_extrawork);
 *
 */
void packet_bnetd_sid_extrawork(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.game_type = packet_get_short(request, ENDIAN_LITTLE);
  packet.length = packet_get_short(request, ENDIAN_LITTLE);
  packet.work_returned_data = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x4B);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_extrawork not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
