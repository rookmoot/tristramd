#ifndef NET_H_
#define NET_H_

#include <netinet/in.h>

enum protocol {
  PROTOCOL_NONE            = 0,
  PROTOCOL_BNETD           = 1,
  PROTOCOL_BNFTP           = 2,
  PROTOCOL_REALM           = 3,
  PROTOCOL_D2GS            = 4,
  PROTOCOL_D2GS_COMPRESSED = 5,
};

enum socket_type {
  SOCKET_TYPE_TCP = 0,
  SOCKET_TYPE_UDP = 1,
};

enum connection_state {
  CONNECTION_STATE_CONNECTED    = 0,
  CONNECTION_STATE_DIAL         = 1,
  CONNECTION_STATE_DISCONNECTED = 2,
};

struct sockinfo {
  char ip[INET6_ADDRSTRLEN];
  unsigned short port;
};

struct connection {
  int socket;
  
  uint32_t uuid;
  struct sockinfo info;

  enum connection_state state;
  enum protocol protocol;

  struct server *srv;
  Eina_List *queue;
  
  struct account *account;
};

struct server {
  int socket;
  enum socket_type type;
  struct sockinfo info;

  char *name;
  char *description;
  unsigned short port;
  unsigned int max_conns;

  Eina_List *conns;
};

struct net {
  Eina_List *sockets;
};

#include "packet.h"


struct net *net_new(void);
void net_free(struct net *net);

Eina_List *net_all_connections_get(void);

struct server *net_bind(struct net *net, enum socket_type type, const char *name, const char *desc, const char *port, unsigned int max_conns);

struct connection *net_server_accept(struct server *srv);
void net_connection_free(struct connection *conn);

uint32_t net_connection_uuid_get(struct connection *conn);
struct connection *net_connection_clone(struct connection *conn, uint32_t uuid);

const char *net_connection_ip_get(struct connection *conn);
unsigned short net_connection_port_get(struct connection *conn);

void connection_response_append(struct connection *conn, struct packet *response);
struct packet *connection_response_get(struct connection *conn);

#endif
