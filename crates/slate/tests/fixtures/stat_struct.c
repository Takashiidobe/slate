#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat info = {0};
  int result = stat("/dev/null", &info);
  printf("%d %lld\n", result, (long long)info.st_size);
  return 0;
}
