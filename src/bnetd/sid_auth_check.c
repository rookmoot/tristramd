#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x51
 * Packet Name: sid_auth_check
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD)   Client Token
 * (DWORD)   EXE Version
 * (DWORD)   EXE Hash
 * (DWORD)   Number of CD-keys in this packet
 * (BOOLEAN) Spawn CD-key **
 * 
 * For Each Key:
 * (DWORD) Key Length
 * (DWORD) CD-key's product value
 * (DWORD) CD-key's public value
 * (DWORD) Unknown (0)
 * (DWORD) [5] Hashed Key Data
 * 
 * (STRING) EXE Information
 * (STRING) CD-Key owner name *
 * 
 * Remarks : 
 * Contains the EXE Version and Hash as reported by `CheckRevision()` and CDKey
 * values.
 * 
 * The data that should be hashed for `Hashed Key Data` is:
 * 1. Client Token
 * 2. Server Token
 * 3. Key Product (from decoded CD key)
 * 4. Key Public (from decoded CD key)
 * 5. `DWORD` `0x00`
 * 6. Key Private (from decoded CD key)
 * 
 * The data that should be used for EXE Information should be separated by one
 * space, in the format of:
 * 1. EXE Name (ex. `war3.exe`)
 * 2. Last Modified Date (ex. `08/16/09`)
 * 3. Last Modified Time (ex. `19:21:59`)
 * 4. Filesize in bytes (ex. `471040`)
 * 
 * An example of a valid string would be: `war3.exe 08/16/09 19:21:59 471040`
 * 
 * \* The CD Key owner name must be no greater than 15 characters, otherwise it
 * becomes trimmed by Battle.net.
 * 
 * \** This `BOOLEAN` value is actually a `DWORD`, not a `BYTE`. You will be
 * IP-banned for 5 minutes if you assume `BYTE` and not use the proper `DWORD`.
 * Also, this value can only be TRUE for STAR, JSTR, and W2BN. Any other game will
 * cause unexpected results.
 *
 */
struct request {
  uint32_t client_token;
  uint32_t exe_version;
  uint32_t exe_hash;
  uint32_t number_of_cd_keys_in_this_packet;
  uint8_t spawn_cd_key;
  uint32_t key_length;
  uint32_t cd_keys_product_value;
  uint32_t cd_keys_public_value;
  uint32_t unknown;
  uint32_t hashed_key_data;
  char * exe_information;
  char * cd_key_owner_name;
} __attribute__((packed));


/*
 * Packet ID: 0x51
 * Packet Name: sid_auth_check
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Result
 * (STRING) Additional Information
 * 
 * Remarks : 
 * Reports success/failure on version & CD Key check.
 * 
 * Result:
 * 0x000: Passed challenge
 * 0x100: Old game version (Additional info field supplies patch MPQ filename)
 * 0x101: Invalid version
 * 0x102: Game version must be downgraded
 * (Additional info field supplies patch MPQ filename)
 * 0x0NN: (where NN is the version code supplied in SID_AUTH_INFO): Invalid version
 * code (note that 0x100 is not set in this case).
 * 0x200: Invalid CD key *
 * 0x201: CD key in use (Additional info field supplies name of user)
 * 0x202: Banned key
 * 0x203: Wrong product
 * The last 4 codes also apply to the second CDKey, as indicated by a bitwise
 * combination with 0x010.
 * 
 * If a patch file cannot be found, additional info is set to 'non-existent'. If
 * either the executable size/date or the version code is wrong, the server will
 * typically return a failure status.
 * 
 * * If you receive this status, official Battle.net servers ban you for 24 hours
 * (as of 06/15/2011). It used to be 2 weeks, but they changed recently.
 * 
 * More:
 * While doing a little research on how Battle.net autoupdates it's clients, I
 * found that it (STAR, anyway) does not use the provisions of the SID_AUTH login
 * system to tell clients which update file to download. Instead, it sends a
 * SID_REPORTVERSION (from the previous logon system) containing the equivalent
 * error code and the name of the patch file to download. This seems strange, and
 * makes me think that the part of Battle.net responsible for updating clients is
 * isolated from the part responsible for processing logon requests. If this is the
 * case, it makes sense that that system was never updated, since it must still
 * support legacy clients. In addition, this would explain why most Blizzard
 * clients retain obsolete packet processing code.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x51, packet_bnetd_sid_auth_check);
 *
 */
void packet_bnetd_sid_auth_check(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.client_token = packet_get_int(request, ENDIAN_LITTLE);
  packet.exe_version = packet_get_int(request, ENDIAN_LITTLE);
  packet.exe_hash = packet_get_int(request, ENDIAN_LITTLE);
  packet.number_of_cd_keys_in_this_packet = packet_get_int(request, ENDIAN_LITTLE);
  packet.spawn_cd_key = packet_get_byte(request, ENDIAN_LITTLE);
  packet.key_length = packet_get_int(request, ENDIAN_LITTLE);
  packet.cd_keys_product_value = packet_get_int(request, ENDIAN_LITTLE);
  packet.cd_keys_public_value = packet_get_int(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);
  packet.hashed_key_data = packet_get_int(request, ENDIAN_LITTLE);
  packet.exe_information = packet_get_string(request, ENDIAN_LITTLE);
  packet.cd_key_owner_name = packet_get_string(request, ENDIAN_LITTLE);


  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  packet_message_id_set(response, 0x51);
  packet_set_int(response, ENDIAN_LITTLE, 0x00);

  //  printf("CONNECTED WITH CLIENT VERSION : %d %d\n", packet.exe_version, packet.exe_hash);
  
  free(packet.cd_key_owner_name);
  free(packet.exe_information);
  
  net_connection_response_append(conn, response);
}
