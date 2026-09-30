#include <stdio.h>

static int atomic_increment(int *counter) {
  int old, status;
  __asm__ volatile("1: ldxr %w0, %1\n"
                    "\tadd %w0, %w0, #1\n"
                    "\tstxr %w3, %w0, %2\n"
                    "\tcbnz %w3, 1b"
                    : "=&r"(old), "+Q"(*counter), "+Q"(*counter), "=&r"(status));
  return old;
}

int main(void) {
  int counter = 41;
  int result = atomic_increment(&counter);
  printf("%d %d\n", result, counter);
  return 0;
}
