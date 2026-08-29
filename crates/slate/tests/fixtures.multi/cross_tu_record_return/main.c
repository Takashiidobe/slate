#include <stdio.h>

struct Version {
  int major;
  int minor;
  int micro;
};

struct Version get_version(void);

int main(void) {
  struct Version v = get_version();
  printf("%d.%d.%d\n", v.major, v.minor, v.micro);
  return 0;
}
