#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#define EINA_LOG_DOMAIN_DEFAULT _log_dom
static int _log_dom = -1; // must come before inclusion of Eina.h!
#include <Eina.h>

#include "tristram.h"
#include "sql.h"
#include "net.h"
#include "selector.h"
#include "packet.h"
#include "protocol.h"

static volatile int keepRunning = 1;

void intHandler() {
  keepRunning = 0;
}

int main(void) {
  int ret = 0;
  Eina_List *l = NULL, *l2 = NULL;
  struct net *net = NULL;
  struct server *srv = NULL;
  struct selector *selector = NULL;
  struct connection *conn = NULL;

  char *name;
  char *port;
  char *description;
  unsigned int max_connections;

  if (!tristram_init("etc/tristramd.cfg")) {
    return -1;
  }
  
  eina_log_level_set(EINA_LOG_LEVEL_DBG);
  _log_dom = eina_log_domain_register("bnetd", EINA_COLOR_CYAN);
  
  if (!(net = net_new())) {
    return -1;
  }

  if (!(selector = selector_new())) {
    return -1;
  }

  EINA_LOG_INFO("Starting up bnetd");
  
  // bind BNETD server
  CONFIG_GET_STRING("servers.bnetd.name", &name);
  CONFIG_GET_STRING("servers.bnetd.port", &port);
  CONFIG_GET_STRING("servers.bnetd.description", &description);
  CONFIG_GET_INT("servers.bnetd.max_connections", (int *)&max_connections);
  srv = net_bind(net, SOCKET_TYPE_TCP, name, description, port, max_connections);
  selector_server_add(selector, srv);

  CONFIG_GET_STRING("servers.realm.name", &name);
  CONFIG_GET_STRING("servers.realm.port", &port);
  CONFIG_GET_STRING("servers.realm.description", &description);
  CONFIG_GET_INT("servers.realm.max_connections", (int *)&max_connections);
  srv = net_bind(net, SOCKET_TYPE_TCP, name, description, port, max_connections);
  selector_server_add(selector, srv);

  CONFIG_GET_STRING("servers.realm.name", &name);
  CONFIG_GET_STRING("servers.realm.description", &description);
  CONFIG_GET_INT("servers.realm.max_connections", (int *)&max_connections);
  srv = net_bind(net, SOCKET_TYPE_TCP, name, description, "4000", max_connections);
  selector_server_add(selector, srv);

  signal(SIGINT, intHandler);
  while (keepRunning) {

    // selecting sockets
    if (selector_select(selector) >= 0) {
      EINA_LIST_FOREACH(net->sockets, l, srv) {
	srv = (struct server *)srv;
	if (selector_server_can_accept(selector, srv)) {
	  conn = net_server_accept(srv);
	  if (srv->info.port == 6113) {
	    conn->protocol = PROTOCOL_REALM;
	    conn->state = CONNECTION_STATE_DIAL;
	  } else if (srv->info.port == 4000) {
	    conn->protocol = PROTOCOL_D2GS;
	    conn->state = CONNECTION_STATE_DIAL;

	    // send welcome packet.
	    struct packet *p = packet_new(PROTOCOL_D2GS);
	    packet_message_id_set(p, 0xAF);
	    packet_set_byte(p, ENDIAN_LITTLE, 0x01);
	    net_connection_response_append(conn, p);
	    
	  } else {
	    conn->protocol = PROTOCOL_NONE;
	  }
	  selector_connection_add(selector, conn);
	  EINA_LOG_WARN("New client %d connected on ip : %s:%hu", conn->socket, net_connection_ip_get(conn), net_connection_port_get(conn));
	}

	EINA_LIST_FOREACH(srv->conns, l2, conn) {
	  if (selector_connection_can_read(selector, (struct connection *)conn)) {
	    struct packet *request = packet_new(conn->protocol);
	    if (request) {
	      ret = packet_read_from_connection(request, conn);
	      if (ret == -1) {
		selector_connection_remove(selector, conn);
		net_connection_free(conn);
		EINA_LOG_WARN("Client %d disconnected", conn->socket);
		continue;
	      } else if (ret == 1) {
		protocol_handler_run(conn, request);
	      }
	    }
	    packet_free(request);
	  }

	  if (selector_connection_can_write(selector, (struct connection *)conn)) {
	    struct packet *response = net_connection_response_get(conn);
	    if (response) {
	      packet_write_to_connection(response, conn);
	      packet_free(response);
	    }
	  }
	}
      }
    }
  }

  selector_free(selector);
  net_free(net);

  EINA_LOG_INFO("Shuting down bnetd");
  tristram_shutdown();
  return 0;
}
