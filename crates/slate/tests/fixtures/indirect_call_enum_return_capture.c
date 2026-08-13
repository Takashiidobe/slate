#include <stdio.h>

typedef enum { E_OK = 0, E_FAIL = 1 } Status;

typedef Status (*Handler)(int);

struct Parser {
  Handler processor;
};

static Status handle_ok(int x) {
  (void)x;
  return E_OK;
}

static Status handle_fail(int x) {
  (void)x;
  return E_FAIL;
}

int main(void) {
  struct Parser p;
  for (int i = 0; i < 2; i++) {
    p.processor   = (i == 0) ? handle_ok : handle_fail;
    Status result = p.processor(5);
    if (result == E_OK) {
      printf("ok\n");
    } else {
      printf("fail\n");
    }
  }
  return 0;
}
