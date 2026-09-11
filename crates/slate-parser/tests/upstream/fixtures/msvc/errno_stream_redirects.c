#include <errno.h>
#include <stdio.h>

int probe_errno_and_streams(void) {
  errno = 0;
  fputs("hi\n", stdout);
  fputs("bye\n", stderr);
  return errno;
}

int main(void) { return probe_errno_and_streams(); }


