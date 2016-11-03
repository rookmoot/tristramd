#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Eina.h>

#include "net.h"
#include "protocol.h"
#include "packet.h"
#include "sql.h"

#include "chat.h"

/*
 * Packet ID: 0x0C
 * Packet Name: sid_joinchannel
 *
 * Direction: Client to Server
 * Used By: Diablo II, Diablo II Lord of Destruction
 *
 * Format : 
 * (DWORD) Flags
 * (STRING) Channel
 * 
 * 
 * Remarks : 
 * Joins a channel after entering chat.
 * 
 * The Channel name must be no greater than 31 characters, otherwise it becomes
 * trimmed by Battle.net.
 * 
 * The flags field may contain the following values:
 * 0x00: NoCreate join
 * 0x01: First join
 * 0x02: Forced join
 * 0x05: D2 first join
 * 
 * <dt>NoCreate Join:</dt>
 * <dd>This will only join the channel specified if it is not empty, and is used by
 * clients when selecting a channel from the channels menu. If the channel is
 * empty, Battle.net sends a SID_CHATEVENT of type EID_CHANNELDOESNOTEXIST, upon
 * which official clients prompt for confirmation that the user wishes to create
 * the channel, in which case, it resends this packet with Flags set to Forced Join
 * (0x02).</dd>
 * <dt>First Join:</dt>
 * <dd>Places user in a channel starting with their product and country, followed
 * by a number, ie 'Brood War GBR-1'. Also automatically sends MOTD after entering
 * the channel. When using this type, the Channel variable has no effect, but must
 * be present anyway to avoid an IP ban. This is sent when first logging onto
 * Battle.net</dd>
 * <dt>Forced Join:</dt>
 * <dd>This is sent when leaving a game, and joins the specified channel without an
 * supplying an MOTD.</dd>
 * <dt>D2 First Join:</dt>
 * <dd>The same as First join, but is used for D2DV/D2XP clients.</dd></dl>
 *
 */
struct request {
  uint32_t flags;
  char * channel;
} __attribute__((packed));

/*
 *
 *
 * In order to enable the packet manager to manage this request handler,
 * you have to put the following line in the protocol.c file.
 *
 * PROTOCOL_REGISTER(PROTOCOL_BNETD, 0x0C, packet_bnetd_sid_joinchannel);
 *
 */
void packet_bnetd_sid_joinchannel(struct connection *conn EINA_UNUSED, struct packet *request) {
  struct request packet;

  struct channel *channel;
  
  memset(&packet, 0, sizeof(packet));
  packet.flags = packet_get_int(request, ENDIAN_LITTLE);
  packet.channel = packet_get_string(request, ENDIAN_LITTLE);

  channel = chat_channel_find(packet.channel);
  if (channel) {
    chat_channel_account_add(channel, conn->account);
  }

  free(packet.channel);  
}
