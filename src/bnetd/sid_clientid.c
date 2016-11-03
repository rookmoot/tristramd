#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x05
 * Packet Name: sid_clientid
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Registration Version
 * (DWORD) Registration Authority
 * (DWORD) Account Number
 * (DWORD) Registration Token
 * (STRING) LAN Computer Name
 * (STRING) LAN Username
 * 
 * Remarks : 
 * This packet was used to ensure that the client's account number was valid. All
 * but the last two fields in this message are now ignored, and may be set to
 * zero.
 * 
 * The 'LAN Computer Name' field is the NetBIOS name of the computer. It can be
 * retrieved using the <a
 * href="http://msdn2.microsoft.com/en-us/library/ms724295.aspx">GetComputerName</a>
 * API.
 * 
 * The 'Lan Username' field is the name of the currently logged on user, and may be
 * retrieved using the <a
 * href="http://msdn2.microsoft.com/en-us/library/ms724432.aspx">GetUsername</a>
 * API.
 * 
 * The following information is historical:
 * 
 * The client would supply this data as issued by a Battle.net server. If the
 * Registration Version, Registration Authority, and Client Token values equated to
 * the account number supplied (Client ID), as determined by an unknown formula,
 * the server would respond with the same values. If they were invalid, the server
 * would assign new values. Registration Version was always 1, Authority was the IP
 * address of the server that issued the account number. Thus, the Client Token was
 * the secret value, used to prove that the client really owned the account in
 * question.
 *
 */
struct request {
  uint32_t registration_version;
  uint32_t registration_authority;
  uint32_t account_number;
  uint32_t registration_token;
  char * lan_computer_name;
  char * lan_username;
} __attribute__((packed));


/*
 * Packet ID: 0x05
 * Packet Name: sid_clientid
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Registration Version
 * (DWORD) Registration Authority
 * (DWORD) Account Number
 * (DWORD) Registration Token
 * 
 * Remarks : 
 * The following information is historical:
 * 
 * This message used to be used to issue new values to the client for the above
 * fields. New values were issued when the values supplied in SID_CLIENTID were
 * found to be invalid.
 * 
 * Since these fields are no longer used, the server now sets them to zero.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x05, packet_bnetd_sid_clientid);
 *
 */
void packet_bnetd_sid_clientid(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.registration_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_authority = packet_get_int(request, ENDIAN_LITTLE);
  packet.account_number = packet_get_int(request, ENDIAN_LITTLE);
  packet.registration_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.lan_computer_name = packet_get_string(request, ENDIAN_LITTLE);
  packet.lan_username = packet_get_string(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x05);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_clientid not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
