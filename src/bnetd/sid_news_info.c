#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x46
 * Packet Name: sid_news_info
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) News timestamp
 * 
 * Remarks : 
 * Requests news and MOTD from battle.net.
 * 
 * The news timestamp specifies the starting date for the news. To retrieve all
 * available news entries, set this to zero. Timestamps are given in C/Unix format
 * -- that is, the number of seconds since January 1, 1970 0:00:00.000 -- and
 * should be biased to UTC.
 * 
 * This message should be sent when you receive SID_ENTERCHAT. The official client
 * stops processing messages after the user joins a game or enters a channel, and
 * discards messages with more than 127 entries.
 * 
 * News can be requested for older products, but Battle.net will only return the
 * server's Message-of-the-Day. However, this behavior has not been observed in
 * official clients, and for an accurate protocol emulation, its use is not
 * recommended.
 *
 */
struct request {
  uint32_t news_timestamp;
} __attribute__((packed));


/*
 * Packet ID: 0x46
 * Packet Name: sid_news_info
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (BYTE) Number of entries
 * (DWORD) Last logon timestamp
 * (DWORD) Oldest news timestamp
 * (DWORD) Newest news timestamp
 * 
 * For each entry:<dl><dd>(DWORD) Timestamp</dd>
 * <dd>(STRING) News</dd></dl>
 * 
 * Remarks : 
 * This packet contains news from battle.net. Timestamps are in C/Unix format,
 * biased for UTC.
 * 
 * Multiple separate news messages can be received. These should be treated the
 * same as if one message was sent with several news entries.
 * 
 * If the 'Timestamp' field for an entry is zero, then that entry is the message of
 * the day (and not a news entry in the normal respect).
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x46, packet_bnetd_sid_news_info);
 *
 */
void packet_bnetd_sid_news_info(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.news_timestamp = packet_get_int(request, ENDIAN_LITTLE);

  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x46);

  packet_set_byte(response, ENDIAN_LITTLE, 0x00);
  packet_set_int(response, ENDIAN_LITTLE, time(NULL));
  packet_set_int(response, ENDIAN_LITTLE, time(NULL));
  packet_set_int(response, ENDIAN_LITTLE, time(NULL));
  
  net_connection_response_append(conn, response);
}
