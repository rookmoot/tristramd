#ifndef CHAT_H_
#define CHAT_H_

#define CHANNEL_PUBLIC        0x00001 // Public Channel
#define CHANNEL_MODERATED     0x00002 // Moderated
#define CHANNEL_RESTRICTED    0x00004 // Restricted
#define CHANNEL_SILENT        0x00008 // Silent
#define CHANNEL_SYSTEM        0x00010 // System
#define CHANNEL_PRODUCT       0x00020 // Product-Specific
#define CHANNEL_GLOBAL_ACCESS 0x01000 // Globally Accessible
#define CHANNEL_ALL           (CHANNEL_PUBLIC | CHANNEL_MODERATED | CHANNEL_RESTRICTED | \
			       CHANNEL_SILENT | CHANNEL_SYSTEM | CHANNEL_PRODUCT | CHANNEL_GLOBAL_ACCESS)

#define EID_SHOWUSER               0x01  // EID_SHOWUSER // User in channel
#define EID_JOIN                   0x02  // EID_JOIN // User joined channel
#define EID_LEAVE                  0x03  // EID_LEAVE // User left channel
#define EID_WHISPER                0x04  // EID_WHISPER // Recieved whisper
#define EID_TALK                   0x05  // EID_TALK // Chat text
#define EID_BROADCAST              0x06  // EID_BROADCAST // Server broadcast
#define EID_CHANNEL                0x07  // EID_CHANNEL // Channel information
#define EID_USERFLAGS              0x09  // EID_USERFLAGS // Flags update
#define EID_WHISPERSENT            0x0A  // EID_WHISPERSENT // Sent whisper
#define EID_CHANNELFULL            0x0D  // EID_CHANNELFULL // Channel full
#define EID_CHANNELDOESNOTEXIST    0x0E  // EID_CHANNELDOESNOTEXIST // Channel doesn't exist
#define EID_CHANNELRESTRICTED      0x0F  // EID_CHANNELRESTRICTED // Channel is restricted
#define EID_INFO                   0x12  // EID_INFO // Information
#define EID_ERROR                  0x13  // EID_ERROR // Error message
#define EID_IGNORE                 0x15  // EID_IGNORE // Notifies that a user has been ignored (DEFUNCT)
#define EID_ACCEPT                 0x16  // EID_ACCEPT // Notifies that a user has been unignored (DEFUNCT)
#define EID_EMOTE                  0x17  // EID_EMOTE // Emote

struct channel {
  char *name;
  unsigned short flags;
  unsigned short limit;
  Eina_List *accounts;
};

void chat_init(void);
void chat_shutdown(void);

struct channel *chat_channel_new(const char *name, unsigned short flags, unsigned short limit);
void chat_channel_free(struct channel *channel);
const Eina_Hash *chat_channel_list_get(void);

struct channel *chat_channel_find(const char *name);
int chat_channel_account_add(struct channel *channel, struct account *account);
int chat_channel_account_remove(struct account *account);

int chat_event_send(int event_id, struct account *to, unsigned int flags, const char *username, const char *data);

#endif
