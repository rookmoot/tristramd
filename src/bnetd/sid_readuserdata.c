#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x26
 * Packet Name: sid_readuserdata
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of Accounts
 * (DWORD) Number of Keys
 * (DWORD) Request ID
 * (STRING)[] Requested Accounts
 * (STRING)[] Requested Keys
 * 
 * Remarks : 
 * Requests an extended profile.
 * 
 * Please note that if you provide more than one account, Battle.net will return
 * your request as if you sent it with 0 accounts and 0 keys, indicating that no
 * data is present other than the 3 DWORDs seen here. This means you cannot use
 * this packet for multiple users all at once; you must submit this packet once for
 * each user you wish to know about. This only applies to official Battle.net
 * servers, it might not apply to PvPGN servers or equivalent software.
 * 
 * Also, requesting 0x20 (32) or more keys at one time makes Battle.net kick you
 * (it does not ban you, however).
 * 
 * <dl>
 * <dt>Profile Keys: *</dt>
 * <dt><b>User Profiles:</b></dt>
 * <dd>profile\sex **</dd>
 * <dd>profile\age † </dd>
 * <dd>profile\location</dd>
 * <dd>profile\description</dd>
 * 
 * <dt><b>Account Info: ***</b></dt>
 * <dd>System\Account Created</dd>
 * <dd>System\AuthLevel</dd>
 * <dd>System\Flags</dd>
 * <dd>System\Friends</dd>
 * <dd>System\Icon</dd>
 * <dd>System\IP</dd>
 * <dd>System\Last Logoff</dd>
 * <dd>System\Last Logon</dd>
 * <dd>System\League</dd>
 * <dd>System\MFT Bytes</dd>
 * <dd>System\MFT Last Write</dd>
 * <dd>System\MFT Time Logged</dd>
 * <dd>System\Port</dd>
 * <dd>System\Time Logged</dd>
 * <dd>System\Username</dd>
 * 
 * <dt><b>Normal Games:</b></dt>
 * <dd>record\GAME\0\wins</dd>
 * <dd>record\GAME\0\losses</dd>
 * <dd>record\GAME\0\disconnects</dd>
 * <dd>record\GAME\0\last game</dd>
 * <dd>record\GAME\0\last game result</dd>
 * 
 * <dt><b>Ladder Games:</b></dt>
 * <dd>record\GAME\1\wins</dd>
 * <dd>record\GAME\1\losses</dd>
 * <dd>record\GAME\1\disconnects</dd>
 * <dd>record\GAME\1\rating</dd>
 * <dd>record\GAME\1\high rating</dd>
 * <dd>DynKey\GAME\1\rank</dd>
 * <dd>record\GAME\1\high rank</dd>
 * <dd>record\GAME\1\last game</dd>
 * <dd>record\GAME\1\last game result</dd>
 * 
 * <dt><b>IronMan Ladder Games: ‡</b></dt>
 * <dd>record\GAME\3\wins</dd>
 * <dd>record\GAME\3\losses</dd>
 * <dd>record\GAME\3\disconnects</dd>
 * <dd>record\GAME\3\last game</dd>
 * <dd>record\GAME\3\last game result</dd>
 * <dd>record\GAME\3\rating</dd>
 * <dd>record\GAME\3\high rating</dd>
 * <dd>DynKey\GAME\3\rank</dd>
 * <dt>* This list is not complete, and could use adding to.</dt>
 * <dt>** This field is defunct in STAR/SEXP/WAR3/W3XP.</dt>
 * <dt>†  This field is defunct.</dt>
 * <dt>*** Battle.net returns blank values for anyone but yourself.</dt>
 * <dt>‡ W2BN only.</dt>
 * <dt>Note that any profile\ key can contain an 0x09 (tab) character.</dt>
 * </dl>
 *
 */
struct request {
  uint32_t number_of_accounts;
  uint32_t number_of_keys;
  uint32_t request_id;
  char * requested_accounts;
  char * requested_keys;
} __attribute__((packed));


/*
 * Packet ID: 0x26
 * Packet Name: sid_readuserdata
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Number of accounts
 * (DWORD) Number of keys
 * (DWORD) Request ID
 * (STRING) [] Requested Key Values
 * 
 * Remarks : 
 * Contains profile information as requested by the client.
 * 
 * If you plan to graphically display this data, note that the fields may contain
 * characters that are otherwise not alphanumeric, such as tab or enter keys. In
 * addition, some bots allow the user to put new lines into fields that the
 * official clients only allow one line for, such as a profile's Location.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x26, packet_bnetd_sid_readuserdata);
 *
 */
void packet_bnetd_sid_readuserdata(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.number_of_accounts = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_keys = packet_get_int(request, ENDIAN_LITTLE);
  packet.request_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.requested_accounts = packet_get_string(request, ENDIAN_LITTLE);
  packet.requested_keys = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x26);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_readuserdata not yet implemented."); packet_free(response); return;

  net_connection_response_append(conn, response);
}
