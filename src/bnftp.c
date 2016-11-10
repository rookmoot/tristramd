#include <stdio.h>
#include <Eina.h>

#include "net.h"
#include "packet.h"
#include "protocol.h"
#include "file.h"

void packet_bnftp(struct connection *conn, struct packet *request) {
  struct packet *response = NULL;
  //uint32_t start_position = 0;
  char *filename = NULL;
  struct file *file = NULL;

  response = packet_new(PROTOCOL_BNFTP);
  if (!response) {
    return;
  }

  packet_get_short(request, ENDIAN_LITTLE);
  packet_get_int(request, ENDIAN_LITTLE);
  packet_get_int(request, ENDIAN_LITTLE);
  packet_get_int(request, ENDIAN_LITTLE);
  packet_get_int(request, ENDIAN_LITTLE);
  //start_position = packet_get_int(request, ENDIAN_LITTLE);
  packet_get_int(request, ENDIAN_LITTLE);

  packet_get_long(request, ENDIAN_LITTLE);
  filename = packet_get_string(request, ENDIAN_LITTLE);

  file = file_new(filename);
  if (!file) {
    EINA_LOG_ERR("Failed to manage file %s", filename);
    free(filename);
    packet_free(response);
    return;
  }

  packet_set_short(response, ENDIAN_LITTLE, 0x00);
  packet_set_int(response, ENDIAN_LITTLE, file_size_get(file));
  packet_set_int(response, ENDIAN_LITTLE, 0x00);
  packet_set_int(response, ENDIAN_LITTLE, 0x00);
  packet_set_long(response, ENDIAN_LITTLE, file_time_get(file));
  packet_set_string(response, ENDIAN_LITTLE, file->filename);
  packet_set_bnftp_file(response, file);
  
  free(filename);
  file_free(file);
  net_connection_response_append(conn, response);
}
