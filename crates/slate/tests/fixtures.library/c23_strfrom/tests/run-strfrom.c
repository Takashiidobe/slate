#include <stddef.h>

extern int format_pi(char *out, size_t n);

int main(void) {
  char output[16] = {0};
  int written = format_pi(output, sizeof(output));
  return (written == 3 && output[0] == '3' && output[1] == '.' && output[2] == '5') ? 0 : 1;
}
