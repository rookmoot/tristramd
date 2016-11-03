#ifndef SELECTOR_H_
#define SELECTOR_H_

struct selector {
  int maxfd;
  fd_set master;
  fd_set read_fds;
  fd_set write_fds;

  Eina_List *conns;
};

struct selector *selector_new(void);
void selector_free(struct selector *selector);

void selector_server_add(struct selector *selector, struct server *server);
void selector_connection_add(struct selector *selector, struct connection *conn);

void selector_connection_remove(struct selector *selector, struct connection *conn);

int  selector_select(struct selector *selector);
int  selector_server_can_accept(struct selector *selector, struct server *server);
int  selector_connection_can_read(struct selector *selector, struct connection *conn);
int  selector_connection_can_write(struct selector *selector, struct connection *conn);
#endif
