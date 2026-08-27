#define _GNU_SOURCE
#include <error.h>
#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

static void handle_exit(int status, void *arg) {
  int *captured = (int *)arg;
  *captured = status;
  printf("on_exit:%d\n", status);
}

int main(void) {
  int captured = -1;
  on_exit(handle_exit, &captured);

  int   mcheck_enabled = mcheck(NULL) == 0;
  void *block          = malloc(16);
  enum mcheck_status probe = mprobe(block);
  printf("mcheck:%d %d\n", mcheck_enabled, probe == MCHECK_OK);
  free(block);

  error_one_per_line = 0;
  error(0, 0, "first message");
  error(0, 0, "second message");
  printf("count_after_two:%u\n", error_message_count);

  error_one_per_line = 1;
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  printf("count_after_dedup:%u\n", error_message_count);

  error_at_line(0, 0, "sample.c", 43, "different line");
  printf("count_after_new_line:%u\n", error_message_count);

  error(5, 0, "fatal message");
  printf("unreachable\n");
  return 0;
}
