#include <stdio.h>

int main(void) {
  static const unsigned char data[] = {
#embed "c23_embed.bin"
  };
  for (size_t i = 0; i < sizeof(data); i++) {
    putchar(data[i]);
  }
  return sizeof(data) == 4 && data[0] == 'C' && data[1] == '2' &&
                 data[2] == '3' && data[3] == '\n'
             ? 0
             : 1;
}
