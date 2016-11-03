#include <stdio.h>
#include <stdlib.h>
#include <Eina.h>

#include "tristram.h"
#include "character.h"
#include "account.h"
#include "chat.h"

Eina_Hash *_channels;

void chat_init(void) {
  _channels = eina_hash_string_superfast_new((void (*)(void *))chat_channel_free);
  if (!_channels) {
    EINA_LOG_CRIT("Failed to create channel hash table.");
    exit(-1);
  }

  // adding default channels
  chat_channel_new("Diablo II", CHANNEL_PUBLIC | CHANNEL_SILENT, 65535);
  chat_channel_new("Heroes of Tristram", CHANNEL_PUBLIC, 1000);
}

void chat_shutdown(void) {
  eina_hash_free(_channels);
}

struct channel *chat_channel_new(const char *name, unsigned short flags, unsigned short limit) {
  struct channel *channel = NULL;

  channel = malloc(sizeof(*channel));
  if (!channel) {
    EINA_LOG_CRIT("Failed to alloc channel memory.");
    return NULL;
  }
  memset(channel, 0, sizeof(*channel));
  
  channel->name = strdup(name);
  channel->flags = flags;
  channel->limit = limit;
  channel->accounts = NULL;

  EINA_LOG_INFO("Adding new Chat Channel : %s", channel->name);
  eina_hash_add(_channels, channel->name, channel);
  
  return channel;
}

void chat_channel_free(struct channel *channel) {
  eina_list_free(channel->accounts);
  free(channel->name);
  free(channel);
}

const Eina_Hash *chat_channel_list_get(void) {
  return (const Eina_Hash *)_channels;
}

struct channel *chat_channel_find(const char *name) {
  struct channel *channel = NULL;

  channel = eina_hash_find(_channels, name);
  if (!channel) {
    channel = chat_channel_new(name, CHANNEL_PUBLIC, 1000);
  }
  return channel;
}

int chat_channel_account_add(struct channel *channel, struct account *account) {
  Eina_List *l = NULL;
  struct account *member = NULL;
  char *name = NULL, *statstring = NULL;

  strcpy(account->channel, channel->name);
  channel->accounts = eina_list_append(channel->accounts, account);

  name = account_chat_name_get(account, 0);
  chat_event_send(EID_CHANNEL, account, channel->flags & ~CHANNEL_ALL, name, channel->name);
  free(name);
  
  EINA_LIST_FOREACH(channel->accounts, l, member) {
    name = account_chat_name_get(member, 1);
    statstring = account_chat_statstring_get(member, 1);
    chat_event_send(EID_SHOWUSER, account, account_chat_flags_get(member), name, statstring);
    EINA_LOG_INFO("EID_SHOWUSER: %s %s", account_chat_name_get(account, 1), name);
    free(name);
    free(statstring);

    if (account != member) {
      name = account_chat_name_get(account, 1);
      statstring = account_chat_statstring_get(account, 1);
      chat_event_send(EID_JOIN, member, account_chat_flags_get(account), name, statstring);
      EINA_LOG_INFO("EID_JOIN: %s %s", account_chat_name_get(member, 1), name);
      free(name);
      free(statstring);
    }
  }
  return 1;
}


int chat_channel_account_remove(struct account *account) {
  struct channel *channel;
  Eina_List *l = NULL;
  struct account *member = NULL;
  char *name;

  if (!strlen(account->channel)) {
    return 0;
  }
  
  EINA_LOG_INFO("Account %ld leaving channel name : %s", account->id, account->channel);
  
  channel = chat_channel_find((const char *)account->channel);
  channel->accounts = eina_list_remove(channel->accounts, account);
  memset(account->channel, 0, 128);

  EINA_LIST_FOREACH(channel->accounts, l, member) {
    name = account_chat_name_get(account, 1);
    chat_event_send(EID_LEAVE, member, account_chat_flags_get(account), name, "");
    free(name);
  }

  return 1;
}

int chat_event_send(int event_id, struct account *to, unsigned int flags, const char *username, const char *data) {
  struct packet *response = NULL;
  
  response = packet_new(PROTOCOL_BNETD);
  if (!response) {
    return 0;
  }
  packet_message_id_set(response, 0x0F);

  packet_set_int(response, ENDIAN_LITTLE, event_id);
  packet_set_int(response, ENDIAN_LITTLE, flags); // flags
  packet_set_int(response, ENDIAN_LITTLE, 0x00);  // ping
  packet_set_int(response, ENDIAN_LITTLE, 0x00);  // ip
  packet_set_int(response, ENDIAN_LITTLE, 0x00);  // account number
  packet_set_int(response, ENDIAN_LITTLE, 0x00);  //  Registration Authority
  packet_set_string(response, ENDIAN_LITTLE, username); // username
  packet_set_string(response, ENDIAN_LITTLE, data); // data

  connection_response_append(account_connection_get(to), response);
  return 1;
}
