#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x50
 * Packet Name: sid_auth_info
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Protocol ID *
 * (DWORD) Platform ID
 * (DWORD) Product ID
 * (DWORD) Version Byte
 * (DWORD) Product language **
 * (DWORD) Local IP **
 * (DWORD) Time zone bias **
 * (DWORD) Locale ID **
 * (DWORD) Language ID **
 * (STRING) Country abbreviation
 * (STRING) Country
 * 
 * Remarks : 
 * Sends information about the Client to Battle.net.
 * 
 * \* This field has only been seen with a value of `0x00`.
 * 
 * \** Battle.net will allow these fields to be set to `0x00`.
 * 
 * **Protocol ID:**
 * - Battle.net's current Protocol ID is `0x00`.
 * 
 * **Platform ID:**
 * - Identifies the client's platform.
 * - See [Platform & Product IDs &
 * VerBytes](/document/12/platform-product-ids-verbytes) for more info.
 * 
 * **Product ID:**
 * - Identifies the client's product (game).
 * - See [Platform & Product IDs &
 * VerBytes](/document/12/platform-product-ids-verbytes) for more info.
 * 
 * **Version Byte:**
 * - Identifies the client's version byte.
 * - See [Platform & Product IDs &
 * VerBytes](/document/12/platform-product-ids-verbytes) for more info.
 * 
 * **Product Language:**
 * - This field is under investigation and can be safely left as `0x00`.
 * 
 * **MPQ Locale ID:**
 * - This field is part of Blizzard's multi-lingual MPQ system and is used to
 * specify which version of an MPQ should be used when the MPQ is available in
 * multiple languages.
 * 
 * **Local IP:**
 * - This is the local network IP of the client, in network byte (big-endian)
 * order.
 * - This is for NAT compatibility.
 * 
 * **Timezone Bias:**
 * - The difference, in minutes, between UTC and local time. The client calculates
 * this value by subtracting the `Local Time` from the `System Time`, having
 * converted both to `FILETIME` structures, and then converting the resultant
 * offset to minutes by dividing it by 600,000,000.
 * - If you wish to understand the mechanism involved, read Microsoft's
 * documentation on file times.
 * 
 * **Language ID, Country Abbreviation, and Country:**
 * - These values can be retrieved by querying the system's locale information.
 * - Language ID can be retrieved using the
 * [GetUserDefaultLangID](https://msdn.microsoft.com/en-us/library/dd318134.aspx)
 * API.
 * - Country Abbreviation &amp; Country can be retrieved using the
 * [GetLocaleInfo](https://msdn.microsoft.com/en-us/library/dd318101.aspx) API to
 * request the `LOCALE_SABBREVCTRYNAME` and `LOCALE_SENGCOUNTRY`, respectively.
 *
 */
struct request {
  uint32_t protocol_id;
  uint32_t platform_id;
  uint32_t product_id;
  uint32_t version_byte;
  uint32_t product_language;
  uint32_t local_ip;
  uint32_t time_zone_bias;
  uint32_t locale_id;
  uint32_t language_id;
  char * country_abbreviation;
  char * country;
} __attribute__((packed));


/*
 * Packet ID: 0x50
 * Packet Name: sid_auth_info
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Logon Type
 * (DWORD) Server Token
 * (DWORD) UDPValue *
 * (FILETIME) MPQ filetime
 * (STRING) IX86ver filename
 * (STRING) ValueString
 * 
 * WAR3/W3XP Only:
 * (VOID) 128-byte Server signature
 * 
 * Remarks : 
 * Contains the Server Token, and the values used in CheckRevision.
 * 
 * Possible Logon Type values:<dl>
 * <dd>0x00: Broken SHA-1 (STAR/SEXP/D2DV/D2XP)</dd>
 * <dd>0x01: NLS version 1 (War3Beta)</dd>
 * <dd>0x02: NLS Version 2 (WAR3/W3XP)</dd></dl>
 * * UDPValue
 * No one really knows what this is, however, it is used in 2nd DWORD of the UDP
 * packet PKT_CONNTEST2. It is also the second part of MCP Chunk 1 in MCP_STARTUP.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x50, packet_bnetd_sid_auth_info);
 *
 */
void packet_bnetd_sid_auth_info(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.protocol_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.platform_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.version_byte = packet_get_int(request, ENDIAN_LITTLE);
  packet.product_language = packet_get_int(request, ENDIAN_LITTLE);
  packet.local_ip = packet_get_int(request, ENDIAN_LITTLE);
  packet.time_zone_bias = packet_get_int(request, ENDIAN_LITTLE);
  packet.locale_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.language_id = packet_get_int(request, ENDIAN_LITTLE);
  packet.country_abbreviation = packet_get_string(request, ENDIAN_LITTLE);
  packet.country = packet_get_string(request, ENDIAN_LITTLE);

  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  packet_message_id_set(response, 0x50);
  packet_set_int(response, ENDIAN_LITTLE, 0x00);
  packet_set_int(response, ENDIAN_LITTLE, 0x00001234);
  packet_set_int(response, ENDIAN_LITTLE, 0x01);
  packet_set_long(response, ENDIAN_LITTLE, 0x0004e29282b8d101);
  packet_set_string(response, ENDIAN_LITTLE, "ver-IX86-5.mpq");
  packet_set_string(response, ENDIAN_LITTLE, "B=1114042892 A=3449106154 C=1069230014 4 A=A-S B=B-C C=C-A A=A^B");

  free(packet.country_abbreviation);
  free(packet.country);

  connection_response_append(conn, response);
}
