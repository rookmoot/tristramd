#include <stdio.h>
#include <sys/stat.h>
#include <Eina.h>

#include "file.h"
#include "utils.h"

static uint64_t file_time_to_bntime(time_t time);

struct file *file_new(const char *filename) {
  FILE *f = NULL;
  char path[4096];
  struct file *file = NULL;
  struct stat sfile;

  memset(path, 0, 4096);
  
  file = malloc(sizeof(*file));
  if (!file) {
    EINA_LOG_CRIT("Failed to alloc file memory for %s", filename);
    return NULL;
  }
  
  file->filename = strdup(filename);
  snprintf(path, 4096, "etc/files/%s", utils_string_to_lower(filename));
  
  if (stat(path, &sfile) < 0) {
    EINA_LOG_CRIT("Failed to stat'd file %s", path);
    file_free(file);
    return NULL;
  }

  file->size = (size_t)sfile.st_size;
  file->time = (uint64_t)file_time_to_bntime(sfile.st_mtime);

  file->data = malloc(file->size);
  if (!file->data) {
    EINA_LOG_CRIT("Failed to allocate file %s data memory of size %d", file->filename, (int)file->size);
    file_free(file);
    return NULL;
  }

  f = fopen(path, "rb");
  if (!f) {
    EINA_LOG_CRIT("Failed to open file %s", path);
    file_free(file);
    return NULL;
  }

  fread(file->data, 1, (long)file->size, f);
  fclose(f);

  EINA_LOG_INFO("Loading new file %s with size of %d and time of %ld", path, (int)file->size, (long int)file->time);
  
  return file;
}

void file_free(struct file *file) {
  if (file->data) {
    free(file->data);
  }
  free(file->filename);
  free(file);
}

uint32_t file_size_get(struct file *file) {
  return file->size;
}

uint64_t file_time_get(struct file *file) {
  return file->time;
}

void *file_data_get(struct file *file, size_t *chunk_size) {
  *chunk_size = file->size;
  return file->data;
}


static uint64_t file_time_to_bntime(time_t time) {
  return (uint64_t)(time * 10000000) + 116444736000000000;
}


void *file_chunk_data_get(struct file *file, size_t *chunk_size) {
  *chunk_size = file->size;
  return file->data;
}
