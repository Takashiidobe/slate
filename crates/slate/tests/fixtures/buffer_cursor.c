#include <stdio.h>

struct cursor {
  int *ptr;
};

int main(void) {
  int values[4];
  for (int i = 0; i < 4; i++) {
    values[i] = i + getchar();
  }
  struct cursor c;
  c.ptr = values + 1;
  struct cursor d;
  d.ptr = values + 3;
  printf("%d %ld\n", *c.ptr + *d.ptr, d.ptr - c.ptr);
  return 0;
}
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-NOT: c.ptr
// REWRITES-NOT: d.ptr
// REWRITES-NOT: .offset_from(
// REWRITES: {{^}}}
