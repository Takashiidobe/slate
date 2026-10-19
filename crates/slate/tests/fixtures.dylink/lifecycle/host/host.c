#include <stdio.h>
#include "dylink.h"
#include "dylink_path.h"

int main(void) {
  void *first = dl_open(DEP_PATH);
  if (!first) {
    printf("open failed\n");
    return 1;
  }
  int (*loaded)(void) = (int (*)(void))dl_sym(first, "plug_loaded");
  int (*call)(void) = (int (*)(void))dl_sym(first, "plug_call");
  int *(*calls_address)(void) = (int *(*)(void))dl_sym(first, "plug_calls_address");
  if (!loaded || !call || !calls_address) {
    printf("resolve failed\n");
    return 2;
  }
  printf("initializer ran: %d\n", loaded());
  int first_call = call();
  int second_call_count = call();
  int third_call = call();
  printf("calls: %d %d %d\n", first_call, second_call_count, third_call);

  void *second = dl_open(DEP_PATH);
  int (*second_call)(void) = (int (*)(void))dl_sym(second, "plug_call");
  printf("same handle: %s\n", first == second ? "yes" : "no");
  printf("shared state: %d\n", second_call());
  printf("address matches: %s\n",
         (int *(*)(void))dl_sym(second, "plug_calls_address") == calls_address ? "yes" : "no");

  printf("close second: %s\n", dl_close(second) ? "ok" : "failed");
  printf("still usable: %d\n", call());
  printf("close first: %s\n", dl_close(first) ? "ok" : "failed");

  void *again = dl_open(DEP_PATH);
  if (!again) {
    printf("reopen failed\n");
    return 3;
  }
  int (*fresh_call)(void) = (int (*)(void))dl_sym(again, "plug_call");
  printf("fresh state after reload: %d\n", fresh_call());
  dl_close(again);
  return 0;
}
