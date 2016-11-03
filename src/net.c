#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <Eina.h>
#include "net.h"
#include "sql.h"
#include "character.h"
#include "account.h"

static inline void *net_socket_get_in_addr(struct sockaddr *sa);
static inline int net_socket_informations_get(struct sockinfo *info, struct sockaddr *addr);

static Eina_List *_all_conns = NULL;

Eina_List *net_all_connections_get(void) {
  return _all_conns;
}

struct net *net_new(void) {
  struct net *net = NULL;

  net = malloc(sizeof(*net));
  if (!net) {
    EINA_LOG_CRIT("Failed to alloc net struct");
    return NULL;
  }
  memset(net, 0, sizeof(*net));
  return net;
}

void net_free(struct net *net) {
  struct server *srv = NULL;
  struct connection *conn = NULL;

  EINA_LIST_FREE(net->sockets, srv) {
    EINA_LIST_FREE(srv->conns, conn) {
      net_connection_free(conn);
    }
    free(srv);
  }
  free(net);
}

struct server *net_bind(struct net *net, enum socket_type type, const char *name, const char *desc, const char *port, unsigned int max_conns) {
  int rv, yes;
  struct server *srv = NULL;
  struct addrinfo hints, *ai, *p;
  //struct sockaddr_in addr;
  
  srv = malloc(sizeof(*srv));
  if (!srv) {
    EINA_LOG_CRIT("Failed to alloc socket struct\n");
    return NULL;
  }
  memset(srv, 0, sizeof(*srv));

  srv->name = (char *)name;
  srv->description = (char *)desc;
  srv->port = atoi(port);
  srv->max_conns = max_conns;

  
  // get us a socket and bind it
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  if (type == SOCKET_TYPE_TCP) {
    hints.ai_socktype = SOCK_STREAM;
  } else {
    hints.ai_socktype = SOCK_DGRAM;
  }
  hints.ai_flags = AI_PASSIVE;
  if ((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0) {
    EINA_LOG_ERR("Failed to select server: %s\n", gai_strerror(rv));
    return NULL;
  }

  for (p = ai; p != NULL; p = p->ai_next) {
    srv->socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (srv->socket < 0) {
      continue;
    }

    // lose the pesky "address already in use" error message
    setsockopt(srv->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (bind(srv->socket, p->ai_addr, p->ai_addrlen) < 0) {
      close(srv->socket);
      continue;
    }

    break;
  }

  // if we got here, it means we didn't get bound
  if (p == NULL) {
    EINA_LOG_ERR("selectserver: failed to bind\n");
    return NULL;
  }

  if (!net_socket_informations_get(&(srv->info), (struct sockaddr *)p->ai_addr)) {
    EINA_LOG_ERR("Failed to get socket information for server : %d", srv->port);
  }
  
  freeaddrinfo(ai); // all done with this
  
  // listen
  if (type == SOCKET_TYPE_TCP) {
    if (listen(srv->socket, 0) == -1) {
      EINA_LOG_ERR("selectserver: failed to listen\n");
      return NULL;
    }
  }

  EINA_LOG_DBG("Starting server %s on %s:%d", srv->name, srv->info.ip, srv->info.port);
  
  net->sockets = eina_list_append(net->sockets, srv);
  return srv;
}

struct connection *net_server_accept(struct server *srv) {
  socklen_t addrlen;
  struct sockaddr_storage addr;
  struct connection *conn = NULL;
  
  conn = malloc(sizeof(*conn));
  if (!conn) {
    EINA_LOG_CRIT("Failed to alloc connection memory.");
    return NULL;
  }

  memset(conn, 0, sizeof(*conn));
  addrlen = sizeof(addr);
  conn->socket = accept(srv->socket, (struct sockaddr *)&addr, &addrlen);
  if (conn->socket == -1) {
    EINA_LOG_ERR("Failed to accept new connection.");
    return NULL;
  }

  conn->uuid = (uint32_t)rand();
  conn->srv = srv;
  conn->state = CONNECTION_STATE_CONNECTED;
  srv->conns = eina_list_append(srv->conns, conn);
  _all_conns = eina_list_append(_all_conns, conn);
  
  if (!net_socket_informations_get(&(conn->info), (struct sockaddr *)&addr)) {
    EINA_LOG_ERR("Failed to get socket information for client : %d", conn->socket);
  }
    
  return conn;
}

void net_connection_free(struct connection *conn) {
  struct packet *packet = NULL;

  _all_conns = eina_list_remove(_all_conns, conn);

  if (conn->account != NULL) {
    //account_free(conn->account);
  }
  
  conn->srv->conns = eina_list_remove(conn->srv->conns, conn);
  EINA_LIST_FREE(conn->queue, packet) {
    packet_free(packet);
  }
  free(conn);
}

uint32_t net_connection_uuid_get(struct connection *conn) {
  return (uint32_t)conn->uuid;
}

struct connection *net_connection_clone(struct connection *conn, uint32_t uuid) {
  Eina_List *l;
  struct account *account = NULL;
  struct connection *found = NULL;

  EINA_LIST_FOREACH(_all_conns, l, found) {
    if (found->uuid == uuid) {
      break;
    }
  }

  if (!found) {
    EINA_LOG_CRIT("Failed to found a matching conn from uuid %d", uuid);
    return NULL;
  }

  if (!strncmp(conn->info.ip, found->info.ip, strlen(found->info.ip))) {
    conn->uuid = found->uuid;
    account = (struct account *)found->account;
    conn->account = (struct account *)account;
    return conn;
  }
  return NULL;
}

const char *net_connection_ip_get(struct connection *conn) {
  return (const char *)conn->info.ip;
}

unsigned short net_connection_port_get(struct connection *conn) {
  return conn->info.port;
}

void connection_response_append(struct connection *conn, struct packet *response) {
  conn->queue = eina_list_append(conn->queue, response);
}

struct packet *connection_response_get(struct connection *conn) {
  struct packet *packet = NULL;
  
  packet = (struct packet *)eina_list_data_get(conn->queue);
  if (packet) {
    conn->queue = eina_list_remove(conn->queue, (const void *)packet);
    return packet;
  }
  return NULL;
}

static inline int net_socket_informations_get(struct sockinfo *info, struct sockaddr *addr) {
  if (!inet_ntop(AF_INET, net_socket_get_in_addr(addr), info->ip, INET6_ADDRSTRLEN)) {
    EINA_LOG_ERR("Failed to get client ip");
  }
  info->port = ntohs(((struct sockaddr_in *)addr)->sin_port);
  return 1;
}

static inline void *net_socket_get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
