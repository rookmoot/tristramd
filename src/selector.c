#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

#include <Eina.h>
#include "net.h"
#include "selector.h"

struct selector *selector_new(void) {
  struct selector *selector = NULL;

  selector = malloc(sizeof(*selector));
  if (!selector) {
    EINA_LOG_CRIT("Failed to allocate selector memory.");
    return NULL;
  }
  memset(selector, 0, sizeof(*selector));

  selector->maxfd = 0;
  FD_ZERO(&(selector->master));
  FD_ZERO(&(selector->read_fds));
  FD_ZERO(&(selector->write_fds));
  
  return selector;
}

void selector_free(struct selector *selector) {
  eina_list_free(selector->conns);
  free(selector);
}

void selector_server_add(struct selector *selector, struct server *server) {
  selector->maxfd = server->socket > selector->maxfd ? server->socket : selector->maxfd;
  FD_SET(server->socket, &(selector->master));  
}

void selector_connection_add(struct selector *selector, struct connection *conn) {
  selector->maxfd = conn->socket > selector->maxfd ? conn->socket : selector->maxfd;
  selector->conns = eina_list_append(selector->conns, conn);
  FD_SET(conn->socket, &(selector->master));
}

void selector_connection_remove(struct selector *selector, struct connection *conn) {
  selector->conns = eina_list_remove(selector->conns, conn);
  FD_CLR(conn->socket, &(selector->master));
}

int selector_select(struct selector *selector) {
  unsigned char can_write = 0;
  Eina_List *l = NULL;
  struct connection *conn = NULL;
  
  FD_ZERO(&(selector->read_fds));
  FD_ZERO(&(selector->write_fds));
  
  selector->read_fds = selector->master;
  selector->write_fds = selector->master;

  EINA_LIST_FOREACH(selector->conns, l, conn) {
    if (eina_list_count(conn->queue)) {
      can_write = 1;
      break;
    }
  }
  return select((selector->maxfd) + 1, &(selector->read_fds), (can_write ? &(selector->write_fds) : NULL), NULL, NULL);
}

int selector_server_can_accept(struct selector *selector, struct server *server) {
  if (!server) { return 0; }
  return FD_ISSET(server->socket, &(selector->read_fds));
}

int selector_connection_can_read(struct selector *selector, struct connection *conn) {
  if (!conn) { return 0; }
  return FD_ISSET(conn->socket, &(selector->read_fds));
}

int  selector_connection_can_write(struct selector *selector, struct connection *conn) {
  EINA_SAFETY_ON_NULL_RETURN_VAL(conn, 0);

  return FD_ISSET(conn->socket, &(selector->write_fds));
}
