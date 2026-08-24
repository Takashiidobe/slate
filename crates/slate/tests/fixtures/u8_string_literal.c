#include <stdio.h>

int main(void) {
  static const char text[]    = u8"\u03a9";
  int               alignment = (int)_Alignof(int);
  int               repeated  = (unsigned char)text[1] + (unsigned char)text[1];
  printf("%u %u %u %zu %d %d\n", (unsigned char)text[0], (unsigned char)text[1],
         (unsigned char)text[2], sizeof(text), alignment, repeated);
  return 0;
}
// REWRITES-DAG: static mut main_text: [i8; 3] = [-50, -87, 0];
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: main_text[1]
// REWRITES-DAG: main_text[2]
// REWRITES-DAG: let alignment: i32 = 4;
// REWRITES: {{^}}}
