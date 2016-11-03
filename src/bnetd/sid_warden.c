#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

/*
 * Packet ID: 0x5E
 * Packet Name: sid_warden
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (VOID) Encrypted Packet
 * <dl>
 * <dt>Contents of encrypted data</dt>
 * <dd>(BYTE) Packet Code</dd>
 * <dd>
 * <dl>
 * <dt>0x00 - Warden Module Info</dt>
 *    <dd>(BYTE) Success (0x00 = Don't have the module, 0x01 = Have the
 * module)</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x01 - Warden Module Data</dt>
 * <dd>(BYTE) Success (0x00 = MD5 doesn't match, 0x01 = MD5 matches)</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x02 - Data Checker</dt>
 * <dd>(WORD) String Length</dd>
 * <dd>(DWORD) String Checksum</dd>
 * <dd>
 * <dl>
 * <dt>(VOID) String Data</dt>
 * <dd>
 * <dl>
 * <dt>MEM_CHECK:</dd>
 * <dd>(BYTE) Success (0x00 = Read data, 0x01 = Unable to read)</dd>
 * <dd>(VOID) Data (0x00 only)</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>PAGE_CHECK_A:</dd>
 * <dd>(BYTE) Success (0x00 = SHA1s match, 0x01 = SHA1s don't match)</dd>
 * </dl>
 * </dd>
 * </dl>
 * </dd>
 * <dd>(BYTE) IDXor</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x04 - Initialization</dt>
 * <dd>(DWORD)[4] Unknown</dd>
 * </dl>
 * </dd>
 * </dl>
 * 
 * Remarks : 
 * The packet is encrypted via standard RC4 hashing, using one key for outbound
 * data and another for inbound. Its purpose is to return executable data and
 * checksum information from various Warden modules. Full information on how to
 * handle this packet may be found at the <a
 * href="http://forum.valhallalegends.com/index.php?topic=17356.0">Rudimentary
 * Warden information</a> topic.
 * 
 * Documentation provided by iago and Ringo.
 *
 */
struct request {
  void * encrypted_packet;
  uint8_t packet_code;
  uint8_t success;
  uint8_t success_198586996;
  uint16_t string_length;
  uint32_t string_checksum;
  void * string_data;
  uint8_t success_1843978632;
  void * data;
  uint8_t success_880307404;
  uint8_t idxor;
  uint32_t unknown;
} __attribute__((packed));


/*
 * Packet ID: 0x5E
 * Packet Name: sid_warden
 *
 * Direction: Server to Client
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (VOID) Encrypted Packet
 * <dl>
 * <dt>Contents of encrypted data</dt>
 * <dd>(BYTE) Packet Code</dd>
 * <dd>
 * <dl>
 * <dt>0x00 - Warden Module Info</dt>
 * <dd>(DWORD)[4] MD5 Hash of the current Module</dd>
 * <dd>(DWORD)[4] Decryption key for Module</dd>
 * <dd>(DWORD) Length of Module</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x01 - Warden Module Data</dt>
 * <dd>(WORD) Length of data (without 3-byte header)</dd>
 * <dd>(VOID) Data</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x02 - Data Checker</dt>
 * <dd>(BYTE) String Length (Usually 0)</dd>
 * <dd>(VOID) String Data</dd>
 * <dd>
 * <dl>
 * <dt>(BYTE) Check ID</dt>
 * <dd>
 * <dl>
 * <dt>MEM_CHECK:</dd>
 * <dd>(BYTE) String Index (Usually 0)</dd>
 * <dd>(DWORD) Address</dd>
 * <dd>(BYTE) Length to Read</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>PAGE_CHECK_A:</dd>
 * <dd>(DWORD) Unknown (Seed?)</dd>
 * <dd>(DWORD)[5] SHA1</dd>
 * <dd>(DWORD) Address</dd>
 * <dd>(BYTE) Length to Read</dd>
 * </dl>
 * </dd>
 * </dl>
 * </dd>
 * <dd>(BYTE) IDXor</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x03 - Library Loader</dt>
 * <dd>(WORD) Length of data (without 7-byte header)</dd>
 * <dd>(DWORD) Checksum of data (without 7-byte header)</dd>
 * <dd>(BYTE) Unknown (0x01)<dd>
 * <dd>(BYTE) Unknown (Usually 0x00)</dd>
 * <dd>(BYTE) Unknown (Usually 0x01)</dd>
 * <dd>(STRING) Library Name</dd>
 * <dd>(DWORD) Funct1</dd>
 * <dd>(DWORD) Funct2</dd>
 * <dd>(DWORD) Funct3</dd>
 * <dd>(DWORD) Funct4</dd>
 * </dl>
 * </dd>
 * <dd>
 * <dl>
 * <dt>0x05 - Initialization</dt>
 * <dd>(DWORD)[5] Unknown</dd>
 * </dl>
 * </dd>
 * </dl>
 * 
 * Remarks : 
 * This packet is received after successfully logging onto Battle.net and usually
 * after receiving the first initial chat events. If the client does not respond to
 * this packet, the client gets dropped two minutes later (give or take about 10
 * seconds).
 * 
 * The packet is encrypted via standard RC4 hashing, using one key for outbound
 * data and another for inbound. Its purpose is to download and execute Warden
 * modules. Full information on how to handle this packet may be found at the <a
 * href="http://forum.valhallalegends.com/index.php?topic=17356.0">Rudimentary
 * Warden information</a> topic.
 * 
 * Documentation provided by iago and Ringo.
 *
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x5E, packet_bnetd_sid_warden);
 *
 */
void packet_bnetd_sid_warden(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  struct request packet;

  memset(&packet, 0, sizeof(packet));
  packet.encrypted_packet = (char *)packet_get_string(request, ENDIAN_LITTLE);
  packet.packet_code = packet_get_byte(request, ENDIAN_LITTLE);
  packet.success = packet_get_byte(request, ENDIAN_LITTLE);
  packet.success = packet_get_byte(request, ENDIAN_LITTLE);
  packet.string_length = packet_get_short(request, ENDIAN_LITTLE);
  packet.string_checksum = packet_get_int(request, ENDIAN_LITTLE);
  packet.string_data = (char *)packet_get_string(request, ENDIAN_LITTLE);
  packet.success = packet_get_byte(request, ENDIAN_LITTLE);
  packet.data = (char *)packet_get_string(request, ENDIAN_LITTLE);
  packet.success = packet_get_byte(request, ENDIAN_LITTLE);
  packet.idxor = packet_get_byte(request, ENDIAN_LITTLE);
  packet.unknown = packet_get_int(request, ENDIAN_LITTLE);



  // create the response packet.
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return;
  }

  // set response message id.
  packet_message_id_set(response, 0x5E);

  // YOU NEED TO REMOVE THIS LINE FOR THE PACKET TO BE ENABLED.
  EINA_LOG_CRIT("packet_bnetd_sid_warden not yet implemented."); packet_free(response); return;

  connection_response_append(conn, response);
}
