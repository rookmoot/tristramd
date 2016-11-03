#ifndef FILE_H_
#define FILE_H_

#include <inttypes.h>

struct file {
  char *filename;

  uint64_t time;

  void *data;
  size_t size;
};

struct file *file_new(const char *filename);
void file_free(struct file *file);

uint32_t file_size_get(struct file *file);
uint64_t file_time_get(struct file *file);

void *file_chunk_data_get(struct file *file, size_t *chunk_size);

#endif
