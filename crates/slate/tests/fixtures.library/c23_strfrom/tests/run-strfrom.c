#include <stddef.h>

extern int format_pi(char *out, size_t n);
extern int root_to_int(void);

int main(void) {
  char output[16] = {0};
  int  written    = format_pi(output, sizeof(output));
  return (written == 3 && output[0] == '3' && output[1] == '.' &&
          output[2] == '5' && root_to_int())
             ? 0
             : 1;
}
