#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info = {0};
  if (stat("/dev/null", &info) != 0)
    return 1;
  printf("%lld\n", (long long)info.st_mtime);
}
