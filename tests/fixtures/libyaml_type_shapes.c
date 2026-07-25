#include <stdio.h>

typedef unsigned char yaml_char_t;
typedef int yaml_read_handler_t(void *, yaml_char_t *, unsigned long,
                                unsigned long *);

typedef struct {
  yaml_read_handler_t *read_handler;
  void *read_handler_data;
} parser_t;

static int read_bytes(void *data, yaml_char_t *buffer, unsigned long size,
                      unsigned long *size_read) {
  yaml_char_t *source = (yaml_char_t *)data;
  for (unsigned long i = 0; i < size; i++) {
    buffer[i] = source[i];
  }
  *size_read = size;
  return 1;
}

int main(void) {
  yaml_char_t input[] = "abc";
  yaml_char_t tag[] = "tag:yaml.org,2002:str";
  yaml_char_t buffer[4] = {0};
  unsigned long size_read = 0;
  parser_t parser;
  parser.read_handler = read_bytes;
  parser.read_handler_data = input;
  int ok = parser.read_handler(parser.read_handler_data, buffer, 3, &size_read);
  printf("%d %lu %c %c\n", ok, size_read, buffer[1], tag[4]);
  return 0;
}
