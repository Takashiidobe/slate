#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

struct GNUCookie {
  char   bytes[32];
  size_t length;
  int    closed;
};

static ssize_t gnu_cookie_write(void *state, const char *buffer, size_t size) {
  struct GNUCookie *cookie = state;
  memcpy(cookie->bytes + cookie->length, buffer, size);
  cookie->length += size;
  return (ssize_t)size;
}

static int gnu_cookie_close(void *state) {
  struct GNUCookie *cookie = state;
  cookie->closed           = 1;
  return 0;
}

static int gnu_cookie_stdio(void) {
  struct GNUCookie      cookie    = {};
  cookie_io_functions_t functions = {};
  FILE                 *stream;
  int                   total = 0;

  functions.write  = gnu_cookie_write;
  functions.close  = gnu_cookie_close;
  stream           = fopencookie(&cookie, "w", functions);
  total           += stream != NULL;
  total           += fprintf(stream, "%s:%d", "cookie", 7) == 8;
  total           += fclose(stream) == 0;
  total           += cookie.closed == 1;
  total           += cookie.length == 8;
  total           += memcmp(cookie.bytes, "cookie:7", 8) == 0;
  return total;
}

int main(void) {
  printf("%d\n", gnu_cookie_stdio());
  return 0;
}


